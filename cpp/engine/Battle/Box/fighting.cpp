#include "fighting.h"
#include "env.h"

void FightingBoxBehaviour::_bind_methods() {}

void FightingBoxBehaviour::_on_gain_control() {
    box->emit_signal("exit_menu");
    box->emit_signal("fight", box->current_target_id);
    global->isDebugTurn = false;
}

void FightingBoxBehaviour::_on_lose_control() {}