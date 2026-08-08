#include "scene_tree.h"
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

void UTGESceneTree::_bind_methods() {
    ClassDB::bind_method(D_METHOD("change_scene", "layer_id", "scene"), &UTGESceneTree::change_scene);
    ClassDB::bind_method(D_METHOD("create_timer", "time_sec", "process_always", "process_in_physics", "ignore_time_scale"), &UTGESceneTree::create_timer, DEFVAL(true), DEFVAL(false), DEFVAL(false));
}

bool UTGESceneTree::_physics_process(double p_delta) {
    _process_timers(p_delta, true);
    return false;
}

bool UTGESceneTree::_process(double p_delta) {
    _process_timers(p_delta, false);
    return false;
}

Node *UTGESceneTree::change_scene(StringName layer_id, Ref<PackedScene> scene) {
    return get_root()->get_layer(layer_id)->change_scene(scene);
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