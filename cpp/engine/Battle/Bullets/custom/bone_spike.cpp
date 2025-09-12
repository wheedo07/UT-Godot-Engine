#include "bone_spike.h"
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/timer.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/canvas_item.hpp>
#define SpikeTime 0.3f
using namespace godot;

BoneSpike::BoneSpike() {
    collision_margin = 4.0f;
    tween_trans = Tween::TRANS_EXPO;
    tween_ease = Tween::EASE_OUT;
}

BoneSpike::~BoneSpike() {}

void BoneSpike::_bind_methods() {
    ClassDB::bind_method(D_METHOD("fire", "size", "warn_time", "remain_time", "mode"), &BoneSpike::fire, 
        DEFVAL(0.4f), DEFVAL(1.0f), DEFVAL(MODE_WHITE));
    ClassDB::bind_method(D_METHOD("spike", "remain_time"), &BoneSpike::spike);
    ClassDB::bind_method(D_METHOD("on_warn_time_timeout", "size", "remain_time"), &BoneSpike::on_warn_time_timeout);
    
    ClassDB::bind_method(D_METHOD("set_collision_margin", "margin"), &BoneSpike::set_collision_margin);
    ClassDB::bind_method(D_METHOD("get_collision_margin"), &BoneSpike::get_collision_margin);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "collision_margin"), "set_collision_margin", "get_collision_margin");

    ClassDB::bind_method(D_METHOD("set_tweenTrans", "value"), &BoneSpike::set_tweenTrans);
    ClassDB::bind_method(D_METHOD("get_tweenTrans"), &BoneSpike::get_tweenTrans);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenTrans", PROPERTY_HINT_ENUM, "Linear,Sine,Quint,Quart,Quad,Expo,Elastic,Cubic,Circ,Bounce,Back,Spring"), "set_tweenTrans", "get_tweenTrans");

    ClassDB::bind_method(D_METHOD("set_tweenEase", "value"), &BoneSpike::set_tweenEase);
    ClassDB::bind_method(D_METHOD("get_tweenEase"), &BoneSpike::get_tweenEase);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenEase", PROPERTY_HINT_ENUM, "In,Out,InOut"), "set_tweenEase", "get_tweenEase");
}

void BoneSpike::ready() {
    warning = Object::cast_to<ReferenceRect>(get_node_internal("Warning"));
    mode_hint = Object::cast_to<Panel>(get_node_internal("Warning/Panel"));
    sprite_rect = Object::cast_to<NinePatchRect>(get_node_internal(get_sprite_path()));
    collision = Object::cast_to<CollisionShape2D>(get_node_internal("Area2D/CollisionShape2D"));
}

void BoneSpike::fire(const Vector2& size, float warn_time, float remain_time, DamageMode mode) {
    set_mode(mode);
    
    sprite_rect->set_size(size);
    warning->set_size(size);
    Panel* panel = Object::cast_to<Panel>(warning->get_child(0));
    panel->set_modulate(get_colors()[mode]);
    
    Object::cast_to<AudioStreamPlayer>(get_node_internal("Alert"))->play();
    
    Ref<Tween> alert_tween = create_tween()->set_trans(Tween::TRANS_EXPO)->set_ease(Tween::EASE_OUT);
    Ref<Tween> alert_tween2 = create_tween()->set_trans(Tween::TRANS_EXPO)->set_ease(Tween::EASE_IN);
    alert_tween->tween_property(mode_hint, "self_modulate:a", 1, warn_time / 5.0f);
    alert_tween2->tween_property(warning, "modulate:a", 0, warn_time * 7.0f / 8.0f);
    alert_tween->tween_property(mode_hint, "self_modulate:a", 0, 4.0f * warn_time / 5.0f);
    
    SceneTree* tree = get_tree();
    Ref<SceneTreeTimer> timer = tree->create_timer(warn_time, false);
    
    timer->connect("timeout", Callable(this, "on_warn_time_timeout").bind(size, remain_time));
}

void BoneSpike::on_warn_time_timeout(const Vector2& size, float remain_time) {
    Ref<RectangleShape2D> shape = memnew(RectangleShape2D);
    
    warning->hide();
    sprite_rect->show();
    
    shape->set_size(Vector2(size.x, 0));
    collision->set_position(Vector2(size.x / 2.0f, 0));
    sprite_rect->set_size(Vector2(size.x, 0));
    collision->set_shape(shape);
    
    spike(remain_time);
}

void BoneSpike::spike(float remain_time) {
    Object::cast_to<AudioStreamPlayer>(get_node_internal("Spike"))->play();

    Ref<Tween> tw = create_tween()->set_ease(tween_ease)->set_trans(tween_trans)->set_parallel();
    tw->tween_property(sprite_rect, "size:y", warning->get_size().y, SpikeTime);
    
    Ref<RectangleShape2D> shape = collision->get_shape();
    float warning_size_y = warning->get_size().y;
    tw->tween_property(shape.ptr(), "size:y", warning_size_y - collision_margin, SpikeTime);
    tw->tween_property(collision, "position:y", (warning_size_y - collision_margin) / 2.0f, SpikeTime);
    tw->tween_interval(remain_time);
    tw->chain();
    tw->tween_property(sprite_rect, "size:y", 0, SpikeTime*2);
    tw->tween_property(shape.ptr(), "size:y", 0, SpikeTime*2);
    tw->tween_property(collision, "position:y", 0, SpikeTime*2);
    tw->tween_property(sprite_rect, "position:y", 33, SpikeTime*2);
    tw->tween_callback(Callable(this, "queue_free"));
}

void BoneSpike::set_collision_margin(float p_collision_margin) {
    collision_margin = p_collision_margin;
}

float BoneSpike::get_collision_margin() const {
    return collision_margin;
}

void BoneSpike::set_tweenTrans(Tween::TransitionType value) {
    tween_trans = value;
}

Tween::TransitionType BoneSpike::get_tweenTrans() const {
    return tween_trans;
}

void BoneSpike::set_tweenEase(Tween::EaseType value) {
    tween_ease = value;
}

Tween::EaseType BoneSpike::get_tweenEase() const {
    return tween_ease;
}