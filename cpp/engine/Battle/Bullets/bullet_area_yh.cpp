#include "bullet_area_yh.h"
using namespace godot;

void BulletAreaYellowHittable::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_yellow_bullet_hit"), &BulletAreaYellowHittable::_on_yellow_bullet_hit);
}

void BulletAreaYellowHittable::_on_yellow_bullet_hit() {
    bullet->_on_hit_yellow();
}