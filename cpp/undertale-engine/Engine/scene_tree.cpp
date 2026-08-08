#include "scene_tree.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
using namespace godot;

void UTGESceneTree::_bind_methods() {
    ClassDB::bind_method(D_METHOD("change_scene_to_file", "layer_id", "path"), &UTGESceneTree::change_scene_to_file);
    ClassDB::bind_method(D_METHOD("change_scene_to_packed", "layer_id", "scene"), &UTGESceneTree::change_scene_to_packed);
    ClassDB::bind_method(D_METHOD("change_scene_to_node", "layer_id", "node"), &UTGESceneTree::change_scene_to_node);
    ClassDB::bind_method(D_METHOD("create_timer", "time_sec", "process_always", "process_in_physics", "ignore_time_scale"), &UTGESceneTree::create_timer, DEFVAL(true), DEFVAL(false), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("layer_timer", "layer_id", "time_sec", "process_always", "process_in_physics", "ignore_time_scale"), &UTGESceneTree::layer_timer, DEFVAL(true), DEFVAL(false), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("pause_layer_timers", "layer_id", "force"), &UTGESceneTree::pause_layer_timers, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("resume_layer_timers", "layer_id", "force"), &UTGESceneTree::resume_layer_timers, DEFVAL(false));
}

bool UTGESceneTree::_physics_process(double p_delta) {
    _process_timers(p_delta, true);
    return false;
}

bool UTGESceneTree::_process(double p_delta) {
    _process_timers(p_delta, false);
    return false;
}

Error UTGESceneTree::change_scene_to_file(StringName layer_id, String path) {
    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(path);
    ERR_FAIL_COND_V(scene.is_null(), ERR_FILE_NOT_FOUND);
    return change_scene_to_packed(layer_id, scene);
}

Error UTGESceneTree::change_scene_to_packed(StringName layer_id, Ref<PackedScene> scene) {
    ERR_FAIL_COND_V(scene.is_null(), ERR_INVALID_PARAMETER);
    Node *node = scene->instantiate();
    return change_scene_to_node(layer_id, node);
}

Error UTGESceneTree::change_scene_to_node(StringName layer_id, Node *node) {
    ERR_FAIL_COND_V(!node, ERR_INVALID_PARAMETER);
    UTGERoot *root = get_root();
    ERR_FAIL_COND_V(!root, ERR_UNAVAILABLE);
    UTGELayer *layer = root->get_layer(layer_id);
    ERR_FAIL_COND_V(!layer, ERR_DOES_NOT_EXIST);
    return layer->change_scene(node);
}

Ref<UTGETimer> UTGESceneTree::create_timer(double p_time_sec, bool p_process_always, bool p_process_in_physics, bool p_ignore_time_scale) {
    Ref<UTGETimer> timer = memnew(UTGETimer);
    timer->set_time_left(p_time_sec);
    timer->set_process_always(p_process_always);
    timer->set_process_in_physics(p_process_in_physics);
    timer->set_ignore_time_scale(p_ignore_time_scale);
    timers.push_back(timer);
    return timer;
}

Ref<UTGETimer> UTGESceneTree::layer_timer(StringName layer_id, double p_time_sec, bool p_process_always, bool p_process_in_physics, bool p_ignore_time_scale) {
    Ref<UTGETimer> timer = create_timer(p_time_sec, p_process_always, p_process_in_physics, p_ignore_time_scale);
    timer->set_layer_id(layer_id);
    return timer;
}

void UTGESceneTree::pause_layer_timers(StringName layer_id, bool force) {
    for(int i=0; i < timers.size(); ++i) {
        Ref<UTGETimer> timer = timers[i];
        if(timer.is_null()) continue;
        if(timer->get_layer_id() == layer_id && (force || !timer->is_process_always())) {
            timer->set_paused(true);
        }
    }
}

void UTGESceneTree::resume_layer_timers(StringName layer_id, bool force) {
    for(int i=0; i < timers.size(); ++i) {
        Ref<UTGETimer> timer = timers[i];
        if(timer.is_null()) continue;
        if(timer->get_layer_id() == layer_id && (force || !timer->is_process_always())) {
            timer->set_paused(false);
        }
    }
}

UTGERoot *UTGESceneTree::get_root() {
    return Object::cast_to<UTGERoot>(SceneTree::get_current_scene());
}

void UTGESceneTree::_process_timers(double p_delta, bool p_process_in_physics) {
    const double tree_time_scale = Engine::get_singleton()->get_time_scale();
    const bool tree_paused = is_paused();
    const int timer_count = timers.size();

    for(int i = timer_count - 1; i >= 0; --i) {
        Ref<UTGETimer> timer = timers[i];
        if(timer.is_null() || timer->is_finished()) {
            timers.remove_at(i);
            continue;
        }
        if(timer->is_processing_in_physics() != p_process_in_physics) continue;
        if(timer->process(p_delta, tree_time_scale, tree_paused)) {
            timers.remove_at(i);
        }
    }
}