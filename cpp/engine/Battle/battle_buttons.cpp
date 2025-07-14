#include "battle_buttons.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

BattleButtons::BattleButtons() {
    enabled = false; 
    choice = 0;
}

BattleButtons::~BattleButtons() {}

void BattleButtons::_bind_methods() {
    ADD_SIGNAL(MethodInfo("movesoul", PropertyInfo(Variant::VECTOR2, "newpos")));
    ADD_SIGNAL(MethodInfo("selectbutton", PropertyInfo(Variant::INT, "id")));
    
    ClassDB::bind_method(D_METHOD("changepos", "action"), &BattleButtons::changepos);
    ClassDB::bind_method(D_METHOD("glow_choice", "id"), &BattleButtons::glow_choice);
    ClassDB::bind_method(D_METHOD("enable"), &BattleButtons::enable);
    ClassDB::bind_method(D_METHOD("disable"), &BattleButtons::disable);
    ClassDB::bind_method(D_METHOD("reset"), &BattleButtons::reset);
    
    ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &BattleButtons::set_enabled);
    ClassDB::bind_method(D_METHOD("get_enabled"), &BattleButtons::get_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "get_enabled");
    
    ClassDB::bind_method(D_METHOD("set_choice", "choice"), &BattleButtons::set_choice);
    ClassDB::bind_method(D_METHOD("get_choice"), &BattleButtons::get_choice);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "choice"), "set_choice", "get_choice");
}

void BattleButtons::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint(); 
    if(isEditor) return;
    move_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("choice"));
    select_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("select"));
    
    buttons.clear();
    buttons.push_back(Object::cast_to<AnimatedSprite2D>(get_node_internal("Button1")));
    buttons.push_back(Object::cast_to<AnimatedSprite2D>(get_node_internal("Button2")));
    buttons.push_back(Object::cast_to<AnimatedSprite2D>(get_node_internal("Button3")));
    buttons.push_back(Object::cast_to<AnimatedSprite2D>(get_node_internal("Button4")));
}

void BattleButtons::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor || !enabled) return;
    
    if(event->is_action_pressed("ui_left")) {
        changepos(-1);
    }else if (event->is_action_pressed("ui_right")) {
        changepos(1);
    }else if(event->is_action_pressed("ui_accept")) {
        disable();
        emit_signal("selectbutton", choice);
        select_sound->play();
    }
}

void BattleButtons::changepos(int action) {
    move_sound->play();
    
    choice = ((choice + action) % 4 + 4) % 4;
    
    if (buttons.size() > choice) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[choice]);
        if (button) {
            Vector2 button_pos = button->get_global_position();
            emit_signal("movesoul", button_pos - Vector2(38, 0));
        }
    }
    
    glow_choice(choice);
}

void BattleButtons::glow_choice(int id) {
    for (int i = 0; i < buttons.size(); i++) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[i]);
        if (button) {
            button->set_frame(0);
        }
    }
    
    if (id >= 0 && id < buttons.size()) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[id]);
        if (button) {
            button->set_frame(1);
        }
    }
}

void BattleButtons::enable() {
    enabled = true;
    
    if (buttons.size() > choice) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[choice]);
        if (button) {
            Vector2 button_pos = button->get_global_position();
            emit_signal("movesoul", button_pos - Vector2(38, 0));
        }
    }
    
    glow_choice(choice);
}

void BattleButtons::disable() {
    enabled = false;
}

void BattleButtons::reset() {
    for (int i = 0; i < buttons.size(); i++) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[i]);
        if (button) {
            button->set_frame(0);
        }
    }
}

void BattleButtons::set_enabled(bool p_enabled) {
    enabled = p_enabled;
}

bool BattleButtons::get_enabled() const {
    return enabled;
}

void BattleButtons::set_choice(int p_choice) {
    choice = p_choice;
}

int BattleButtons::get_choice() const {
    return choice;
}