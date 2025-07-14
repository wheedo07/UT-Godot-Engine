#include "enemy_overworld.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

EnemyOverworld::EnemyOverworld() {
    character = SANS;
    walk_speed = 60;
    walk_speed_mod = 1.0f;
    counter = 0;
    frame_alert = 0;
    current_index = 0;
    current_animation_name = "idle_down";
    current_anim_state = IDLE;
}

EnemyOverworld::~EnemyOverworld() {}

void EnemyOverworld::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_canmove", "value"), &EnemyOverworld::set_canmove);
    ClassDB::bind_method(D_METHOD("start_walking", "direction"), &EnemyOverworld::start_walking, DEFVAL(Vector2i(0, 0)));
    ClassDB::bind_method(D_METHOD("show_alert", "duration"), &EnemyOverworld::show_alert, DEFVAL(0.35f));
    ClassDB::bind_method(D_METHOD("set_frame", "index"), &EnemyOverworld::set_frame);
    ClassDB::bind_method(D_METHOD("play_anim", "key", "speed", "back"), &EnemyOverworld::play_anim, DEFVAL(1), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("set_walk_direction", "direction"), &EnemyOverworld::set_walk_direction);
    ClassDB::bind_method(D_METHOD("_on_area_interacted"), &EnemyOverworld::_on_area_interacted);
    
    ClassDB::bind_method(D_METHOD("set_walk_speed", "speed"), &EnemyOverworld::set_walk_speed);
    ClassDB::bind_method(D_METHOD("get_walk_speed"), &EnemyOverworld::get_walk_speed);

    ClassDB::bind_method(D_METHOD("set_dialogues", "dialogues"), &EnemyOverworld::set_dialogues);
    ClassDB::bind_method(D_METHOD("get_dialogues"), &EnemyOverworld::get_dialogues);

    ClassDB::bind_method(D_METHOD("set_character", "character"), &EnemyOverworld::set_character);
    ClassDB::bind_method(D_METHOD("get_character"), &EnemyOverworld::get_character);

    ClassDB::bind_method(D_METHOD("set_current_index", "index"), &EnemyOverworld::set_current_index);
    ClassDB::bind_method(D_METHOD("get_current_index"), &EnemyOverworld::get_current_index);

    ClassDB::bind_method(D_METHOD("set_frame_alert", "value"), &EnemyOverworld::set_frame_alert);
    ClassDB::bind_method(D_METHOD("get_frame_alert"), &EnemyOverworld::get_frame_alert);

    bind_enum(get_class_static(), "set_character", "get_character");
   
    ADD_SIGNAL(MethodInfo("character_finished"));
    ADD_SIGNAL(MethodInfo("animation_finished"));
    ADD_PROPERTY(PropertyInfo(Variant::INT, "walk_speed", PROPERTY_HINT_RANGE, "0,400,1"), 
        "set_walk_speed", "get_walk_speed");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "frame_alert"), "set_frame_alert", "get_frame_alert");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_index"), "set_current_index", "get_current_index");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dialogues", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":Dialogues")
    ,"set_dialogues", "get_dialogues");
}

void EnemyOverworld::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) return;
    text_box = ResourceLoader::get_singleton()->load("res://Overworld/text_box.tscn");
    sprite = Object::cast_to<AnimatedSprite2D>(get_node_internal("Sprite"));
    alert = Object::cast_to<AnimatedSprite2D>(get_node_internal("Alert"));
    encounter = Object::cast_to<AudioStreamPlayer>(get_node_internal("encounter"));
}

void EnemyOverworld::_physics_process(double delta) {
    Vector2 vel = Vector2(walk_direction) * walk_speed * walk_speed_mod;
    set_velocity(vel);
    
    // 이동 처리
    if (canmove) {
        move_and_collide(vel * delta);
    }
    
    bool is_moving = !vel.is_zero_approx() && canmove;
    
    if (is_moving && current_anim_state != MOVING) {
        current_anim_state = MOVING;
        if (sprite) sprite->play(current_animation_name.replace("idle_", "move_"));
    } else if (!is_moving && current_anim_state == MOVING) {
        current_anim_state = IDLE;
        if (sprite) sprite->play(current_animation_name.replace("move_", "idle_"));
    }
}

void EnemyOverworld::show_alert(float duration) {
    alert->set_frame(frame_alert);
    alert->show();
    encounter->play();
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(duration);
    timer->connect("timeout", Callable(alert, "hide"), CONNECT_ONE_SHOT);
}

void EnemyOverworld::set_canmove(bool value) {
    canmove = value;
}

void EnemyOverworld::start_walking(const Vector2i& dir) {
    
    if (dir != Vector2i()) {
        canmove = true;
        set_walk_direction(dir);
        current_anim_state = MOVING;
        sprite->play(current_animation_name);
    } else {
        canmove = false;
        current_anim_state = IDLE;
        sprite->play(current_animation_name.replace("move_", "idle_"));
    }
}

void EnemyOverworld::set_frame(int index) {
    sprite->set_animation("act");
    sprite->set_frame(index);
}

void EnemyOverworld::play_anim(String key, float speed, bool back) {
    sprite->connect("animation_finished", Callable(this, "emit_signal").bind("animation_finished"), CONNECT_ONE_SHOT);
    sprite->play(key, speed, back);
}

void EnemyOverworld::set_walk_direction(const Vector2i& direction) {
    walk_direction = direction;
    
    bool is_moving = !walk_direction.is_zero_approx();
    
    String anim_prefix = is_moving ? "move_" : "idle_";
    
    if (walk_direction.x > 0) {
        // 오른쪽
        current_animation_name = anim_prefix + "side";
        if (sprite) sprite->set_flip_h(true);
    } else if (walk_direction.x < 0) {
        // 왼쪽
        current_animation_name = anim_prefix + "side";
        if (sprite) sprite->set_flip_h(false);
    }
    
    if (walk_direction.y > 0) {
        // 아래
        current_animation_name = anim_prefix + "down";
        if (sprite) sprite->set_flip_h(false);
    } else if (walk_direction.y < 0) {
        // 위
        current_animation_name = anim_prefix + "up";
        if (sprite) sprite->set_flip_h(false);
    }
    
    if (is_moving && current_anim_state != MOVING) {
        current_anim_state = MOVING;
        if (sprite) sprite->play(current_animation_name);
    } else if (!is_moving && current_anim_state == MOVING) {
        current_anim_state = IDLE;
        if (sprite) sprite->play(current_animation_name);
    }
}

void EnemyOverworld::_on_area_interacted() {
    if (dialogues.size() == 0) {
        printf("EnemyOverworld: 대화 내용이 없음\n");
        return;
    }
    
    Node* instance = text_box->instantiate();
    if (!instance) {
        ERR_PRINT("EnemyOverworld: 에러(1)");
        return;
    }
    add_child(instance);
    
    TextBox* ct = Object::cast_to<TextBox>(instance);
    ct->connect("dialogue_finished", Callable(this, "emit_signal").bind("character_finished"), CONNECT_ONE_SHOT);
    if(ct) {
        ct->character(false, character, dialogues[current_index]);
    } else {
        ERR_PRINT("EnemyOverworld: 에러(2)");
    }
}

void EnemyOverworld::set_dialogues(const TypedArray<Dialogues>& p_dialogues) {
    dialogues = p_dialogues;
}

TypedArray<Dialogues> EnemyOverworld::get_dialogues() const {
    return dialogues;
}

void EnemyOverworld::set_walk_speed(int p_speed) {
    walk_speed = p_speed;
}

int EnemyOverworld::get_walk_speed() const {
    return walk_speed;
}

void EnemyOverworld::set_character(Character p_character) {
    character = p_character;
}

Character EnemyOverworld::get_character() const {
    return character;
}

void EnemyOverworld::set_current_index(int index) {
    current_index = index;
}

int EnemyOverworld::get_current_index() {
    return current_index;
}

void EnemyOverworld::set_frame_alert(int value) {
    frame_alert = value;
}

int EnemyOverworld::get_frame_alert() {
    return frame_alert;
}