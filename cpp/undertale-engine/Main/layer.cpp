#include "layer.h"
#include "env.h"
using namespace godot;

void UTGELayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_layer_id", "value"), &UTGELayer::set_layer_id);
    ClassDB::bind_method(D_METHOD("get_layer_id"), &UTGELayer::get_layer_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "layer_id"), "set_layer_id", "get_layer_id");

    ClassDB::bind_method(D_METHOD("change_scene", "scene"), &UTGELayer::change_scene);
    ClassDB::bind_method(D_METHOD("get_current_scene"), &UTGELayer::get_current_scene);
    ClassDB::bind_method(D_METHOD("clear"), &UTGELayer::clear);
    ClassDB::bind_method(D_METHOD("pause"), &UTGELayer::pause);
    ClassDB::bind_method(D_METHOD("unpause"), &UTGELayer::unpause);
}

Node *UTGELayer::change_scene(Ref<PackedScene> scene) {
    ERR_FAIL_COND_V(scene.is_null(), nullptr);
    clear();
    current_scene = scene->instantiate();
    add_child(current_scene);
    return current_scene;
}

Node *UTGELayer::get_current_scene() {
    return current_scene;
}

void UTGELayer::clear() {
    if(!current_scene) return;
    current_scene->queue_free();
    current_scene = nullptr;
}

void UTGELayer::pause() {
}

void UTGELayer::unpause() {
}

void UTGELayer::set_layer_id(StringName value) {
    layer_id = value;
}

StringName UTGELayer::get_layer_id() {
    return layer_id;
}