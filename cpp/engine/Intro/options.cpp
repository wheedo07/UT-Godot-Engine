#include "options.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/variant/callable.hpp>

Options::Options() {
    current_pos = 0;
}

Options::~Options() {}

void Options::_bind_methods() {
    ADD_SIGNAL(MethodInfo("confirm_name"));
    ADD_SIGNAL(MethodInfo("backspace"));
    ADD_SIGNAL(MethodInfo("enter_typing", PropertyInfo(Variant::INT, "x")));
    
    ClassDB::bind_method(D_METHOD("enable", "x"), &Options::enable);
    ClassDB::bind_method(D_METHOD("disable"), &Options::disable);
    ClassDB::bind_method(D_METHOD("refresh_thing", "action"), &Options::refresh_thing, DEFVAL(0));
    
    ClassDB::bind_method(D_METHOD("set_current_pos", "pos"), &Options::set_current_pos);
    ClassDB::bind_method(D_METHOD("get_current_pos"), &Options::get_current_pos);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_pos"), "set_current_pos", "get_current_pos");
}

void Options::_ready() {
    if(isEditor) return;
    option_nodes = get_children();
    set_process_unhandled_input(false);
}

void Options::set_current_pos(int pos) {
    int modulo = 3;
    current_pos = ((pos % modulo) + modulo) % modulo;
}

int Options::get_current_pos() const {
    return current_pos;
}

void Options::enable(int x) {
    audio_player->play("choice");
    current_pos = 0;
    
    int option_pos = 0;
    if (x < 2) {
        option_pos = 0;
    } else if (x < 5) {
        option_pos = 1;
    } else {
        option_pos = 2;
    }
    
    refresh_thing(option_pos);
    set_process_unhandled_input(true);
    refresh_thing();
}

void Options::disable() {
    Node* option = Object::cast_to<Node>(option_nodes[current_pos]);
    if (option) {
        option->set("selected", false);
    }
    
    emit_signal("enter_typing", current_pos);
    set_process_unhandled_input(false);
}

void Options::_unhandled_input(const Ref<InputEvent>& event) {
    if (!is_visible() || isEditor) return;
    
    if (event->is_action_pressed("ui_up") || event->is_action_pressed("ui_down")) {
        disable();
        return;
    }
    
    if (event->is_action_pressed("ui_left")) {
        refresh_thing(-1);
        return;
    }
    
    if (event->is_action_pressed("ui_right")) {
        refresh_thing(1);
        return;
    }
    
    if (event->is_action_pressed("ui_accept")) {
        switch (current_pos) {
            case 0: {
                Node* parent = get_node_internal("../");
                Ref<Tween> tween = create_tween();
                tween->tween_property(parent, "modulate:a", 0.0, 0.8)->set_trans(Tween::TRANS_SINE);
                tween->connect("finished", Callable(global->get_scene_container(), "change_scene_to_file").bind("res://Intro/intro.tscn"));
                break;
            }
            case 1:
                emit_signal("backspace");
                break;
            case 2:
                emit_signal("confirm_name");
                break;
        }
    }
}

void Options::refresh_thing(int action) {
    audio_player->play("choice");
    
    Object::cast_to<Node>(option_nodes[current_pos])->set("selected", false);
    
    current_pos += action;
    set_current_pos(current_pos);
    
    Object::cast_to<Node>(option_nodes[current_pos])->set("selected", true);
}