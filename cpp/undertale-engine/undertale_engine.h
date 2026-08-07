#pragma once

#include<godot_cpp/core/class_db.hpp>
using namespace godot;

/* [Global] */
#include "Global/ut.h"

/* [Main] */
#include "Main/root.h"
#include "Main/layer.h"
#include "Main/scene_tree.h"

void undertale_engine_init(ModuleInitializationLevel p_level);
void undertale_engine_uninit(ModuleInitializationLevel p_level);

void on_startup();
void on_shutdown();
void on_frame();