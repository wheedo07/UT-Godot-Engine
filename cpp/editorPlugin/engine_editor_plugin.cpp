#include "engine_editor_plugin.h"
#include "textBox_editor_plugin.h"
using namespace godot;

EngineEditorPlugin::EngineEditorPlugin() {}

EngineEditorPlugin::~EngineEditorPlugin() {}

void EngineEditorPlugin::_bind_methods() {}

void EngineEditorPlugin::_enter_tree() {
    Ref<TextBoxEditorPlugin> text_box_inspector_plugin = memnew(TextBoxEditorPlugin);
    add_inspector_plugin(text_box_inspector_plugin);
}

void EngineEditorPlugin::_exit_tree() {}