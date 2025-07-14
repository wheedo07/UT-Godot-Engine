#ifndef RegisterTYPE_H
#define RegisterTYPE_H
#include<godot_cpp/core/class_db.hpp>
#include "mainNode/mainNode.h"
#include "mainNode/dropText.h"
#include "trigger/text_trigger.h"
#include "trigger/move_trigger.h"

// attackNode
#include "mainAttacks/sans/attack_sans.h"

// overworld Node
#include "overworld/test.h"

// enemy Node
#include "enemy/sans/enemy_sans.h"

// Bullets Node
using namespace godot;

void register_init();

#endif
