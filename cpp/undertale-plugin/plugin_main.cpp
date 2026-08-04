#include "undertale_plugin.h"
#include<godot_cpp/classes/engine.hpp>
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

extern "C" {
    GDExtensionBool GDE_EXPORT undertale_plugin_main_init(GDExtensionInterfaceGetProcAddress p_get, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_init) {
        GDExtensionBinding::InitObject init_obj(p_get, p_library, r_init);
        init_obj.register_initializer(undertale_plugin_init);
        init_obj.register_terminator(undertale_plugin_uninit);
        init_obj.register_startup_callback(on_startup);
        init_obj.register_shutdown_callback(on_shutdown);
        init_obj.register_frame_callback(on_frame);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);
        return init_obj.init();
    }
}

void on_startup() {}

void on_shutdown() {}

void on_frame() {}