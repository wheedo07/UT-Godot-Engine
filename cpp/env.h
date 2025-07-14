#ifndef ENV_H
#define ENV_H
#include "engine/defaultNode/global.h"
#include "engine/defaultNode/audio_player.h"
#include "engine/defaultNode/overworld_scene_changer.h"
#include "src/mainNode/mainNode.h"
using namespace godot;
extern MainNode* sys;
extern Global* global;
extern OverworldSceneChanger* scene_changer;
extern AudioPlayer* audio_player;
extern int isEditor;

#endif