#include "battle_object.h"
using namespace godot;

BattleObject::BattleObject() {
    velocity = Vector2(0,0);
}

BattleObject::~BattleObject() {}

void BattleObject::_bind_methods() {
    ClassDB::bind_method(D_METHOD("fade"), &BattleObject::fade);
    ClassDB::bind_method(D_METHOD("kill"), &BattleObject::kill);
    ClassDB::bind_method(D_METHOD("on_exit_screen"), &BattleObject::on_exit_screen);
    
    ClassDB::bind_method(D_METHOD("set_velocity_tween", "value"), &BattleObject::set_velocity_tween);
    ClassDB::bind_method(D_METHOD("get_velocity_tween"), &BattleObject::get_velocity_tween);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "velocity_tween", PROPERTY_HINT_RESOURCE_TYPE, "Tween", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_velocity_tween", "get_velocity_tween");
    
    ClassDB::bind_method(D_METHOD("set_velocity", "value"), &BattleObject::set_velocity);
    ClassDB::bind_method(D_METHOD("get_velocity"), &BattleObject::get_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_velocity", "get_velocity");
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

void BattleObject::set_velocity_tween(Ref<Tween> value) {
    velocity_tween = value;
}

Ref<Tween> BattleObject::get_velocity_tween() const {
    return velocity_tween;
}

void BattleObject::set_velocity(Vector2 value) {
    velocity = value;
}

Vector2 BattleObject::get_velocity() const {
    return velocity;
}