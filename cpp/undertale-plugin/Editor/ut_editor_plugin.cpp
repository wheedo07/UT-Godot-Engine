#include "ut_editor_plugin.h"
#include "debugger_editor_plugin.h"
#include "undertale-plugin/Button/run.h"
#include<godot_cpp/classes/editor_interface.hpp>
using namespace godot;

void UTEditorPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("current_play", "layer_id", "scene_path"), &UTEditorPlugin::current_play);
}

void UTEditorPlugin::_enter_tree() {
    debugger.instantiate();
    add_debugger_plugin(debugger);

    UTEditorRunBtn* runBtn = memnew(UTEditorRunBtn);
    runBtn->set_plugin(this);
    add_control_to_container(
        CONTAINER_TOOLBAR,
        runBtn
    );
}

void UTEditorPlugin::_exit_tree() {
    remove_debugger_plugin(debugger);
}

void UTEditorPlugin::current_play(StringName layer_id, String scene_path) {
    EditorInterface *editor = EditorInterface::get_singleton();
    ERR_FAIL_NULL(editor);

    if(editor->is_playing_scene()) {
        editor->stop_playing_scene();
    }

    debugger->queue_run_request(layer_id, scene_path);
    editor->play_main_scene();
}