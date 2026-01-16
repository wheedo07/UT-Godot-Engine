#include "overworld_character.h"
#include "env.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

OverworldCharacterNode::OverworldCharacterNode() {
    character = "";
    walk_speed = 60;
    walk_speed_mod = 1.0f;
    counter = 0;
    frame_alert = 0;
    current_index = 0;
    current_animation_name = "idle_down";
    current_anim_state = IDLE;
    canmove = true;

    sprite = nullptr;
    area_interact = nullptr;
}

OverworldCharacterNode::~OverworldCharacterNode() {}

void OverworldCharacterNode::_bind_methods() {
    ADD_SIGNAL(MethodInfo("character_finished"));
    ADD_SIGNAL(MethodInfo("animation_finished"));
    ClassDB::bind_method(D_METHOD("start_walking", "direction"), &OverworldCharacterNode::start_walking, DEFVAL(Vector2i(0, 0)));
    ClassDB::bind_method(D_METHOD("force_direction", "dir"), &OverworldCharacterNode::force_direction);
    ClassDB::bind_method(D_METHOD("show_alert", "duration"), &OverworldCharacterNode::show_alert, DEFVAL(0.35f));
    ClassDB::bind_method(D_METHOD("set_frame", "index"), &OverworldCharacterNode::set_frame);
    ClassDB::bind_method(D_METHOD("play_anim", "key", "speed", "from_end"), &OverworldCharacterNode::play_anim, DEFVAL(1), DEFVAL(false));

    ClassDB::bind_method(D_METHOD("_set_canmove", "value"), &OverworldCharacterNode::_set_canmove);
    ClassDB::bind_method(D_METHOD("_on_area_interacted"), &OverworldCharacterNode::_on_area_interacted);
    
    ClassDB::bind_method(D_METHOD("set_walk_speed", "speed"), &OverworldCharacterNode::set_walk_speed);
    ClassDB::bind_method(D_METHOD("get_walk_speed"), &OverworldCharacterNode::get_walk_speed);

    ClassDB::bind_method(D_METHOD("set_dialogues", "dialogues"), &OverworldCharacterNode::set_dialogues);
    ClassDB::bind_method(D_METHOD("get_dialogues"), &OverworldCharacterNode::get_dialogues);

    ClassDB::bind_method(D_METHOD("set_character", "character"), &OverworldCharacterNode::set_character);
    ClassDB::bind_method(D_METHOD("get_character"), &OverworldCharacterNode::get_character);

    ClassDB::bind_method(D_METHOD("set_current_index", "index"), &OverworldCharacterNode::set_current_index);
    ClassDB::bind_method(D_METHOD("get_current_index"), &OverworldCharacterNode::get_current_index);

    ClassDB::bind_method(D_METHOD("set_frame_alert", "value"), &OverworldCharacterNode::set_frame_alert);
    ClassDB::bind_method(D_METHOD("get_frame_alert"), &OverworldCharacterNode::get_frame_alert);

    ClassDB::bind_method(D_METHOD("set_sprite", "sprite"), &OverworldCharacterNode::set_sprite);
    ClassDB::bind_method(D_METHOD("get_sprite"), &OverworldCharacterNode::get_sprite);

    ClassDB::bind_method(D_METHOD("set_area_interact", "area_interact"), &OverworldCharacterNode::set_area_interact);
    ClassDB::bind_method(D_METHOD("get_area_interact"), &OverworldCharacterNode::get_area_interact);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "character"), "set_character", "get_character");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "walk_speed", PROPERTY_HINT_RANGE, "0,400,1"), 
        "set_walk_speed", "get_walk_speed");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "frame_alert"), "set_frame_alert", "get_frame_alert");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sprite", PROPERTY_HINT_NODE_TYPE, "CharacterSprite"), "set_sprite", "get_sprite");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "area_interact", PROPERTY_HINT_NODE_TYPE, "InteractionTrigger"), "set_area_interact", "get_area_interact");
    
    ADD_GROUP("Dialogue", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_index"), "set_current_index", "get_current_index");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "dialogues", PROPERTY_HINT_RESOURCE_TYPE, "DialogueAsset"), "set_dialogues", "get_dialogues");
}

void OverworldCharacterNode::_ready() {
    if(!sprite || !area_interact) {
        ERR_PRINT("OverworldCharacterNode: Sprite 또는 InteractionTrigger 노드가 설정되지 않음");
        return;
    }
    area_interact->connect("interacted", Callable(this, "_on_area_interacted"));

    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load("res://Engine/RequiredNode/overworld_character_node.tscn");
    Node* requiredNode = scene->instantiate();
    add_child(requiredNode);
    
    alert = Object::cast_to<AnimatedSprite2D>(requiredNode->get_node_internal("Alert"));
    encounter = Object::cast_to<AudioStreamPlayer>(requiredNode->get_node_internal("encounter"));
    if(dialogues.is_valid()) dialogues->load_locale_data();
}

void OverworldCharacterNode::_physics_process(double delta) {
    Vector2 vel = Vector2(walk_direction) * walk_speed * walk_speed_mod;
    set_velocity(vel);
    
    // 이동 처리
    if(canmove) {
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

    if(!sprite) return;
    Vector2 alert_offset = sprite->get_position();
    Ref<SpriteFrames> frames = sprite->get_sprite_frames();
    Ref<Texture2D> texture = frames->get_frame_texture(sprite->get_animation(), sprite->get_frame());
    alert_offset.y -= texture->get_height();
    alert_offset += sprite->get_alert_offset();

    alert->set_position(alert_offset);
}

void OverworldCharacterNode::show_alert(float duration) {
    alert->set_frame(frame_alert);
    alert->show();
    encounter->play();
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(duration, false);
    timer->connect("timeout", Callable(alert, "hide"), CONNECT_ONE_SHOT);
}

void OverworldCharacterNode::_set_canmove(bool value) {
    canmove = value;
}

void OverworldCharacterNode::start_walking(const Vector2i& dir) {
    if(dir != Vector2i(0,0)) {
        canmove = true;
        set_walk_direction(dir);
        current_anim_state = MOVING;
        sprite->play(current_animation_name);
    }else {
        canmove = false;
        current_anim_state = IDLE;
        sprite->play(current_animation_name.replace("move_", "idle_"));
    }
}

void OverworldCharacterNode::force_direction(const Vector2& dir) {
    set_walk_direction(dir.normalized());
    walk_direction = Vector2i(0, 0);
    _physics_process(0);
}

void OverworldCharacterNode::set_frame(int index) {
    sprite->set_animation("act");
    sprite->set_frame(index);
}

void OverworldCharacterNode::play_anim(String key, float speed, bool from_end) {
    Callable calls = Callable(this, "emit_signal").bind("animation_finished");
    if(sprite->is_connected("animation_finished", calls)) sprite->disconnect("animation_finished", calls);
    sprite->connect("animation_finished", calls, CONNECT_ONE_SHOT);
    sprite->play(key, speed, from_end);
}

void OverworldCharacterNode::set_walk_direction(const Vector2i& direction) {
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

void OverworldCharacterNode::_on_area_interacted() {
    if(!dialogues.is_valid()) {
        printf("OverworldCharacterNode: 대화 에셋이 유효하지 않음\n");
        return;
    }
    if(!dialogues->has_data()) {
        printf("OverworldCharacterNode: 대화 내용이 없음\n");
        return;
    }
    
    TextBox* ct = stagehand->summontextbox();
    ct->connect("dialogue_finished", Callable(this, "emit_signal").bind("character_finished"), CONNECT_ONE_SHOT);
    Ref<Dialogues> data = dialogues->get_data(current_index);
    if(data.is_valid()) {
        ct->character(false, character, data);
    } else {
        ERR_PRINT("OverworldCharacterNode: 에러(2)");
    }
}

void OverworldCharacterNode::set_dialogues(Ref<DialogueAsset> p_dialogues) {
    dialogues = p_dialogues;
    if(!isEditor && dialogues.is_valid()) {
        dialogues->load_locale_data();
    }
}

Ref<DialogueAsset> OverworldCharacterNode::get_dialogues() const {
    return dialogues;
}

void OverworldCharacterNode::set_walk_speed(int p_speed) {
    walk_speed = p_speed;
}

int OverworldCharacterNode::get_walk_speed() const {
    return walk_speed;
}

void OverworldCharacterNode::set_character(String p_character) {
    character = p_character;
}

String OverworldCharacterNode::get_character() const {
    return character;
}

void OverworldCharacterNode::set_current_index(int index) {
    current_index = index;
}

int OverworldCharacterNode::get_current_index() {
    return current_index;
}

void OverworldCharacterNode::set_frame_alert(int value) {
    frame_alert = value;
}

int OverworldCharacterNode::get_frame_alert() {
    return frame_alert;
}

void OverworldCharacterNode::set_sprite(CharacterSprite* p_sprite) {
    sprite = p_sprite;
}

CharacterSprite* OverworldCharacterNode::get_sprite() const {
    return sprite;
}

void OverworldCharacterNode::set_area_interact(InteractionTrigger* p_area) {
    area_interact = p_area;
}

InteractionTrigger* OverworldCharacterNode::get_area_interact() const {
    return area_interact;
}