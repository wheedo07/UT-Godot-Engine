#include "yellow_soul_bullet.h"
#include "env.h"

YellowBullet::YellowBullet() {
    velocity = Vector2(0, 0);
}

YellowBullet::~YellowBullet() {}

void YellowBullet::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_area_2d_area_entered", "area"), &YellowBullet::_on_area_2d_area_entered);
    
    ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &YellowBullet::set_velocity);
    ClassDB::bind_method(D_METHOD("get_velocity"), &YellowBullet::get_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity"), "set_velocity", "get_velocity");
}

void YellowBullet::_physics_process(double delta) {
    if(isEditor) return;
    set_position(get_position() + velocity);

    Rect2 screen_rect = get_viewport()->get_visible_rect();
    Vector2 global_pos = get_global_position();
    
    // 화면 바깥으로 나갔는지 확인
    bool outside_screen = 
        global_pos.x < -50 || 
        global_pos.x > screen_rect.size.width + 50 || 
        global_pos.y < -50 || 
        global_pos.y > screen_rect.size.height + 50;
        
    if (outside_screen) queue_free();
}

void YellowBullet::_on_area_2d_area_entered(Area2D* area) {
    if(area->is_class("BulletAreaYellowHittable")) area->call("_on_yellow_bullet_hit");
}

void YellowBullet::set_velocity(const Vector2& p_velocity) {
    velocity = p_velocity;
}

Vector2 YellowBullet::get_velocity() const {
    return velocity;
}