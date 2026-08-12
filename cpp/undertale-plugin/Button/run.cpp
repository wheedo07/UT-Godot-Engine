#include "run.h"
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/editor_interface.hpp>
using namespace godot;

void UTEditorRunBtn::_bind_methods() {
}

void UTEditorRunBtn::_ready() {
    set_text("UTGE RUN");
}

void UTEditorRunBtn::_pressed() {
    EditorInterface::get_singleton()->play_main_scene();
}