#include "death_screen.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/input_event_action.hpp>

DeathScreen::DeathScreen() {
    death_soul = nullptr;
}

DeathScreen::~DeathScreen() {}

void DeathScreen::_bind_methods() {
    GDVIRTUAL_BIND(ready);
    ClassDB::bind_method(D_METHOD("_setup_death_animation"), &DeathScreen::_setup_death_animation);

    ClassDB::bind_method(D_METHOD("end_death"), &DeathScreen::end_death);
    ClassDB::bind_method(D_METHOD("get_gameOver_text"), &DeathScreen::get_gameOver_text);

    ClassDB::bind_method(D_METHOD("set_property", "value"), &DeathScreen::set_property);
    ClassDB::bind_method(D_METHOD("get_camera_fx"), &DeathScreen::get_camera_fx);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "CameraFx", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_camera_fx");

    ClassDB::bind_method(D_METHOD("set_death_soul", "p_death_soul"), &DeathScreen::set_death_soul);
    ClassDB::bind_method(D_METHOD("get_death_soul"), &DeathScreen::get_death_soul);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "death_soul", PROPERTY_HINT_NODE_TYPE, "DeathSoul"), "set_death_soul", "get_death_soul");
}

void DeathScreen::_ready() {
    global->get_scene_container()->get_camera()->kill();
    call_deferred("_setup_death_animation");
}

void DeathScreen::ready() {}

void DeathScreen::_setup_death_animation() {
    global->get_Music()->stop();
    
    if(death_soul) {
        death_soul->set_position(global->get_player_position());
        death_soul->set_modulate(global->game_over["color"]);
        if(global->game_over["overworld"]) {
            death_soul->set_scale(Vector2(0.35, 0.35));
        }else {
            death_soul->set_scale(Vector2(1, 1));
        }
    }
    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    }else {
        ready();
    }
}

void DeathScreen::end_death() {
    global->set_just_died(true);
    if(global->get_first()) {
        global->resetgame();
        global->get_scene_container()->reload_camera();
        global->get_scene_container()->change_scene_to_file("res://Core/Startup/intro.tscn");
        return;
    }
    
    global->load_game();
    Variant room = global->get_overworld_data()["room"];
    String room_path;
    
    if(room.get_type() == Variant::STRING) {
        room_path = room;
    }else {
        room_path = scene_changer->get_default_scene();
    }
    
    global->get_scene_container()->reload_camera();
    scene_changer->enter_room_path(room_path, Dictionary());
}

PackedStringArray DeathScreen::get_gameOver_text() {
    return global->game_over["text"];
}

void DeathScreen::set_property(Variant value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

void DeathScreen::set_death_soul(DeathSoul *p_death_soul) {
    death_soul = p_death_soul;
}

DeathSoul *DeathScreen::get_death_soul() const {
    return death_soul;
}

CameraFx *DeathScreen::get_camera_fx() const {
    return global->get_scene_container()->get_camera();
}