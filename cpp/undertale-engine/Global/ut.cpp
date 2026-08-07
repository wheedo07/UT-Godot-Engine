#include "ut.h"
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

void UT::_bind_methods() {
    ClassDB::bind_static_method("UT", D_METHOD("tree"), &UT::tree);
}

UTGESceneTree* UT::tree() {
    return Object::cast_to<UTGESceneTree>(Engine::get_singleton()->get_main_loop());
}