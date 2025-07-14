#include "iteming.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>

void ItemingBoxBehaviour::_bind_methods() {}

void ItemingBoxBehaviour::_on_gain_control() {
    if(global->get_items().is_empty()) {
        box->emit_signal("moved_to_buttons");
        box->backout();
        return;
    }
    box->current_page = 0;
    box->scroll_counter = 0;
    box->soul_position = Vector2();
    box->set_items();
    box->soul_choice(Vector2());
    box->screens[BattleBox::State_Iteming].call("show");
}

void ItemingBoxBehaviour::input(const Ref<InputEvent>& event) {
    if(event->is_action_pressed("ui_down")) {
        if(box->soul_position.y < box->items_per_page-1 && box->soul_position.y < (global->get_items().size() - box->current_page) - 1)
            box->soul_choice(Vector2i(0, 1));
        else if(box->current_page < global->get_items().size()-box->items_per_page) {
            box->current_page++;
        }
        box->call_deferred("set_items");
    }else if(event->is_action_pressed("ui_up")) {
        if(box->soul_position.y > 0) 
            box->soul_choice(Vector2i(0, -1));
        else if(box->current_page > 0) {
            box->current_page--;
        }
        box->call_deferred("set_items");
    }else if(event->is_action_pressed("ui_cancel")) {
        box->backout();
        box->emit_signal("moved_to_buttons");
    }else if(event->is_action_pressed("ui_accept")) {
        if(!global->get_items().size() > 0) {
            box->emit_signal("moved_to_buttons");
            box->backout();
            return;
        }
        box->change_state(BattleBox::State_Blittering);
        box->set_used_item(global->get_items()[box->soul_pos_to_id(box->soul_position, true)]);
        box->blitter_item();
    }
}

void ItemingBoxBehaviour::_on_lose_control() {
    box->screens[BattleBox::State_Iteming].call("hide");
}