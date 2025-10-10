#ifndef ENV_H
#define ENV_H
#include "engine/defaultNode/global.h"
#include "engine/defaultNode/stagehand.h"
#include "engine/defaultNode/overworld_scene_changer.h"
#include "src/mainNode/mainNode.h"
using namespace godot;
extern MainNode* sys;
extern Global* global;
extern OverworldSceneChanger* scene_changer;
extern Stagehand* stagehand;
extern bool isEditor;

#endif