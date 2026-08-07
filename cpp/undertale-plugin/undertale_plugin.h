#pragma once

#include<godot_cpp/core/class_db.hpp>
using namespace godot;

#include "Editor/ut_editor_plugin.h"

void undertale_plugin_init(ModuleInitializationLevel p_level);
void undertale_plugin_uninit(ModuleInitializationLevel p_level);

void on_startup();
void on_shutdown();
void on_frame();