#include "bullet_area.h"
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

BulletArea::BulletArea() {
    bullet_path = NodePath();
}

BulletArea::~BulletArea() {}

void BulletArea::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_yellow_bullet_hit"), &BulletArea::_on_yellow_bullet_hit);
    ClassDB::bind_method(D_METHOD("set_bullet_path", "value"), &BulletArea::set_bullet_path);
    ClassDB::bind_method(D_METHOD("get_bullet_path"), &BulletArea::get_bullet_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "bullet_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Bullet"), "set_bullet_path", "get_bullet_path");
}

void BulletArea::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    bullet = Object::cast_to<Bullet>(get_node_internal(bullet_path));
    bullet->connect("bullet_fade", Callable(this, "set_monitorable").bind(false));
    bullet->connect("bullet_fade", Callable(this, "set_monitoring").bind(false));
    damage = bullet->get_damage();
    iframes = bullet->get_iframe_grant();
    kr = bullet->get_kr_amount();
}

void BulletArea::_process(double delta) {
    if(isEditor) return;
    damage_mode = bullet->damage_mode;
    if(is_monitoring()) {
        TypedArray<Area2D> overlapping_areas = get_overlapping_areas();
        
        for (int i = 0; i < overlapping_areas.size(); i++) {
            Node* overlap_node = Object::cast_to<Node>(overlapping_areas[i]);
            
            if (overlap_node && overlap_node->is_in_group("player") && 
                (damage_mode <= Bullet::MODE_GREEN) && bullet->delete_upon_hit_value) {
                    bullet->_on_hit_player();
            }
        }
    }
}

void BulletArea::_on_yellow_bullet_hit() {
    if(bullet->has_method("on_hit_yellow")) { // C++ 이랑 GDscript 모두 호환되도록
        bullet->call("on_hit_yellow");
    } else {
        bullet->on_hit_yellow();
    }
}

void BulletArea::set_bullet_path(NodePath value) {
    bullet_path = value;
}

NodePath BulletArea::get_bullet_path() const {
    return bullet_path;
}