#include "enemy_sans.h"
#include "env.h"

Enemy_SANS::Enemy_SANS() {}

Enemy_SANS::~Enemy_SANS() {}

void Enemy_SANS::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_throws", "dir", "power"), &Enemy_SANS::_on_throws, DEFVAL(Vector2(0, 1)), DEFVAL(10));
    ClassDB::bind_method(D_METHOD("end_throws"), &Enemy_SANS::end_throws);
}

void Enemy_SANS::ready() {
    body = Object::cast_to<AnimatedSprite2D>(get_node_internal("sans/Idle/body"));
    head = Object::cast_to<AnimatedSprite2D>(get_node_internal("sans/Idle/body/head"));
    throw_head = Object::cast_to<AnimatedSprite2D>(get_node_internal("sans/Throw/head"));
    AnimStates = Object::cast_to<AnimationNodeStateMachinePlayback>(get_node_internal("sans/States")->get("parameters/playback"));
    throw_timer = Object::cast_to<Timer>(get_node_internal("Timer"));
    sprite = Object::cast_to<Node2D>(get_sprites());
    attackScene = ResourceLoader::get_singleton()->load("res://Game/mainAttacks/attack_sans.tscn");
}

void Enemy_SANS::_on_get_turn() {
    create_attack()->start_attack();
}

void Enemy_SANS::_on_end_turn() {}

void Enemy_SANS::on_mercy_used() {
    box->blitter_print({ String::utf8("* 더 이상 샌즈에게 이건 소용없을거 같다") });
    sys->executeTrue([this]() { return !global->get_battle_text_box(); },
    [this]() {
        emit_signal("on_mercy_end");
    });
}

AttackSans* Enemy_SANS::create_attack() {
    AttackSans* attack = Object::cast_to<AttackSans>(attacks->add_attack(attackScene));
    attack->connect("throws", Callable(this, "_on_throws"));
    return attack;
}

void Enemy_SANS::_on_throws(Vector2 dir, int power) {
    throw_timer->start();
    AnimStates->stop();
    String key = "";
    if(dir == Vector2(-1, 0))
        key = "throw_left";
    else if(dir == Vector2(1, 0))
        key = "throw_right";
    else if(dir == Vector2(0, 1))
        key = "throw_down";
    else if(dir == Vector2(0, -1))
        key = "throw_up";

    AnimStates->travel(key); 
    throw_head->play("crazy", true);

    sys->sleep([this, dir, power]() {
        soul->set_gravity_direction(dir);
        soul->set_gravity_multiplier(power);
    }, 0.2);
}

void Enemy_SANS::end_throws() {
    throw_head->set_animation("heads");
}

void Enemy_SANS::camera_pro(float time, String key, Variant value) {
    Ref<Tween> tween = create_tween();
    // zoom, position, rotation
    if(key == "all") {
        tween->tween_property(main->camera, "zoom", Vector2(1,1), time);
        tween->tween_property(main->camera, "position", Vector2(320, 240), time);
        tween->tween_property(main->camera, "rotation", 0, time);
    }else tween->tween_property(main->camera, key, value, time);
}