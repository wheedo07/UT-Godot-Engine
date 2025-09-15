#include "death_screen.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/input_event_action.hpp>

DeathScreen::DeathScreen() {
    pressed = false;
    isCustom = false;
}

DeathScreen::~DeathScreen() {}

void DeathScreen::_bind_methods() {
    ClassDB::bind_method(D_METHOD("done"), &DeathScreen::done);
    ClassDB::bind_method(D_METHOD("get_gameOver_text"), &DeathScreen::get_gameOver_text);

    ClassDB::bind_method(D_METHOD("_setup_death_animation"), &DeathScreen::_setup_death_animation);

    ClassDB::bind_method(D_METHOD("set_is_custom", "value"), &DeathScreen::set_is_custom);
    ClassDB::bind_method(D_METHOD("get_is_custom"), &DeathScreen::get_is_custom);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_custom"), "set_is_custom", "get_is_custom");

    ClassDB::bind_method(D_METHOD("set_soul_path", "value"), &DeathScreen::set_soul_path);
    ClassDB::bind_method(D_METHOD("get_soul_path"), &DeathScreen::get_soul_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "soul_path"), "set_soul_path", "get_soul_path");
}

void DeathScreen::_ready() {
    if(isEditor) return;
    death_soul = Object::cast_to<DeathSoul>(get_node_internal(soul_path));
    if(!isCustom) {
        camera = global->get_scene_container()->get_camera();
        fade_text = Object::cast_to<RichTextLabel>(get_node_internal("Fade/RichTextLabel"));
        text_typer = Object::cast_to<GenericTextTyper>(get_node_internal("Fade/GenericTextTyper"));
        music_player = Object::cast_to<AudioStreamPlayer>(get_node_internal("mus"));
    }

    call_deferred("_setup_death_animation");
}

void DeathScreen::_setup_death_animation() {
    global->get_Music()->stop();
    
    death_soul->set_position(global->get_player_position());
    death_soul->set_modulate(global->game_over["color"]);
    if(isCustom) return;

    audio_player->play("hurt");
    camera->add_shake(0.13f);

    tween = create_tween()->set_trans(Tween::TRANS_QUAD)->set_parallel();
    tween->tween_interval(0.4);
    tween->chain()->tween_property(camera, "zoom", Vector2(3.0f, 3.0f), 0.4)->set_ease(Tween::EASE_IN)->set_delay(0.4);
    tween->tween_property(camera, "position", death_soul->get_position(), 0.4)->set_ease(Tween::EASE_OUT);
    tween->chain()->tween_callback(Callable(death_soul, "die"))->set_delay(0.3);
    tween->tween_property(fade_text, "modulate:a", 1, 0.5)->set_ease(Tween::EASE_OUT)->set_delay(3.0);
    tween->tween_callback(Callable(music_player, "play"))->set_delay(2.8);
    tween->tween_property(music_player, "volume_db", 0, 0.4)->set_delay(2.8);
    tween->connect("finished", Callable(text_typer, "type_text").bind(get_gameOver_text()));
}

void DeathScreen::_unhandled_input(const Ref<InputEvent>& event) {
    if(event->is_action_pressed("ui_accept") && !pressed) {
        done();
    }
}

void DeathScreen::done() {
    if(tween.is_valid() && tween->is_valid()) {
        tween->kill();
    }
    pressed = true;
    
    global->set_just_died(true);
    if(global->get_first()) {
        global->call_deferred("resetgame");
        global->get_scene_container()->call_deferred("reload_camera");
        global->get_scene_container()->change_scene_to_file("res://Intro/intro.tscn");
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

void DeathScreen::set_is_custom(bool value) {
    isCustom = value;
}

bool DeathScreen::get_is_custom() {
    return isCustom;
}

void DeathScreen::set_soul_path(NodePath value) {
    soul_path = value;
}

NodePath DeathScreen::get_soul_path() {
    return soul_path;
}