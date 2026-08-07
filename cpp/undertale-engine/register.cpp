#include "undertale_engine.h"
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

void undertale_engine_init(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
    /* [Global] */
    GDREGISTER_RUNTIME_CLASS(UT);
    
    /* [Engine] */
    GDREGISTER_CLASS(UTGESceneTree);
    GDREGISTER_RUNTIME_CLASS(UTGEPool);
    
    /* [Main] */
    GDREGISTER_RUNTIME_CLASS(UTGERoot);
    GDREGISTER_CLASS(UTGELayer);
}

void undertale_engine_uninit(ModuleInitializationLevel p_level) {
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}