#include "ui_box.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

UI_Box::UI_Box() {}

UI_Box::~UI_Box() {}

void UI_Box::_bind_methods() {
    ClassDB::bind_method(D_METHOD("grow"), &UI_Box::grow);
    ClassDB::bind_method(D_METHOD("shrink"), &UI_Box::shrink);
    ClassDB::bind_method(D_METHOD("hide_box"), &UI_Box::hide_box);
}

void UI_Box::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) return;
    defsize = get_size();
    
    Color modulate_color = get_modulate();
    modulate_color.a = 0;
    set_modulate(modulate_color);
    set_visible(false);
}

Ref<Tween> UI_Box::get_tw() {
    return tw;
}

void UI_Box::grow() {
    set_visible(true);
    
    if (!tw.is_null() && tw->is_valid()) {
        tw->kill();
    }
    
    tw = create_tween();
    tw->set_trans(Tween::TRANS_EXPO);
    tw->set_ease(Tween::EASE_OUT);
    tw->set_parallel(true);
    
    Color current_modulate = get_modulate();
    Color target_modulate = current_modulate;
    target_modulate.a = 1.0;
    
    tw->tween_property(this, "modulate", target_modulate, 0.5);
}

void UI_Box::shrink() {
    if (!tw.is_null() && tw->is_valid()) {
        tw->kill();
    }
    
    tw = create_tween();
    tw->set_trans(Tween::TRANS_EXPO);
    tw->set_ease(Tween::EASE_OUT);
    tw->set_parallel(true);
    
    Color current_modulate = get_modulate();
    Color target_modulate = current_modulate;
    target_modulate.a = 0.0;
    
    tw->tween_property(this, "modulate", target_modulate, 0.5);
    
    tw->chain()->tween_callback(Callable(this, "hide_box"));
}

void UI_Box::hide_box() {
    set_visible(false);
}