#include "camera.h"
#include "undertale-engine/Global/ut.h"
using namespace godot;

UTGECamera::UTGECamera() {
    connect("ready", callable_mp(this, &UTGECamera::_utge_ready));
}

void UTGECamera::_bind_methods() {
    /* API 함수 */
    ClassDB::bind_method(D_METHOD("activate"), &UTGECamera::activate);
    ClassDB::bind_method(D_METHOD("deactivate"), &UTGECamera::deactivate);
    ClassDB::bind_method(D_METHOD("is_active"), &UTGECamera::is_active);

    /* 스크립트 속성 */
    ClassDB::bind_method(D_METHOD("set_camera", "value"), &UTGECamera::set_camera);
    ClassDB::bind_method(D_METHOD("get_camera"), &UTGECamera::get_camera);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera2D", PROPERTY_USAGE_SCRIPT_VARIABLE | PROPERTY_USAGE_READ_ONLY), "set_camera", "get_camera");

    /* 공개 속성 */
    ClassDB::bind_method(D_METHOD("set_zoom", "value"), &UTGECamera::set_zoom);
    ClassDB::bind_method(D_METHOD("get_zoom"), &UTGECamera::get_zoom);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "zoom"), "set_zoom", "get_zoom");

    ADD_GROUP("Position Smoothing", "position_smoothing_");
    ClassDB::bind_method(D_METHOD("set_position_smoothing_enabled", "value"), &UTGECamera::set_position_smoothing_enabled);
    ClassDB::bind_method(D_METHOD("is_position_smoothing_enabled"), &UTGECamera::is_position_smoothing_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "position_smoothing_enabled", PROPERTY_HINT_GROUP_ENABLE), "set_position_smoothing_enabled", "is_position_smoothing_enabled");

    ClassDB::bind_method(D_METHOD("set_position_smoothing_speed", "value"), &UTGECamera::set_position_smoothing_speed);
    ClassDB::bind_method(D_METHOD("get_position_smoothing_speed"), &UTGECamera::get_position_smoothing_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "position_smoothing_speed"), "set_position_smoothing_speed", "get_position_smoothing_speed");

    ADD_GROUP("Limits", "limit_");
    ClassDB::bind_method(D_METHOD("set_limit_left", "value"), &UTGECamera::set_limit_left);
    ClassDB::bind_method(D_METHOD("get_limit_left"), &UTGECamera::get_limit_left);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_left"), "set_limit_left", "get_limit_left");

    ClassDB::bind_method(D_METHOD("set_limit_top", "value"), &UTGECamera::set_limit_top);
    ClassDB::bind_method(D_METHOD("get_limit_top"), &UTGECamera::get_limit_top);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_top"), "set_limit_top", "get_limit_top");

    ClassDB::bind_method(D_METHOD("set_limit_right", "value"), &UTGECamera::set_limit_right);
    ClassDB::bind_method(D_METHOD("get_limit_right"), &UTGECamera::get_limit_right);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_right"), "set_limit_right", "get_limit_right");

    ClassDB::bind_method(D_METHOD("set_limit_bottom", "value"), &UTGECamera::set_limit_bottom);
    ClassDB::bind_method(D_METHOD("get_limit_bottom"), &UTGECamera::get_limit_bottom);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_bottom"), "set_limit_bottom", "get_limit_bottom");
}

void UTGECamera::_utge_ready() {
    camera = UT::tree()->get_root()->get_camera();
    UT::tree()->get_root()->set_current_camera(this);
}

void UTGECamera::activate() {
    if(!camera) return;
    _set_limits();
    camera->set_zoom(zoom);
    camera->set_position_smoothing_enabled(position_smoothing_enabled);
    camera->set_position_smoothing_speed(position_smoothing_speed);
    set_remote_node(camera->get_path());
}

void UTGECamera::deactivate() {
    if(!camera) return;
    set_remote_node(NodePath());
}

bool UTGECamera::is_active() {
    if(!camera) return false;
    return get_remote_node() == camera->get_path();
}

void UTGECamera::_set_limits() {
    if(!is_active()) return;
    camera->set_limit(Side::SIDE_LEFT, limit_left);
    camera->set_limit(Side::SIDE_TOP, limit_top);
    camera->set_limit(Side::SIDE_RIGHT, limit_right);
    camera->set_limit(Side::SIDE_BOTTOM, limit_bottom);
}

void UTGECamera::set_camera(Camera2D *value) {
    camera = value;
}

Camera2D *UTGECamera::get_camera() {
    return camera;
}

void UTGECamera::set_zoom(Vector2 value) {
    zoom = value;
    if(is_active()) camera->set_zoom(value);
}

Vector2 UTGECamera::get_zoom() {
    return zoom;
}

void UTGECamera::set_position_smoothing_enabled(bool value) {
    position_smoothing_enabled = value;
    if(is_active()) camera->set_position_smoothing_enabled(value);
}

bool UTGECamera::is_position_smoothing_enabled() {
    return position_smoothing_enabled;
}

void UTGECamera::set_position_smoothing_speed(double value) {
    position_smoothing_speed = value;
    if(is_active()) camera->set_position_smoothing_speed(value);
}

double UTGECamera::get_position_smoothing_speed() {
    return position_smoothing_speed;
}

void UTGECamera::set_limit_left(int value) {
    limit_left = value;
    _set_limits();
}

int UTGECamera::get_limit_left() {
    return limit_left;
}

void UTGECamera::set_limit_top(int value) {
    limit_top = value;
    _set_limits();
}

int UTGECamera::get_limit_top() {
    return limit_top;
}

void UTGECamera::set_limit_right(int value) {
    limit_right = value;
    _set_limits();
}

int UTGECamera::get_limit_right() {
    return limit_right;
}

void UTGECamera::set_limit_bottom(int value) {
    limit_bottom = value;
    _set_limits();
}

int UTGECamera::get_limit_bottom() {
    return limit_bottom;
}