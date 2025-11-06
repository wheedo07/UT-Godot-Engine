#ifndef EDITOR_PLUGIN_REGISTER_H
#define EDITOR_PLUGIN_REGISTER_H
#include<godot_cpp/core/class_db.hpp>
using namespace godot;

#include "editorPlugin/engine_editor_plugin.h"
#include "editorPlugin/textBox_editor_plugin.h"

void editorPlugin_init(ModuleInitializationLevel p_level);
void editorPlugin_uninit(ModuleInitializationLevel p_level);

#endif