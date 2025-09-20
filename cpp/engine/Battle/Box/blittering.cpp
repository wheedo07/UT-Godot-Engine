#include "blittering.h"
using namespace godot;

void BlitteringBoxBehaviour::_bind_methods() {}

void BlitteringBoxBehaviour::_on_gain_control() {
    box->screens[BattleBox::State_Blittering].call("show");
    if(box->action_memory.size() > 1) box->emit_signal("exit_menu");
    box->get_blitter_text()->connect("finished_all_texts", Callable(box, "_disable"), CONNECT_ONE_SHOT);
}

void BlitteringBoxBehaviour::input(const Ref<InputEvent>& event) {
    if(!box->get_blitter_text()->get_typing()) return;

    if(event->is_action_pressed("ui_accept")) get_viewport()->set_input_as_handled();
}

void BlitteringBoxBehaviour::_on_lose_control() {
    box->screens[BattleBox::State_Blittering].call("hide");
}