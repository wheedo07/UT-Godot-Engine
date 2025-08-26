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
    int new_x = pos.x;
    int new_y = pos.y;

    if (new_x < 0) new_x = LIMIT - 1;
    if (new_x >= LIMIT) new_x = 0;

    Array arr = letters[new_x];
    if (arr.is_empty()) return;

    if (new_y < 0) new_y = arr.size() - 1;
    if (new_y >= arr.size()) new_y = 0;

    int dx = (pos.x - current_pos.x);
    int dy = (pos.y - current_pos.y);
    dx = dx == 0 ? 0 : (dx > 0 ? 1 : -1);
    dy = dy == 0 ? 0 : (dy > 0 ? 1 : -1);

    int safety = 0;
    while (safety < 100) {
        NodePath path = arr[new_y];
        if (!path.is_empty()) break;

        new_x += dx;
        new_y += dy;

        if (new_x < 0) new_x = LIMIT - 1;
        if (new_x >= LIMIT) new_x = 0;

        arr = letters[new_x];
        if (arr.is_empty()) return;
        if (new_y < 0) new_y = arr.size() - 1;
        if (new_y >= arr.size()) new_y = 0;

        safety++;
    }
    current_pos = Vector2i(new_x, new_y);
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
    
    set_current_pos(current_pos + action);
    
    arr = letters[current_pos.x];
    Object::cast_to<OptionSelectable>(get_node_internal(arr[current_pos.y]))->set_selected(true);
}

void Typing::set_margin_letters(Vector2 value) {
    margin_letters = value;
}

Vector2 Typing::get_margin_letters() {
    return margin_letters;
}