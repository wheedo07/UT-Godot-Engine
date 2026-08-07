#include "scene_tree.h"
using namespace godot;

void UTGESceneTree::_bind_methods() {
    ClassDB::bind_method(D_METHOD("create_timer", "time_sec", "process_always", "process_in_physics", "ignore_time_scale"), &UTGESceneTree::create_timer, DEFVAL(true), DEFVAL(false), DEFVAL(false));
}

Ref<SceneTreeTimer> UTGESceneTree::create_timer(double p_time_sec, bool p_process_always, bool p_process_in_physics, bool p_ignore_time_scale) {
    print_line("UTGESceneTree::create_timer called");
    return SceneTree::create_timer(p_time_sec, p_process_always, p_process_in_physics, p_ignore_time_scale);
}