#include "undertale_plugin.h"
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

void undertale_plugin_init(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_EDITOR) return;
    GDREGISTER_CLASS(UTEditorPlugin);
    EditorPlugins::add_by_type<UTEditorPlugin>();
}

void undertale_plugin_uninit(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_EDITOR) return;
}