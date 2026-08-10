#include "ut_editor_plugin.h"
#include "undertale-plugin/Button/run.h"
using namespace godot;

void UTEditorPlugin::_bind_methods() {
}

void UTEditorPlugin::_enter_tree() {
    UTEditorRunBtn* runBtn = memnew(UTEditorRunBtn);
    add_control_to_container(
        CONTAINER_TOOLBAR,
        runBtn
    );
}

void UTEditorPlugin::_exit_tree() {
}