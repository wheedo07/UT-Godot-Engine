#include "green_soul.h"
#include "engine/Battle/Bullets/bullet_area.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

GreenShielding::GreenShielding() {}

GreenShielding::~GreenShielding() {}

void GreenShielding::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_timer_timeout"), &GreenShielding::_on_timer_timeout);
    ClassDB::bind_method(D_METHOD("_on_bullet_hit", "bullet"), &GreenShielding::_on_bullet_hit);
    ClassDB::bind_method(D_METHOD("_change_shield_rot_deg", "to"), &GreenShielding::_change_shield_rot_deg);
    ClassDB::bind_method(D_METHOD("_on_shield_area_entered", "area"), &GreenShielding::_on_shield_area_entered);
}

void GreenShielding::_draw() {
    draw_arc(Vector2(0, 0), RAD, 0, Math_TAU, 128, COLOR, 1);
}

void GreenShielding::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    hit_timer = Object::cast_to<Timer>(get_node_internal("Timer"));
    line = Object::cast_to<Line2D>(get_node_internal("Shield/Col/Line2D"));
    shield = Object::cast_to<Area2D>(get_node_internal("Shield"));
    ding_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("ding"));
    
    line->set_modulate(DEF_COL);
}

void GreenShielding::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    if (event->is_action_pressed("ui_left")) {
        _change_shield_rot_deg(0);
    }
    else if (event->is_action_pressed("ui_right")) {
        _change_shield_rot_deg(180);
    }
    else if (event->is_action_pressed("ui_up")) {
        _change_shield_rot_deg(90);
    }
    else if (event->is_action_pressed("ui_down")) {
        _change_shield_rot_deg(270);
    }
}

void GreenShielding::_change_shield_rot_deg(int to) {
    shield_tween = create_tween();
    shield_tween->set_trans(Tween::TRANS_SINE);
    shield_tween->tween_property(shield, "rotation_degrees", to, TIME);
}

void GreenShielding::_on_shield_area_entered(Area2D* area) {
    BulletArea* bullet_area = Object::cast_to<BulletArea>(area);
    Bullet* bullet = bullet_area->bullet;
    if(bullet) {
        call_deferred("_on_bullet_hit", bullet);
        ding_sound->play();
        line->set_modulate(HIT_COL);
        hit_timer->start();
    }else ERR_PRINT("bullet 없음 에러");
}

void GreenShielding::_on_bullet_hit(Bullet* bullet) {
    if(bullet->has_method("on_hit_player_shield")) { // C++ 이랑 GDscript 모두 호환되도록
        bullet->call("on_hit_player_shield");
    } else {
        bullet->on_hit_player_shield();
    }
}

void GreenShielding::_on_timer_timeout() {
    line->set_modulate(DEF_COL);
}