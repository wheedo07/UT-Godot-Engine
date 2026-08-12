#include "ut_editor_plugin.h"
#include "undertale-plugin/Button/run.h"
#include "debugger_editor_plugin.h"
using namespace godot;

void UTEditorPlugin::_bind_methods() {
}

void UTEditorPlugin::_enter_tree() {
    debugger.instantiate();
    add_debugger_plugin(debugger);
    UTEditorRunBtn* runBtn = memnew(UTEditorRunBtn);
    add_control_to_container(
        CONTAINER_TOOLBAR,
        runBtn
    );
}

void UTEditorPlugin::_exit_tree() {
    remove_debugger_plugin(debugger);
}