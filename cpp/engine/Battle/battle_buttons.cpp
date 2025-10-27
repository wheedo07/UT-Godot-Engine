#include "battle_buttons.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

BattleButtons::BattleButtons() {
    enabled = false; 
    choice = 0;

    enableds.resize(4);
    for(int i=0; i < 4; i++) {
        enableds[i] = true;
    }
}

BattleButtons::~BattleButtons() {}

void BattleButtons::_bind_methods() {
    ADD_SIGNAL(MethodInfo("animation_finished"));
    ADD_SIGNAL(MethodInfo("movesoul", PropertyInfo(Variant::VECTOR2, "newpos")));
    ADD_SIGNAL(MethodInfo("selectbutton", PropertyInfo(Variant::INT, "id")));
    ClassDB::bind_method(D_METHOD("_enable"), &BattleButtons::_enable);
    ClassDB::bind_method(D_METHOD("_reset"), &BattleButtons::_reset);

    ClassDB::bind_method(D_METHOD("set_button_enabled", "enabled"), &BattleButtons::set_button_enabled);
    ClassDB::bind_method(D_METHOD("get_button_enabled"), &BattleButtons::get_button_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "enableds", PROPERTY_HINT_ARRAY_TYPE, "bool"), "set_button_enabled", "get_button_enabled");
    
    ClassDB::bind_method(D_METHOD("glow_choice", "id"), &BattleButtons::glow_choice);
    ClassDB::bind_method(D_METHOD("play", "id", "anim", "custom_speed", "from_end"), &BattleButtons::play, DEFVAL(1.0), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("hide_button", "id"), &BattleButtons::hide_button);
    ClassDB::bind_method(D_METHOD("show_button", "id"), &BattleButtons::show_button);
}

void BattleButtons::_ready() {
    move_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("choice"));
    select_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("select"));
    
    buttons.clear();
    buttons.push_back(Object::cast_to<AnimatedSprite2D>(get_node_internal("Button1")));
    buttons.push_back(Object::cast_to<AnimatedSprite2D>(get_node_internal("Button2")));
    buttons.push_back(Object::cast_to<AnimatedSprite2D>(get_node_internal("Button3")));
    buttons.push_back(Object::cast_to<AnimatedSprite2D>(get_node_internal("Button4")));
}

void BattleButtons::_unhandled_input(const Ref<InputEvent>& event) {
    if(!enabled) return;
    
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

void BattleButtons::set_button(Ref<ButtonSet> button_set) {
    current_button_set = button_set;
    if(button_set.is_null()) return;
    for(int i=0; i < buttons.size(); i++) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[i]);
        switch(i) {
            case 0:
                if(!button_set->get_fight_button().is_null())
                    button->set_sprite_frames(button_set->get_fight_button());
                break;
            case 1:
                if(!button_set->get_act_button().is_null())
                    button->set_sprite_frames(button_set->get_act_button());
                break;
            case 2:
                if(!button_set->get_item_button().is_null())
                    button->set_sprite_frames(button_set->get_item_button());
                break;
            case 3:
                if(!button_set->get_mercy_button().is_null())
                    button->set_sprite_frames(button_set->get_mercy_button());
                break;
        }
    }
}

void BattleButtons::changepos(int action) {
    int new_choice = choice;
    for(int i=0; i < 4 && (new_choice < buttons.size() && !bool(enableds[new_choice])); i++) {
        new_choice += action;
        if(new_choice < 0) new_choice = buttons.size() -1;
        if(new_choice >= buttons.size()) new_choice = 0;
    }
    
    if(buttons.size() > new_choice) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[new_choice]);
        bool is_enabled = enableds[new_choice];
        if(is_enabled) {
            choice = new_choice;
            Vector2 button_pos = button->get_global_position();
            Vector2 offset = current_button_set.is_valid() ? current_button_set->get_soul_offset() : Vector2(38, 0);
            emit_signal("movesoul", button_pos - offset);
            move_sound->play();
        }
    }
    
    glow_choice(choice);
}

void BattleButtons::glow_choice(int id) {
    bool id_enabled = (id >=0 && id < enableds.size()) ? bool(enableds[id]) : true;
    if(!id_enabled) return;

    bool active_scale = current_button_set.is_valid() && current_button_set->is_active_scale();
    if(active_scale) {
        if(tween.is_valid()) tween->kill();
        tween = create_tween()->set_parallel();
    }

    for(int i=0; i < buttons.size(); i++) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[i]);
        button->set_frame(0);

        if(active_scale) {
            if(i == id) {
                tween->tween_property(button, "scale", Vector2(1.2, 1.2), 0.2);
                button->set_modulate(Color(1,1,1,1));
            }else {
                tween->tween_property(button, "scale", Vector2(1, 1), 0.2);
                button->set_modulate(Color(1,1,1, 0.5));
            }
        }else {
            button->set_modulate(Color(1,1,1,1));
            button->set_scale(Vector2(1,1));
        }
    }
    
    if(id >= 0 && id < buttons.size()) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[id]);
        button->set_frame(1);
    }
}

void BattleButtons::play(int id, String anim, float custom_speed, bool from_end) {
    if(id >= 0 && id < buttons.size()) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[id]);
        button->play(anim, custom_speed, from_end);
        button->connect("animation_finished", Callable(this, "emit_signal").bind("animation_finished"));
    }else ERR_PRINT("에러: BattleButtons::play - 잘못된 버튼 ID");
}

void BattleButtons::hide_button(int id) {
    if(id >= 0 && id < buttons.size()) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[id]);
        button->hide();
    }else ERR_PRINT("에러: BattleButtons::hide_button - 잘못된 버튼 ID");
}

void BattleButtons::show_button(int id) {
    if(id >= 0 && id < buttons.size()) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[id]);
        button->show();
    }else ERR_PRINT("에러: BattleButtons::show_button - 잘못된 버튼 ID");
}

void BattleButtons::_enable() {
    enabled = true;
    
    if(buttons.size() > choice) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[choice]);
        if (button) {
            Vector2 button_pos = button->get_global_position();
            Vector2 offset = current_button_set.is_valid() ? current_button_set->get_soul_offset() : Vector2(38, 0);
            emit_signal("movesoul", button_pos - offset);
        }
    }
    
    glow_choice(choice);
}

void BattleButtons::disable() {
    enabled = false;
}

void BattleButtons::_reset() {
    for (int i = 0; i < buttons.size(); i++) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[i]);
        if (button) {
            button->set_frame(0);
        }
    }

    for(int i=0; i < buttons.size(); i++) {
        AnimatedSprite2D* button = Object::cast_to<AnimatedSprite2D>(buttons[i]);
        button->set_modulate(Color(1,1,1,1));
        button->set_scale(Vector2(1,1));
    }
}

void BattleButtons::set_button_enabled(Array enabled) {
    enableds = enabled;
}

Array BattleButtons::get_button_enabled() const {
    return enableds;
}