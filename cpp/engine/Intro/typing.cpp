#include "typing.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/variant/callable.hpp>
#include<godot_cpp/variant/packed_string_array.hpp>
#define LIMIT 7

Typing::Typing() {
    LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz";
    margin_letters = Vector2(0, 0);
    current_pos = Vector2i(0, 0);
    shift_pressed = false;
    
    letters = Array();
    letters.resize(LIMIT);
    for (int i = 0; i < LIMIT; i++) {
        letters[i] = Array();
        Array arr = letters[i];
        arr.resize(8);
        for(int j=0; j < 8; j++) arr[j] = NodePath();
    }
}

Typing::~Typing() {}

void Typing::_bind_methods() {
    ADD_SIGNAL(MethodInfo("letter_input", PropertyInfo(Variant::STRING, "letter")));
    ADD_SIGNAL(MethodInfo("exited_letter", PropertyInfo(Variant::INT, "x")));
    ADD_SIGNAL(MethodInfo("backspace_key"));
    ADD_SIGNAL(MethodInfo("enter_key"));
    
    ClassDB::bind_method(D_METHOD("_create_letters"), &Typing::_create_letters);
    ClassDB::bind_method(D_METHOD("enable_input", "x"), &Typing::enable_input);
    ClassDB::bind_method(D_METHOD("disable_input"), &Typing::disable_input);
    ClassDB::bind_method(D_METHOD("refresh_thing", "action"), &Typing::refresh_thing, DEFVAL(Vector2i(0, 0)));
    ClassDB::bind_method(D_METHOD("_on_shift_pressed", "shift"), &Typing::_on_shift_pressed);
    
    ClassDB::bind_method(D_METHOD("set_current_pos", "pos"), &Typing::set_current_pos);
    ClassDB::bind_method(D_METHOD("get_current_pos"), &Typing::get_current_pos);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "current_pos"), "set_current_pos", "get_current_pos");
    
    ClassDB::bind_method(D_METHOD("set_margin_letters", "margin"), &Typing::set_margin_letters); 
    ClassDB::bind_method(D_METHOD("get_margin_letters"), &Typing::get_margin_letters);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "margin_letters"), "set_margin_letters", "get_margin_letters");
}

void Typing::_ready() {
    if(isEditor) return;
    shift_button = Object::cast_to<OptionSelectable>(get_node_internal("Shift"));
    call_deferred("_create_letters");
}

void Typing::set_current_pos(const Vector2i& pos) {
    current_pos.x = Math::clamp(pos.x, 0, LIMIT-1);
    
    Array column = letters[current_pos.x];
    bool found_valid = false;
    
    if (current_pos.x >= 5) {
        if (pos.y >= 3 && pos.y <= 3) {
            current_pos.y = 2;
        } else if (pos.y >= 7) {
            current_pos.y = 6; 
        } else {
            current_pos.y = Math::clamp(pos.y, 0, LIMIT);
        }
    } else {
        current_pos.y = Math::clamp(pos.y, 0, LIMIT);
    }
    
    NodePath path = column[current_pos.y];
    if (path.is_empty()) {
        for (int offset = 1; offset <= LIMIT; offset++) {
            int check_up = current_pos.y - offset;
            if (check_up >= 0) {
                NodePath up_path = column[check_up];
                if (!up_path.is_empty()) {
                    current_pos.y = check_up;
                    found_valid = true;
                    break;
                }
            }
            
            int check_down = current_pos.y + offset;
            if (check_down <= LIMIT) {
                NodePath down_path = column[check_down];
                if (!down_path.is_empty()) {
                    current_pos.y = check_down;
                    found_valid = true;
                    break;
                }
            }
        }
        
        if (!found_valid) {
            for (int x_offset = 1; x_offset < LIMIT; x_offset++) {
                int check_x = (current_pos.x + x_offset) % LIMIT;
                Array check_column = letters[check_x];
                if (!NodePath(check_column[0]).is_empty()) {
                    current_pos.x = check_x;
                    current_pos.y = 0;
                    break;
                }
            }
        }
    }
}

Vector2i Typing::get_current_pos() const {
    return current_pos;
}

void Typing::_create_letters() {
    int counter = 0;
    ResourceLoader* loader = ResourceLoader::get_singleton();
    Vector2 margin_current = Vector2(0, 0);
    
    PackedStringArray cases = LETTERS.split("\n", false);
    
    for (int case_idx=0; case_idx < cases.size(); case_idx++) {
        String case_str = cases[case_idx];
        
        // 케이스 내 각 문자에 대해 처리
        for (int letter_idx = 0; letter_idx < case_str.length(); letter_idx++) {
            String letter = case_str.substr(letter_idx, 1);
            OptionSelectable* option = memnew(OptionSelectable);
            
            Ref<Theme> theme = loader->load("res://Themes/DTMono24.tres", "Theme");
            option->set_theme(theme);
            option->add_theme_font_size_override("normal_font_size", 30);
            
            option->set_text(letter);
            option->set_scroll_active(false);
            
            add_child(option);
            option->set_size(Vector2(32, 32));
            
            int x_idx = counter % LIMIT;
            int y_idx = counter / LIMIT;
            Array arr = letters[x_idx];
            arr[y_idx] = get_path_to(option);
            
            Vector2 pos = margin_letters * Vector2(x_idx, y_idx) + margin_current;
            option->set_position(pos);
            
            counter++;
        }
        
        counter = ((counter / LIMIT) + 1) * LIMIT;
        margin_current += Vector2(0, 8);
    }
    
    refresh_thing();
}

void Typing::enable_input(int x) {
    audio_player->play("choice");
    refresh_thing();
    set_process_unhandled_input(true);
}

void Typing::disable_input() {
    set_process_unhandled_input(false);
    Array arr = letters[current_pos.x];
    Object::cast_to<OptionSelectable>(get_node_internal(arr[current_pos.y]))->set_selected(false);
    
    emit_signal("exited_letter", current_pos.x);
}

void Typing::_on_shift_pressed(bool shift) {
    shift_pressed = shift;
    shift_button->set_selected(shift);
}

void Typing::_unhandled_input(const Ref<InputEvent>& event) {
    if(!is_visible() || isEditor) return;
    
    Ref<InputEventKey> key_event = event;
    if (key_event.is_valid()) {
        // Shift 키 처리
        if (key_event->get_keycode() == Key::KEY_SHIFT) {
            _on_shift_pressed(key_event->is_pressed());
        }
        
        if (shift_pressed && key_event->is_pressed()) {
            if (key_event->get_keycode() > 64 && key_event->get_keycode() < 91) {
                int index = key_event->get_keycode() - 65;
                String letter = LETTERS.substr(index, 1);
                emit_signal("letter_input", letter);
                audio_player->play("choice");
            }
            
            if (key_event->get_keycode() == Key::KEY_BACKSPACE) {
                emit_signal("backspace_key");
                audio_player->play("choice");
            }
            
            if (key_event->get_keycode() == Key::KEY_ENTER) {
                emit_signal("enter_key");
                set_process_unhandled_input(false);
                _on_shift_pressed(false);
            }
            
            return;
        }
    }
    
    // Shift가 눌린 경우 추가 입력 처리하지 않음
    if (shift_pressed) {
        return;
    }
    
    Array arr = letters[current_pos.x];
    if (event->is_action_pressed("ui_down")) {
        if (current_pos.y >= arr.size() - 1 || 
            (current_pos.y >= arr.size() - 1)) {
            disable_input();
            return;
        }
        refresh_thing(Vector2i(0, 1)); 
    }
    
    if (event->is_action_pressed("ui_right")) {
        refresh_thing(Vector2i(1, 0)); 
    }
    
    if (event->is_action_pressed("ui_left")) {
        refresh_thing(Vector2i(-1, 0)); 
    }
    
    if (event->is_action_pressed("ui_up")) {
        refresh_thing(Vector2i(0, -1)); 
    }
    
    if (event->is_action_pressed("ui_accept")) {
        audio_player->play("choice");
        Array arr = letters[current_pos.x];
        
        String text = Object::cast_to<OptionSelectable>(get_node_internal(arr[current_pos.y]))->get_parsed_text();
        emit_signal("letter_input", text);
    }
}

void Typing::refresh_thing(const Vector2i& action) {
    Array arr = letters[current_pos.x];
    if (action != Vector2i(0, 0)) {
        audio_player->play("choice");
    }
    
    Object::cast_to<OptionSelectable>(get_node_internal(arr[current_pos.y]))->set_selected(false);
    
    current_pos += action;
    set_current_pos(current_pos);
    
    arr = letters[current_pos.x];
    Object::cast_to<OptionSelectable>(get_node_internal(arr[current_pos.y]))->set_selected(true);
}

void Typing::set_margin_letters(Vector2 value) {
    margin_letters = value;
}

Vector2 Typing::get_margin_letters() {
    return margin_letters;
}