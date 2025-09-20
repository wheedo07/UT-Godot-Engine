#include "mercying.h"
using namespace godot;

void MercyingBoxBehaviour::_bind_methods() {}

void MercyingBoxBehaviour::_on_gain_control() {
    Array arr = box->history[box->button_choice];
    if(arr[1]) box->soul_position = arr[1];
    box->_soul_choice(Vector2(0,0));
    box->set_mercy_options();
    box->screens[BattleBox::State_Mercying].call("show");
}

void MercyingBoxBehaviour::input(const Ref<InputEvent>& event) {
    if(event->is_action_pressed("ui_cancel")) {
        box->_backout();
        box->emit_signal("moved_to_buttons");
    }else if(event->is_action_pressed("ui_accept")) {
        box->blitter_mercy();
    }
}

void MercyingBoxBehaviour::_on_lose_control() {
    Array arr = box->history[box->button_choice];
    arr[1] = box->soul_position;
    box->screens[BattleBox::State_Mercying].call("hide");
}