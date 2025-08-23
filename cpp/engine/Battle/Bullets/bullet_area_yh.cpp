#include "bullet_area_yh.h"
using namespace godot;

void BulletAreaYellowHittable::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_yellow_bullet_hit"), &BulletAreaYellowHittable::_on_yellow_bullet_hit);
}

void BulletAreaYellowHittable::_on_yellow_bullet_hit() {
    if(bullet->has_method("on_hit_yellow")) { // C++ 이랑 GDscript 모두 호환되도록
        bullet->call("on_hit_yellow");
    } else {
        bullet->on_hit_yellow();
    }
}