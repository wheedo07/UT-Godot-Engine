#include "battle_object.h"
using namespace godot;

BattleObject::BattleObject() {
    velocity = Vector2(0,0);
    trans = Tween::TRANS_QUAD;
    ease = Tween::EASE_IN_OUT;
}

BattleObject::~BattleObject() {}

void BattleObject::_bind_methods() {
    BIND_ENUM_CONSTANT(VELOCITY);
    BIND_ENUM_CONSTANT(TWEEN);
    ClassDB::bind_method(D_METHOD("fade"), &BattleObject::fade);
    ClassDB::bind_method(D_METHOD("kill"), &BattleObject::kill);
    ClassDB::bind_method(D_METHOD("on_exit_screen"), &BattleObject::on_exit_screen);

    ClassDB::bind_method(D_METHOD("set_trans", "value"), &BattleObject::set_trans);
    ClassDB::bind_method(D_METHOD("get_trans"), &BattleObject::get_trans);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenTrans", PROPERTY_HINT_ENUM, "Linear,Quad,Cubic,Quart,Quint,Sine,Circ,Exp,Elastic,Bounce,Back"), "set_trans", "get_trans");

    ClassDB::bind_method(D_METHOD("set_ease", "value"), &BattleObject::set_ease);
    ClassDB::bind_method(D_METHOD("get_ease"), &BattleObject::get_ease);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenEase", PROPERTY_HINT_ENUM, "In,Out,InOut,OutIn"), "set_ease", "get_ease");
}

void BattleObject::fade() {
    Ref<Tween> fade_tween = create_tween()->set_trans(Tween::TRANS_EXPO)->set_ease(Tween::EASE_OUT);
    fade_tween->tween_property(this, "modulate:a", 0, 0.5);
    fade_tween->tween_callback(Callable(this, "kill"));
    fade_tween->play();
}

void BattleObject::kill() {
    if(velocity_tween.is_valid() && velocity_tween->is_running()) velocity_tween->kill();
    queue_free();
}

void BattleObject::on_exit_screen() {
    if(velocity_tween.is_valid() && !velocity_tween->is_valid()) {
        fade();
        return;
    }

    if(!velocity_tween.is_valid() || !velocity.is_zero_approx()) 
        fade();
}

void BattleObject::set_trans(Tween::TransitionType value) {
    trans = value;
}

Tween::TransitionType BattleObject::get_trans() const {
    return trans;
}

void BattleObject::set_ease(Tween::EaseType value) {
    ease = value;
}

Tween::EaseType BattleObject::get_ease() const {
    return ease;
}