#include "root.h"
using namespace godot;

void UTGERoot::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_layer", "layer_id"), &UTGERoot::get_layer, DEFVAL("main"));

    ClassDB::bind_method(D_METHOD("set_layer_parent", "value"), &UTGERoot::set_layer_parent);
    ClassDB::bind_method(D_METHOD("get_layer_parent"), &UTGERoot::get_layer_parent);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "layer_parent", PROPERTY_HINT_NODE_TYPE, "Node"), "set_layer_parent", "get_layer_parent");
}

void UTGERoot::_ready() {
    TypedArray<Node> children = layer_parent->get_children();
    for(int i=0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        if(child->is_class("UTGELayer")) {
            UTGELayer *layer = Object::cast_to<UTGELayer>(child);
            layers[layer->get_layer_id()] = layer;
        }
    }
}

UTGELayer *UTGERoot::get_layer(StringName layer_id) {
    UTGELayer **layer_ptr = layers.getptr(layer_id);
    if(layer_ptr) {
        return *layer_ptr;
    }else return nullptr;
}

void UTGERoot::set_layer_parent(Node *value) {
    layer_parent = value;
}

Node *UTGERoot::get_layer_parent() {
    return layer_parent;
}