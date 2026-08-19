#include "root.h"
#include "undertale-engine/Global/ut.h"
#include<godot_cpp/classes/engine_debugger.hpp>
using namespace godot;

UTGERoot::UTGERoot() {
    connect("ready", callable_mp(this, &UTGERoot::_utge_ready));
}

void UTGERoot::_bind_methods() {
    /* API 메서드 */
    ClassDB::bind_method(D_METHOD("get_layer", "layer_id"), &UTGERoot::get_layer, DEFVAL("main"));
    ClassDB::bind_method(D_METHOD("get_layers"), &UTGERoot::get_layers);

    /* 스크립트 속성 */
    ClassDB::bind_method(D_METHOD("set_current_camera", "value"), &UTGERoot::set_current_camera);
    ClassDB::bind_method(D_METHOD("get_current_camera"), &UTGERoot::get_current_camera);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "current_camera", PROPERTY_HINT_NODE_TYPE, "UTGECamera", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_current_camera", "get_current_camera");

    /* 공개 속성 */
    ClassDB::bind_method(D_METHOD("set_layer_parent", "value"), &UTGERoot::set_layer_parent);
    ClassDB::bind_method(D_METHOD("get_layer_parent"), &UTGERoot::get_layer_parent);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "layer_parent", PROPERTY_HINT_NODE_TYPE, "Node"), "set_layer_parent", "get_layer_parent");

    ClassDB::bind_method(D_METHOD("set_camera", "value"), &UTGERoot::set_camera);
    ClassDB::bind_method(D_METHOD("get_camera"), &UTGERoot::get_camera);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera2D"), "set_camera", "get_camera");
}

void UTGERoot::_utge_ready() {
    TypedArray<Node> children = layer_parent->get_children();
    for(int i=0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        if(child->is_class("UTGELayer")) {
            UTGELayer *layer = Object::cast_to<UTGELayer>(child);
            layers[layer->get_layer_id()] = layer;
        }
    }

    EngineDebugger *debugger = EngineDebugger::get_singleton();
    if(debugger && debugger->is_active()) {
        debugger->register_message_capture("ut_debugger", callable_mp(this, &UTGERoot::_debugger_capture));
    }
}

bool UTGERoot::_debugger_capture(String message, Array data) {
    if(message == "call") {
        if(data.size() < 4) return true;
        const int64_t request_id = data[0];
        const NodePath path = data[1];
        const StringName method = data[2];
        const Array args = data[3];

        Node *node = get_node_or_null(path);

        if(!node) {
            print_error("Node not found: ", path);
            return true;
        }

        if(!node->has_method(method)) {
            print_error("Method not found: ", method);
            return true;
        }

        Variant result = node->callv(method, args);
        EngineDebugger::get_singleton()->send_message(
            "ut_debugger:call_result",
            { request_id, result }
        );
        return true;
    }else if(message == "tree_call") {
        if(data.size() < 3) return true;
        const int64_t request_id = data[0];
        const StringName method = data[1];
        const Array args = data[2];

        UTGESceneTree *tree = UT::tree();

        if(!tree) {
            print_error("SceneTree is null.");
            return true;
        }
        if(!tree->has_method(method)) {
            print_error("SceneTree method not found: ", method);
            return true;
        }
        Variant result = tree->callv(method, args);

        EngineDebugger::get_singleton()->send_message(
            "ut_debugger:tree_call_result",
            { request_id, result }
        );
        return true;
    }
    return false;
}

UTGELayer *UTGERoot::get_layer(StringName layer_id) {
    UTGELayer **layer_ptr = layers.getptr(layer_id);
    if(layer_ptr) {
        return *layer_ptr;
    }else return nullptr;
}

TypedArray<UTGELayer> UTGERoot::get_layers() {
    TypedArray<UTGELayer> result;
    for(auto it = layers.begin(); it != layers.end(); ++it) {
        result.push_back(it->value);
    }
    return result;
}

void UTGERoot::set_layer_parent(Node *value) {
    layer_parent = value;
}

Node *UTGERoot::get_layer_parent() {
    return layer_parent;
}

void UTGERoot::set_camera(Camera2D *value) {
    camera = value;
}

Camera2D *UTGERoot::get_camera() {
    return camera;
}

void UTGERoot::set_current_camera(UTGECamera *value) {
    if(current_camera == value) return;
    if(current_camera) current_camera->deactivate();
    if(value) value->activate();
    current_camera = value;
}

UTGECamera *UTGERoot::get_current_camera() {
    return current_camera;
}