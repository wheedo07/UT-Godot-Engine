#include "blittering_casual.h"
using namespace godot;

void BlitteringCasualBoxBehaviour::_bind_methods() {}

void BlitteringCasualBoxBehaviour::_on_gain_control() {
    box->screens[BattleBox::State_BlitteringCasual].call("show");
}

void BlitteringCasualBoxBehaviour::_on_lose_control() {
    box->screens[BattleBox::State_BlitteringCasual].call("hide");
}