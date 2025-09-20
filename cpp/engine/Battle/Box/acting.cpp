#include "acting.h"
using namespace godot;

void ActingBoxBehaviour::_bind_methods() {}

void ActingBoxBehaviour::_on_gain_control() {
    Array arr = box->history[box->button_choice];
    if(arr[1]) box->soul_position = arr[1];
    else box->soul_position = Vector2i(0, 0);
    box->set_options();
    box->call_deferred("_soul_choice", Vector2(0,0));
    box->screens[BattleBox::State_Acting].call("show");
}

void ActingBoxBehaviour::input(const Ref<InputEvent>& event) {
    if(event->is_action_pressed("ui_cancel")) box->call_deferred("_backout");

    else if(event->is_action_pressed("ui_accept")) {
        box->change_state(BattleBox::State_Blittering);
        box->blitter_act();
    }
}

void ActingBoxBehaviour::_on_lose_control() {
    Array arr = box->history[box->button_choice];
    arr[1] = box->soul_position;
    box->screens[BattleBox::State_Acting].call("hide");
}