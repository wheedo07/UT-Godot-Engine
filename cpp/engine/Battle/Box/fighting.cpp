#include "fighting.h"
#include<godot_cpp/classes/scene_tree.hpp>
using namespace godot;

void FightingBoxBehaviour::_bind_methods() {}

void FightingBoxBehaviour::_on_gain_control() {
    box->emit_signal("exit_menu");
    box->emit_signal("fight", box->current_target_id);
}

void FightingBoxBehaviour::_on_lose_control() {}