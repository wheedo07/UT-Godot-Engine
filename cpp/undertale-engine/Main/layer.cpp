#include "layer.h"
#include "undertale-engine/Global/ut.h"
using namespace godot;

void UTGELayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_layer_id", "value"), &UTGELayer::set_layer_id);
    ClassDB::bind_method(D_METHOD("get_layer_id"), &UTGELayer::get_layer_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "layer_id"), "set_layer_id", "get_layer_id");

    ClassDB::bind_method(D_METHOD("change_scene", "node"), &UTGELayer::change_scene);
    ClassDB::bind_method(D_METHOD("get_current_scene"), &UTGELayer::get_current_scene);
    ClassDB::bind_method(D_METHOD("clear"), &UTGELayer::clear);
    ClassDB::bind_method(D_METHOD("pause"), &UTGELayer::pause);
    ClassDB::bind_method(D_METHOD("resume"), &UTGELayer::resume);
}

Error UTGELayer::change_scene(Node *node) {
    ERR_FAIL_COND_V(!node, ERR_INVALID_PARAMETER);
    clear();
    add_child(node);
    current_scene = node;
    return OK;
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
    process_mode_cache = get_process_mode();
    set_process_mode(ProcessMode::PROCESS_MODE_DISABLED);
    UT::tree()->pause_layer_timers(layer_id);
}

void UTGELayer::resume() {
    set_process_mode(process_mode_cache);
    UT::tree()->resume_layer_timers(layer_id);
}

void UTGELayer::set_layer_id(StringName value) {
    layer_id = value;
}

StringName UTGELayer::get_layer_id() {
    return layer_id;
}