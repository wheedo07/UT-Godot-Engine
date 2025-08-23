#include "player_overworld.h"
#include "../ui.h"
#include "env.h"
#include<godot_cpp/core/math.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/classes/input.hpp>
#include<godot_cpp/classes/resource_loader.hpp>

PlayerOverworld::PlayerOverworld() {
    current_animation_name = "idle_down";
    current_anim_state = IDLE;
    frame_alert = 0;
}

PlayerOverworld::~PlayerOverworld() {}

void PlayerOverworld::_bind_methods() {
    ADD_SIGNAL(MethodInfo("animation_finished"));
    
    ClassDB::bind_method(D_METHOD("set_canmove", "value"), &PlayerOverworld::set_canmove);
    ClassDB::bind_method(D_METHOD("start_walking", "direction"), &PlayerOverworld::start_walking, DEFVAL(Vector2i()));
    ClassDB::bind_method(D_METHOD("show_alert", "duration"), &PlayerOverworld::show_alert, DEFVAL(0.35f));
    ClassDB::bind_method(D_METHOD("set_frame", "index"), &PlayerOverworld::set_frame);
    ClassDB::bind_method(D_METHOD("play_anim", "key", "speed", "back"), &PlayerOverworld::play_anim, DEFVAL(1), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("refresh_direction"), &PlayerOverworld::refresh_direction);
    ClassDB::bind_method(D_METHOD("force_direction", "dir"), &PlayerOverworld::force_direction);
    ClassDB::bind_method(D_METHOD("set_direction"), &PlayerOverworld::set_direction);
    ClassDB::bind_method(D_METHOD("_step"), &PlayerOverworld::_step);
    ClassDB::bind_method(D_METHOD("_enter_random_encounter"), &PlayerOverworld::_enter_random_encounter);
    ClassDB::bind_method(D_METHOD("on_encounter_timer_completed"), &PlayerOverworld::on_encounter_timer_completed);
    ClassDB::bind_method(D_METHOD("on_hide_timer_completed"), &PlayerOverworld::on_hide_timer_completed);
    
    ClassDB::bind_method(D_METHOD("set_walk_speed", "speed"), &PlayerOverworld::set_walk_speed);
    ClassDB::bind_method(D_METHOD("get_walk_speed"), &PlayerOverworld::get_walk_speed);
    
    ClassDB::bind_method(D_METHOD("set_step_counter_needed", "steps"), &PlayerOverworld::set_step_counter_needed);
    ClassDB::bind_method(D_METHOD("get_step_counter_needed"), &PlayerOverworld::get_step_counter_needed);
    
    ClassDB::bind_method(D_METHOD("set_step_increase", "increase"), &PlayerOverworld::set_step_increase);
    ClassDB::bind_method(D_METHOD("get_step_increase"), &PlayerOverworld::get_step_increase);
    
    ClassDB::bind_method(D_METHOD("set_encounters", "encounters"), &PlayerOverworld::set_encounters);
    ClassDB::bind_method(D_METHOD("get_encounters"), &PlayerOverworld::get_encounters);
    
    ClassDB::bind_method(D_METHOD("set_interact_posx", "positions"), &PlayerOverworld::set_interact_posx);
    ClassDB::bind_method(D_METHOD("get_interact_posx"), &PlayerOverworld::get_interact_posx);
    
    ClassDB::bind_method(D_METHOD("set_interact_posy", "positions"), &PlayerOverworld::set_interact_posy);
    ClassDB::bind_method(D_METHOD("get_interact_posy"), &PlayerOverworld::get_interact_posy);
    
    ClassDB::bind_method(D_METHOD("set_frame_alert", "value"), &PlayerOverworld::set_frame_alert);
    ClassDB::bind_method(D_METHOD("get_frame_alert"), &PlayerOverworld::get_frame_alert);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "walk_speed", PROPERTY_HINT_RANGE, "0,500,0.1"), "set_walk_speed", "get_walk_speed");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "frame_alert"), "set_frame_alert", "get_frame_alert");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "StepCounterNeeded"), "set_step_counter_needed", "get_step_counter_needed");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "STEP_INCREASE"), "set_step_increase", "get_step_increase");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "Encounters", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":Encounter")
    ,"set_encounters", "get_encounters");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "InteractPosx"), "set_interact_posx", "get_interact_posx");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "InteractPosy"), "set_interact_posy", "get_interact_posy");
}

void PlayerOverworld::_ready() {
    if(isEditor) return;
    sprite = Object::cast_to<AnimatedSprite2D>(get_node_internal("Sprite"));
    interacter = Object::cast_to<Area2D>(get_node_internal("Interacter"));
    alert_sprite = Object::cast_to<AnimatedSprite2D>(get_node_internal("Alert"));
    encounter_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("encounter"));
    
    player_menu = ResourceLoader::get_singleton()->load("res://Overworld/ui.tscn");
    alert_sprite->set_visible(false);
}

void PlayerOverworld::_physics_process(double delta) {
    if (!global || isEditor) return;
    
    if (forced_walking) {
        set_direction();
        bool moving_old = moving;
        if (moving && !moving_old) {
            sprite->play(current_animation_name);
        }
        
        Vector2 force_vel = Vector2(forced_direction) * walk_speed * walk_speed_modifier;
        set_velocity(force_vel);
        move_and_slide();
    }else if (global->get_player_can_move() && !global->get_player_in_menu() && global->get_player_move()) {
        bool moving_old = moving;
        refresh_direction();
        if (moving && !moving_old) {
            sprite->play(current_animation_name);
        }
        
        Vector2 vel;
        vel.x = direction.x * walk_speed * walk_speed_modifier;
        vel.y = direction.y * walk_speed * walk_speed_modifier;
        set_velocity(vel);
        move_and_slide();
    }
    
    global->set_player_position(get_global_transform_with_canvas().get_origin() + Vector2(0, -17));

    bool is_moving = (forced_walking && moving) || (!direction.is_zero_approx() && global->get_player_can_move() && !global->get_player_in_menu() && global->get_player_move());
                      
    if (is_moving && current_anim_state != MOVING) {
        current_anim_state = MOVING;
        sprite->play(current_animation_name);
    } else if (!is_moving && current_anim_state == MOVING) {
        current_anim_state = IDLE;
        sprite->play(current_animation_name.replace("move_", "idle_"));
    }
    
    interactables.clear();
    TypedArray<Area2D> areas = interacter->get_overlapping_areas();
    for (int i = 0; i < areas.size(); i++) {
        Area2D* area = Object::cast_to<Area2D>(areas[i]);
        if(area->is_in_group("interactable")) {
            interactables.push_back(area);
        }
    }
}

void PlayerOverworld::set_canmove(bool value) {
    moving = value;
}

void PlayerOverworld::refresh_direction() {
    Input* input = Input::get_singleton();
    float right = input->get_action_raw_strength("ui_right");
    float left = input->get_action_raw_strength("ui_left");
    float down = input->get_action_raw_strength("ui_down");
    float up = input->get_action_raw_strength("ui_up");
    
    direction = Vector2(right - left, down - up);
    set_direction();
}

void PlayerOverworld::force_direction(const Vector2& dir) {
    direction = dir.normalized();
    set_direction();
}

void PlayerOverworld::set_direction() {
    if (!interacter) return;
    
    int dir_x = direction.x;
    int dir_y = direction.y;
    
    if (interact_posx.has(dir_x)) {
        interacter->set_position(interact_posx[dir_x]);
    }
    
    if (interact_posy.has(dir_y)) {
        interacter->set_position(interact_posy[dir_y]);
    }
    
    if (!direction.is_zero_approx() && !moving) {
        moving = true;
    }
    
    if (!direction.is_zero_approx()) {
        last_dir = direction;
    }
    
    String anim_prefix = moving ? "move_" : "idle_";
    
    if (last_dir.x > 0.5f) {
        current_animation_name = anim_prefix + "side";
        if (sprite) sprite->set_flip_h(true);
    }
    else if (last_dir.x < -0.5f) {
        current_animation_name = anim_prefix + "side";
        if (sprite) sprite->set_flip_h(false);
    }
    
    if (last_dir.y > 0.5f) {
        current_animation_name = anim_prefix + "down";
        if (sprite) sprite->set_flip_h(false);
    }
    else if (last_dir.y < -0.5f) {
        current_animation_name = anim_prefix + "up";
        if (sprite) sprite->set_flip_h(false);
    }
    
    if (!direction.is_zero_approx() && (moving || forced_walking)) {
        current_anim_state = MOVING;
        sprite->play(current_animation_name);
    } else if (direction.is_zero_approx() && current_anim_state == MOVING) {
        current_anim_state = IDLE;
        sprite->play(current_animation_name.replace("move_", "idle_"));
    }
    
    if (direction.is_zero_approx() || ((!global->get_player_can_move() || global->get_player_in_menu()) && !forced_walking)) {
        moving = false;
    }
}

void PlayerOverworld::start_walking(const Vector2i& dir) {
    forced_walking = true;
    forced_direction = dir;
    
    if (dir != Vector2i()) {
        moving = true;
        force_direction(dir);
        current_anim_state = MOVING;
        sprite->play(current_animation_name);
    } else {
        moving = false;
        forced_walking = false;
        current_anim_state = IDLE;
        sprite->play(current_animation_name.replace("move_", "idle_"));
    }
}

void PlayerOverworld::show_alert(float duration) {
    alert_sprite->set_frame(frame_alert);
    alert_sprite->show();
    encounter_sound->play();
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(duration);
    timer->connect("timeout", Callable(alert_sprite, "hide"), CONNECT_ONE_SHOT);
}

void PlayerOverworld::set_frame(int index) {
    sprite->set_animation("act");
    sprite->set_frame(index);
}

void PlayerOverworld::play_anim(String key, float speed, bool back) {
    sprite->connect("animation_finished", Callable(this, "emit_signal").bind("animation_finished"), CONNECT_ONE_SHOT);
    sprite->play(key, speed, back);
}

void PlayerOverworld::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    
    if (global->get_player_can_move() && !global->get_player_in_menu() && !forced_walking) {
        if (event->is_action("ui_left") || event->is_action("ui_right") || 
            event->is_action("ui_up") || event->is_action("ui_down")) {
            _step();
        }
        
        if (event->is_action_pressed("ui_accept")) {
            get_viewport()->set_input_as_handled();
            
            for (int i = 0; i < interactables.size(); i++) {
                interactables[i].call("emit_signal", "interacted");
            }
        }
        
        if (event->is_action_pressed("ui_menu") && !global->get_player_text_box() && !global->get_player_in_menu()) {
            if (!player_menu.is_null()) {
                UI* menu = Object::cast_to<UI>(player_menu->instantiate());
                bool is = global->get_flag("isGenocide");
                add_child(menu);
                menu->set_is_kill(is);
            }
        }
    }else {
        moving = false;
    }
}

void PlayerOverworld::_step() {
    step_count++;
    
    if (step_count > step_counter_needed && UtilityFunctions::randf() > 0.11f) {
        step_count = 0;
        step_counter_needed += step_increase;
        _enter_random_encounter();
    }
}

void PlayerOverworld::_enter_random_encounter() {
    if (encounters.size() == 0) return;
   
    current_anim_state = IDLE;
    sprite->play(current_animation_name.replace("move_", "idle_"));

    global->set_player_can_move(false);
    alert_sprite->set_frame(frame_alert);
    alert_sprite->set_visible(true);
    encounter_sound->play();
    
    waiting_for_encounter_timer = true;
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.35);
    timer->connect("timeout", Callable(this, "on_encounter_timer_completed"), CONNECT_ONE_SHOT);
}

void PlayerOverworld::on_encounter_timer_completed() {
    if (!waiting_for_encounter_timer) return;
    waiting_for_encounter_timer = false;
    
    int random_idx = UtilityFunctions::randf_range(0, encounters.size());
    Ref<Encounter> selected_encounter = encounters[random_idx];
    
    scene_changer->load_battle(selected_encounter);
    
    waiting_for_hide_timer = true;
    Ref<SceneTreeTimer> hide_timer = get_tree()->create_timer(0.6);
    hide_timer->connect("timeout", Callable(this, "on_hide_timer_completed"), CONNECT_ONE_SHOT);
}

void PlayerOverworld::on_hide_timer_completed() {
    if (!waiting_for_hide_timer) return;
    waiting_for_hide_timer = false;
    
    alert_sprite->set_visible(false);
}

void PlayerOverworld::set_walk_speed(float p_walk_speed) {
    walk_speed = p_walk_speed;
}

float PlayerOverworld::get_walk_speed() const {
    return walk_speed;
}

void PlayerOverworld::set_step_counter_needed(int p_steps) {
    step_counter_needed = p_steps;
}

int PlayerOverworld::get_step_counter_needed() const {
    return step_counter_needed;
}

void PlayerOverworld::set_step_increase(int p_increase) {
    step_increase = p_increase;
}

int PlayerOverworld::get_step_increase() const {
    return step_increase;
}

void PlayerOverworld::set_encounters(const TypedArray<Encounter>& p_encounters) {
    encounters = p_encounters;
}

TypedArray<Encounter> PlayerOverworld::get_encounters() const {
    return encounters;
}

void PlayerOverworld::set_interact_posx(const Dictionary& p_positions) {
    interact_posx = p_positions;
}

Dictionary PlayerOverworld::get_interact_posx() const {
    return interact_posx;
}

void PlayerOverworld::set_interact_posy(const Dictionary& p_positions) {
    interact_posy = p_positions;
}

Dictionary PlayerOverworld::get_interact_posy() const {
    return interact_posy;
}

void PlayerOverworld::set_frame_alert(int value) {
    frame_alert = value;
}

int PlayerOverworld::get_frame_alert() {
    return frame_alert;
}