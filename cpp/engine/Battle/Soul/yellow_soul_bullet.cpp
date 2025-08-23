#include "yellow_soul_bullet.h"
#include "env.h"

YellowBullet::YellowBullet() {
    velocity = Vector2(0, 0);
    speed = 4.5;
}

YellowBullet::~YellowBullet() {}

void YellowBullet::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_area_2d_area_entered", "area"), &YellowBullet::_on_area_2d_area_entered);
    ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &YellowBullet::set_velocity);

    ClassDB::bind_method(D_METHOD("set_speed", "value"), &YellowBullet::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &YellowBullet::get_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

void YellowBullet::_physics_process(double delta) {
    if(isEditor) return;
    set_position(get_position() + velocity);
}

void YellowBullet::_on_area_2d_area_entered(Area2D* area) {
    if(area->is_class("BulletAreaYellowHittable")) area->call("_on_yellow_bullet_hit");
}

void YellowBullet::set_velocity(const Vector2& p_velocity) {
    velocity = p_velocity;
}

void YellowBullet::set_speed(float value) {
    speed = value;
}

float YellowBullet::get_speed() {
    return speed;
}