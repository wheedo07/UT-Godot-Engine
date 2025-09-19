#include "bullet.h"
#include "bullet_area.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/templates/hashfuncs.hpp>
using namespace godot;

Bullet::Bullet() {
    colors.resize(4);
    colors[MODE_WHITE] = Color(1.0, 1.0, 1.0); // WHITE
    colors[MODE_GREEN] = Color(0.0, 1.0, 0.0); // GREEN
    colors[MODE_BLUE] = Color(0.0, 0.85, 1.0); // BLUE
    colors[MODE_ORANGE] = Color(1.0, 0.65, 0.0); // ORANGE

    fire_mode = MOVEMENT_VELOCITY;
    damage_value = 5;
    iframe_grant_value = 50;
    kr_amount_value = 5;
    delete_upon_hit_value = false;
    sprite_path = NodePath();
    sprite = nullptr;
    damage_mode = MODE_WHITE;
}

Bullet::~Bullet() {}

void Bullet::_bind_methods() {
    // 스크립트 메소드
    GDVIRTUAL_BIND(ready);
    GDVIRTUAL_BIND(on_hit_player);
    GDVIRTUAL_BIND(on_hit_yellow);
    GDVIRTUAL_BIND(on_hit_player_shield);

    ADD_SIGNAL(MethodInfo("bullet_fade"));
    ADD_SIGNAL(MethodInfo("shake_camera", PropertyInfo(Variant::FLOAT, "shake_amt")));
    
    BIND_ENUM_CONSTANT(MOVEMENT_VELOCITY);
    BIND_ENUM_CONSTANT(MOVEMENT_TWEEN);
    
    BIND_ENUM_CONSTANT(MODE_WHITE);
    BIND_ENUM_CONSTANT(MODE_GREEN);
    BIND_ENUM_CONSTANT(MODE_BLUE);
    BIND_ENUM_CONSTANT(MODE_ORANGE);

    ClassDB::bind_method(D_METHOD("set_mode", "mode"), &Bullet::set_mode, DEFVAL(MODE_BLUE));
    ClassDB::bind_method(D_METHOD("fade"), &Bullet::fade);
    ClassDB::bind_method(D_METHOD("kill"), &Bullet::kill);
    ClassDB::bind_method(D_METHOD("on_exit_screen"), &Bullet::on_exit_screen);
    
    ClassDB::bind_method(D_METHOD("set_damage", "damage"), &Bullet::set_damage);
    ClassDB::bind_method(D_METHOD("get_damage"), &Bullet::get_damage);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "damage", PROPERTY_HINT_RANGE, "0,100,1"), "set_damage", "get_damage");
    
    ClassDB::bind_method(D_METHOD("set_iframe_grant", "iframe_grant"), &Bullet::set_iframe_grant);
    ClassDB::bind_method(D_METHOD("get_iframe_grant"), &Bullet::get_iframe_grant);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "iframe_grant", PROPERTY_HINT_RANGE, "0,100,1"), "set_iframe_grant", "get_iframe_grant");
    
    ClassDB::bind_method(D_METHOD("set_kr_amount", "kr_amount"), &Bullet::set_kr_amount);
    ClassDB::bind_method(D_METHOD("get_kr_amount"), &Bullet::get_kr_amount);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "kr_amount", PROPERTY_HINT_RANGE, "0,100,1"), "set_kr_amount", "get_kr_amount");
    
    ClassDB::bind_method(D_METHOD("set_delete_upon_hit", "delete_upon_hit"), &Bullet::set_delete_upon_hit);
    ClassDB::bind_method(D_METHOD("get_delete_upon_hit"), &Bullet::get_delete_upon_hit);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "delete_upon_hit"), "set_delete_upon_hit", "get_delete_upon_hit");
    
    ClassDB::bind_method(D_METHOD("set_sprite_path", "sprite_path"), &Bullet::set_sprite_path);
    ClassDB::bind_method(D_METHOD("get_sprite_path"), &Bullet::get_sprite_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "sprite_path"), "set_sprite_path", "get_sprite_path");
    
    ClassDB::bind_method(D_METHOD("set_colors", "colors"), &Bullet::set_colors);
    ClassDB::bind_method(D_METHOD("get_colors"), &Bullet::get_colors);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "colors", PROPERTY_HINT_ARRAY_TYPE, "Color"), "set_colors", "get_colors");

    ClassDB::bind_method(D_METHOD("set_velocity_tween", "velocity_tween"), &Bullet::set_velocity_tween);
    ClassDB::bind_method(D_METHOD("get_velocity_tween"), &Bullet::get_velocity_tween);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "velocity_tween", PROPERTY_HINT_NONE, "Tween", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_velocity_tween", "get_velocity_tween");
}

void Bullet::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    sprite = Object::cast_to<CanvasItem>(get_node_internal(sprite_path));

    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    } else {
        ready();
    }
}

void Bullet::ready() {}
void Bullet::on_hit_player() {}
void Bullet::on_hit_yellow() {}
void Bullet::on_hit_player_shield() {}

void Bullet::_physics_process(double delta) {
    if(isEditor) return;

    // 스프라이트 색상 모드에 따라 설정
    if (damage_mode >= 0 && damage_mode < colors.size()) {
        sprite->set_modulate(colors[damage_mode]);
    }
    
    // 이동 처리
    if(fire_mode == MOVEMENT_VELOCITY) {
        move_and_slide();
    }
}

Bullet* Bullet::set_mode(DamageMode mode) {
    damage_mode = mode;
    return this;
}

void Bullet::kill() {
    if(velocity_tween.is_valid() && velocity_tween->is_running()) velocity_tween->kill();
    queue_free();
}

void Bullet::fade() {
    Ref<Tween> fade_tween = create_tween()->set_trans(Tween::TRANS_EXPO)->set_ease(Tween::EASE_OUT);
    fade_tween->tween_property(this, "modulate:a", 0.0, 0.5);

    emit_signal("bullet_fade");
    
    fade_tween->tween_callback(Callable(this, "kill"));
    fade_tween->play();
}

void Bullet::on_exit_screen() {
    if(velocity_tween.is_valid() && !velocity_tween->is_valid()) {
        fade();
        return;
    }

    if(!velocity_tween.is_valid() || !get_velocity().is_zero_approx()) 
        fade();
}

void Bullet::set_damage(int p_damage) {
    damage_value = p_damage;
}

int Bullet::get_damage() const {
    return damage_value;
}

void Bullet::set_iframe_grant(int p_iframe_grant) {
    iframe_grant_value = p_iframe_grant;
}

int Bullet::get_iframe_grant() const {
    return iframe_grant_value;
}

void Bullet::set_kr_amount(int p_kr_amount) {
    kr_amount_value = p_kr_amount;
}

int Bullet::get_kr_amount() const {
    return kr_amount_value;
}

void Bullet::set_delete_upon_hit(bool p_delete_upon_hit) {
    delete_upon_hit_value = p_delete_upon_hit;
}

bool Bullet::get_delete_upon_hit() const {
    return delete_upon_hit_value;
}

void Bullet::set_sprite_path(const NodePath& p_sprite_path) {
    sprite_path = p_sprite_path;
}

NodePath Bullet::get_sprite_path() const {
    return sprite_path;
}

void Bullet::set_colors(const Array& p_colors) {
    colors = p_colors;
}

Array Bullet::get_colors() const {
    return colors;
}

void Bullet::set_velocity_tween(const Ref<Tween>& value) {
    velocity_tween = value;
}

Ref<Tween> Bullet::get_velocity_tween() const {
    return velocity_tween;
}