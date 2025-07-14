#include "defaultbullet.h"
using namespace godot;

DefaultBullet::DefaultBullet() {
    collision_margin = 4.0f;
    tween_trans = Tween::TRANS_QUAD;
    tween_ease = Tween::EASE_IN_OUT;
}

DefaultBullet::~DefaultBullet() {}

void DefaultBullet::_bind_methods() {
    ClassDB::bind_method(D_METHOD("fire", "target", "movement_type", "speed"), &DefaultBullet::fire, DEFVAL(100.0f));
    ClassDB::bind_method(D_METHOD("queue_fire", "delay", "target", "movement_type", "speed"), &DefaultBullet::queue_fire, DEFVAL(100.0f));
    ClassDB::bind_method(D_METHOD("_await_fire", "fire_call", "delay"), &DefaultBullet::_await_fire);
    ClassDB::bind_method(D_METHOD("_on_tween_finished", "fire_call", "delay"), &DefaultBullet::_on_tween_finished);
    ClassDB::bind_method(D_METHOD("get_sprite_size"), &DefaultBullet::get_sprite_size);

    ClassDB::bind_method(D_METHOD("set_collision_margin", "margin"), &DefaultBullet::set_collision_margin);
    ClassDB::bind_method(D_METHOD("get_collision_margin"), &DefaultBullet::get_collision_margin);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "collision_margin"), "set_collision_margin", "get_collision_margin");

    ClassDB::bind_method(D_METHOD("set_collision_path", "collision_path"), &DefaultBullet::set_collision_path);
    ClassDB::bind_method(D_METHOD("get_collision_path"), &DefaultBullet::get_collision_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "collision_path"), "set_collision_path", "get_collision_path");

    ClassDB::bind_method(D_METHOD("set_tweenTrans", "value"), &DefaultBullet::set_tweenTrans);
    ClassDB::bind_method(D_METHOD("get_tweenTrans"), &DefaultBullet::get_tweenTrans);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenTrans", PROPERTY_HINT_ENUM, "Linear,Sine,Quint,Quart,Quad,Expo,Elastic,Cubic,Circ,Bounce,Back,Spring"), "set_tweenTrans", "get_tweenTrans");

    ClassDB::bind_method(D_METHOD("set_tweenEase", "value"), &DefaultBullet::set_tweenEase);
    ClassDB::bind_method(D_METHOD("get_tweenEase"), &DefaultBullet::get_tweenEase);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenEase", PROPERTY_HINT_ENUM, "In,Out,InOut,OutIn"), "set_tweenEase", "get_tweenEase");
}

void DefaultBullet::ready() {
    sprite = get_node_internal(get_sprite_path());
    collision = Object::cast_to<CollisionShape2D>(get_node_internal(get_collision_path()));
    
    Ref<RectangleShape2D> shape = memnew(RectangleShape2D);
    Vector2 sprite_size = get_sprite_size();
    shape->set_size(sprite_size);
    collision->set_shape(shape);
}

void DefaultBullet::_process(double delta) {
    if(!is_inside_tree() || isEditor) return;
    
    Vector2 sprite_size = get_sprite_size();
    
    Ref<RectangleShape2D> shape = collision->get_shape();
    shape->set_size(Vector2(sprite_size.x - collision_margin, sprite_size.y - collision_margin));
}

void DefaultBullet::fire(const Vector2& target, MovementMode movement_type, float speed) {
    fire_mode = movement_type;
    target_position = target;
    Vector2 distance;
    if(get_parent()->is_class("Node2D"))
        distance = target_position - get_global_position();
    else distance = target_position - get_position();
    
    switch(fire_mode) {
        case MOVEMENT_VELOCITY: {
            set_velocity(speed * distance.normalized());
            break;
        }
        case MOVEMENT_TWEEN: {
            velocity_tween = create_tween();
            velocity_tween->set_ease(tween_ease);
            velocity_tween->set_trans(tween_trans);
            
            velocity_tween->tween_property(this, "position", distance, distance.length() / speed)->as_relative();
            break;
        }
    }
}

void DefaultBullet::queue_fire(float delay, const Vector2& target, MovementMode movement_type, float speed) {
    Callable fire_call = Callable(this, "fire").bind(target, movement_type, speed);
    _await_fire(fire_call, delay);
}

void DefaultBullet::_await_fire(const Callable& fire_call, float delay) {
    if (tw.is_valid() && tw->is_running()) {
        tw->connect("finished", Callable(this, "_on_tween_finished").bind(fire_call, delay));
        return;
    }
    
    _start_await_fire(fire_call, delay);
}

void DefaultBullet::_on_tween_finished(const Callable& fire_call, float delay) {
    _start_await_fire(fire_call, delay);
}

void DefaultBullet::_start_await_fire(const Callable& fire_call, float delay) {
    tw = create_tween();
    
    if (velocity_tween.is_valid() && velocity_tween->is_running()) {
        tw->pause();
        velocity_tween->connect("finished", Callable(tw.ptr(), "play"));
    }
    
    tw->tween_interval(delay);
    tw->tween_callback(fire_call);
}

Vector2 DefaultBullet::get_sprite_size() {
    String class_name = sprite->get_class();
    Vector2 size;
    Vector2 scale = Vector2(1, 1);
    
    if (class_name == "Sprite2D" || class_name == "AnimatedSprite2D") {
        if (sprite->has_method("get_texture")) {
            Variant texture = sprite->call("get_texture");
            if (texture.get_type() != Variant::NIL) {
                size = texture.call("get_size");
            }
        } else if (sprite->has_method("get_sprite_frames")) {
            Variant frames = sprite->call("get_sprite_frames");
            if (frames.get_type() != Variant::NIL) {
                String anim = sprite->call("get_animation");
                int frame = sprite->call("get_frame");
                Variant texture = frames.call("get_frame_texture", anim, frame);
                if (texture.get_type() != Variant::NIL) {
                    size = texture.call("get_size");
                }
            }
        }
        
        if (sprite->has_method("get_scale")) {
            scale = sprite->call("get_scale");
        }
    } else if (class_name == "TextureRect" || class_name == "NinePatchRect") {
        if (sprite->has_method("get_size")) {
            size = sprite->call("get_size");
        }
    } else {
        size = Vector2(16, 16);
    }
    
    return Vector2(size.x * scale.x, size.y * scale.y);
}

void DefaultBullet::set_collision_margin(float p_collision_margin) {
    collision_margin = p_collision_margin;
}

float DefaultBullet::get_collision_margin() const {
    return collision_margin;
}

void DefaultBullet::set_collision_path(const NodePath& p_collision_path) {
    collision_path = p_collision_path;
}

NodePath DefaultBullet::get_collision_path() const {
    return collision_path;
}

void DefaultBullet::set_tweenTrans(Tween::TransitionType value) {
    tween_trans = value;
}

Tween::TransitionType DefaultBullet::get_tweenTrans() const {
    return tween_trans;
}

void DefaultBullet::set_tweenEase(Tween::EaseType value) {
    tween_ease = value;
}

Tween::EaseType DefaultBullet::get_tweenEase() const {
    return tween_ease;
}