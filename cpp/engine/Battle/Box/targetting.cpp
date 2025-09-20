#include "targetting.h"
#include "env.h"

void TargettingBoxBehaviour::_bind_methods() {}

void TargettingBoxBehaviour::_on_gain_control() {
    Array arr = box->history[box->button_choice];
    if(arr[0]) box->soul_position = arr[0];
    box->call_deferred("_soul_choice", Vector2(0,0));
    box->choices_extends.resize(box->enemies.size());
    box->choices_extends.fill(1);
    box->screens[BattleBox::State_TargetEnemy].call("show");
    box->_set_targets(box->button_choice ? false : true);
}

void TargettingBoxBehaviour::input(const Ref<InputEvent>& event) {
    if(event->is_action_pressed("ui_cancel")) {
        box->_backout();
        box->emit_signal("moved_to_buttons");
    }else if(event->is_action_pressed("ui_accept")) {
        int id = box->soul_pos_to_id(box->soul_position, false, 1);
        if(box->button_choice == BattleBox::OPTION_FIGHT && box->enemies[id])
            box->change_state(BattleBox::State_Fighting);
        else if(box->button_choice == BattleBox::OPTION_ACT && box->enemies[id])
            box->change_state(BattleBox::State_Acting);
        else if(box->button_choice == BattleBox::OPTION_MERCY && box->enemies[id])
            box->change_state(BattleBox::State_Mercying);
    }
}

void TargettingBoxBehaviour::_on_lose_control() {
    box->current_target_id = box->soul_pos_to_id(box->soul_position, false, 1);
    Array arr = box->history[box->button_choice];
    arr[0] = box->soul_position;
    box->screens[BattleBox::State_TargetEnemy].call("hide");
}