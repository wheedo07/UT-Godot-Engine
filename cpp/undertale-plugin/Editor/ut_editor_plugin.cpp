#include "ut_editor_plugin.h"
#include<godot_cpp/classes/button.hpp>
using namespace godot;

void UTEditorPlugin::_bind_methods() {
}

void UTEditorPlugin::_enter_tree() {
    Button *button = memnew(Button);
    button->set_text("UT Run");
    
    add_control_to_container(
        CONTAINER_TOOLBAR,
        button
    );
}

void UTEditorPlugin::_exit_tree() {
}