#include "register.h"
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

void editorPlugin_init(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_EDITOR) return;

    GDREGISTER_CLASS(EngineEditorPlugin);
    GDREGISTER_CLASS(TextBoxEditorPlugin);

    EditorPlugins::add_by_type<EngineEditorPlugin>();
}

void editorPlugin_uninit(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_EDITOR) return;
}

extern "C" {
    GDExtensionBool GDE_EXPORT undertale_editorPlugin_main_init(GDExtensionInterfaceGetProcAddress p_get, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_init) {
        GDExtensionBinding::InitObject init_obj(p_get, p_library, r_init);
        init_obj.register_initializer(editorPlugin_init);
        init_obj.register_terminator(editorPlugin_uninit);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);
        return init_obj.init();
    }
}