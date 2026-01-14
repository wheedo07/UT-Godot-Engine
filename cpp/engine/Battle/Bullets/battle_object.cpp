#include "battle_object.h"
using namespace godot;

BattleObject::BattleObject() {}

BattleObject::~BattleObject() {}

void BattleObject::_bind_methods() {
    ClassDB::bind_method(D_METHOD("fade"), &BattleObject::fade);
}

void BattleObject::fade() {
    Ref<Tween> fade_tween = create_tween()->set_trans(Tween::TRANS_EXPO)->set_ease(Tween::EASE_OUT);
    fade_tween->tween_property(this, "modulate:a", 0, 0.5);
    fade_tween->tween_callback(Callable(this, "queue_free"));
    fade_tween->play();
}