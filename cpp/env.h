#ifndef ENV_H
#define ENV_H
#include "engine/defaultNode/global.h"
#include "engine/defaultNode/stagehand.h"
#include "engine/defaultNode/scene_changer.h"
#include "src/mainNode/mainNode.h"
using namespace godot;
extern MainNode* sys;
extern Global* global;
extern SceneChanger* scene_changer;
extern Stagehand* stagehand;
extern bool isEditor;

#endif