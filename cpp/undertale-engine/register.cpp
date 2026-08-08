#include "undertale_engine.h"
#include<godot_cpp/classes/engine.hpp>
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

void undertale_engine_init(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
    /* [Global] */
    GDREGISTER_CLASS(UT);
    
    /* [Engine] */
    GDREGISTER_CLASS(UTGESceneTree);
    GDREGISTER_CLASS(UTGETimer);
    GDREGISTER_CLASS(UTGEPool);
    Engine::get_singleton()->register_singleton("UTGEPool", memnew(UTGEPool));
    
    /* [Main] */
    GDREGISTER_RUNTIME_CLASS(UTGERoot);
    GDREGISTER_RUNTIME_CLASS(UTGELayer);
}

void undertale_engine_uninit(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}