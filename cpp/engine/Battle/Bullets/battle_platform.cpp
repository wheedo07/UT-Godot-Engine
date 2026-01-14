#include "battle_platform.h"
#include "engine/Battle/Soul/soul.h"
#include "env.h"

BattlePlatform::BattlePlatform() {
    movement = Vector2(0, 0);
    velocity = Vector2(0,0);
    trans = Tween::TRANS_QUAD;
    ease = Tween::EASE_IN_OUT;
}

BattlePlatform::~BattlePlatform() {}

void BattlePlatform::_bind_methods() {
    BIND_ENUM_CONSTANT(VELOCITY);
    BIND_ENUM_CONSTANT(TWEEN);
    ClassDB::bind_method(D_METHOD("fire", "target", "length", "speed", "fire_mode"), &BattlePlatform::fire, DEFVAL(40.0f), DEFVAL(100.0f), DEFVAL(FireMode::TWEEN));
    ClassDB::bind_method(D_METHOD("_on_tween_completed"), &BattlePlatform::_on_tween_completed);

    ClassDB::bind_method(D_METHOD("set_trans", "value"), &BattlePlatform::set_trans);
    ClassDB::bind_method(D_METHOD("get_trans"), &BattlePlatform::get_trans);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenTrans", PROPERTY_HINT_ENUM, "Linear,Quad,Cubic,Quart,Quint,Sine,Circ,Exp,Elastic,Bounce,Back"), "set_trans", "get_trans");

    ClassDB::bind_method(D_METHOD("set_ease", "value"), &BattlePlatform::set_ease);
    ClassDB::bind_method(D_METHOD("get_ease"), &BattlePlatform::get_ease);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenEase", PROPERTY_HINT_ENUM, "In,Out,InOut,OutIn"), "set_ease", "get_ease");

    ClassDB::bind_method(D_METHOD("set_velocity_tween", "value"), &BattlePlatform::set_velocity_tween);
    ClassDB::bind_method(D_METHOD("get_velocity_tween"), &BattlePlatform::get_velocity_tween);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "velocity_tween", PROPERTY_HINT_RESOURCE_TYPE, "Tween", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_velocity_tween", "get_velocity_tween");

    ClassDB::bind_method(D_METHOD("set_velocity", "value"), &BattlePlatform::set_velocity);
    ClassDB::bind_method(D_METHOD("get_velocity"), &BattlePlatform::get_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_velocity", "get_velocity");
}

void BattlePlatform::_ready() {
    platform = Object::cast_to<NinePatchRect>(get_node_internal("Platform"));
    platform_col = Object::cast_to<StaticBody2D>(get_node_internal("StaticBody2D"));
    collision = Object::cast_to<CollisionShape2D>(get_node_internal("StaticBody2D/CollisionShape2D"));
    area = Object::cast_to<Area2D>(get_node_internal("Area2D"));
    collision_area = Object::cast_to<CollisionShape2D>(get_node_internal("Area2D/CollisionShape2D"));

    Ref<RectangleShape2D> rect_shape = memnew(RectangleShape2D);
    rect_shape->set_size(Vector2(1, 1));
    collision->set_shape(rect_shape);

    Ref<RectangleShape2D> rect_shape2 = memnew(RectangleShape2D);
    rect_shape2->set_size(Vector2(1, 1.5));
    collision_area->set_shape(rect_shape2);
}

void BattlePlatform::_physics_process(double delta) {
    if(!velocity.is_zero_approx()) {
        movement= velocity * delta;
        set_position(get_position() + movement);
    }

    TypedArray<Area2D> overlapping_areas = area->get_overlapping_areas();
    for (int i=0; i < overlapping_areas.size(); i++) {
        Area2D* overlap_node = Object::cast_to<Area2D>(overlapping_areas[i]);
        if(overlap_node->is_in_group("soul")) {
            SoulBattle* soul = Object::cast_to<SoulBattle>(overlap_node->get_parent());
            if(soul->get_mode() == SoulBattle::BLUE && soul->is_on_floor()) 
                soul->set_global_position(soul->get_global_position() + movement);
        }
    }
}

void BattlePlatform::fire(Vector2 target, float length, float speed, FireMode fire_mode) {
    Ref<RectangleShape2D> rect_shape = collision->get_shape();
    Ref<RectangleShape2D> rect_shape2 = collision_area->get_shape();
    set_size(Vector2(length, get_size().y));
    rect_shape->set_size(Vector2(length, get_size().y));
    rect_shape2->set_size(Vector2(length, get_size().y+10));
    collision->set_position(Vector2(length / 2, get_size().y));
    collision_area->set_position(Vector2(length / 2, get_size().y));
    Vector2 distance = target - get_position();
    switch(fire_mode) {
        case FireMode::VELOCITY:
            velocity = distance.normalized() * speed;
            movement = velocity * 0.016f;
            break;

        case FireMode::TWEEN:
            movement = distance.normalized() * speed * 0.016f;
            velocity_tween = create_tween()->set_ease(get_ease())->set_trans(get_trans());
            velocity_tween->tween_property(this, "position", distance, distance.length() / speed)->as_relative();
            velocity_tween->tween_callback(Callable(this, "_on_tween_completed"));
            velocity_tween->play();
            break;
    }
}

void BattlePlatform::_on_tween_completed() {
    movement = Vector2(0, 0);
}

void BattlePlatform::set_trans(Tween::TransitionType value) {
    trans = value;
}

Tween::TransitionType BattlePlatform::get_trans() const {
    return trans;
}

void BattlePlatform::set_ease(Tween::EaseType value) {
    ease = value;
}

Tween::EaseType BattlePlatform::get_ease() const {
    return ease;
}

void BattlePlatform::set_velocity_tween(Ref<Tween> value) {
    velocity_tween = value;
}

Ref<Tween> BattlePlatform::get_velocity_tween() const {
    return velocity_tween;
}

void BattlePlatform::set_velocity(Vector2 value) {
    velocity = value;
}

Vector2 BattlePlatform::get_velocity() const {
    return velocity;
}