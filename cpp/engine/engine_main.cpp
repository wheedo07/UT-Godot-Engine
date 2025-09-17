#include "undertale_engine.h"
#include "env.h"
#include<godot_cpp/classes/engine.hpp>
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

extern "C" {
    GDExtensionBool GDE_EXPORT undertale_engine_main_init(GDExtensionInterfaceGetProcAddress p_get, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_init) {
        GDExtensionBinding::InitObject init_obj(p_get, p_library, r_init);
        init_obj.register_initializer(undertale_engine_init);
        init_obj.register_terminator(undertale_engine_uninit);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        return init_obj.init();
    }
}

void on_startup() {
    isEditor = Engine::get_singleton()->is_editor_hint();
}

void on_shutdown() {}

void on_frame() {}