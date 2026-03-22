#ifndef __RegisterTYPE_H__
#define __RegisterTYPE_H__
#include<godot_cpp/core/class_db.hpp>
#include "mainNode/mainNode.h"

// attackNode
#include "mainAttacks/sans/attack_sans.h"

// enemy Node
#include "enemy/sans/enemy_sans.h"

// utils
#include "util/cutscene_sequence.h"
#include "util/signal_condition.h"

using namespace godot;
void register_init();

#endif
