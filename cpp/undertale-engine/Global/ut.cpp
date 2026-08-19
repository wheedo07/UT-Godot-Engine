#include "ut.h"
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

void UT::_bind_methods() {
    /* API 메서드 */
    ClassDB::bind_static_method("UT", D_METHOD("tree"), &UT::tree);
    ClassDB::bind_static_method("UT", D_METHOD("pool"), &UT::pool);
}

UTGESceneTree *UT::tree() {
    return Object::cast_to<UTGESceneTree>(Engine::get_singleton()->get_main_loop());
}

UTGEPool *UT::pool() {
    return Object::cast_to<UTGEPool>(Engine::get_singleton()->get_singleton("UTGEPool"));
}