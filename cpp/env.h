#ifndef __ENV_H__
#define __ENV_H__
#include "engine/defaultNode/global.h"
#include "engine/defaultNode/stagehand.h"
#include "engine/defaultNode/scene_changer.h"
#include "src/mainNode/mainNode.h"
using namespace godot;
extern MainNode *sys;
extern Global *global;
extern SceneChanger *scene_changer;
extern Stagehand *stagehand;
extern bool isEditor;

#endif