#include "camera.h"
#include "undertale-engine/Global/ut.h"
using namespace godot;

UTGECamera::UTGECamera() {
    connect("ready", Callable(this, "_utge_ready"));
}

void UTGECamera::_bind_methods() {
    /* 내부 메서드 */
    ClassDB::bind_method(D_METHOD("_utge_ready"), &UTGECamera::_utge_ready);

    ClassDB::bind_method(D_METHOD("set_camera", "value"), &UTGECamera::set_camera);
    ClassDB::bind_method(D_METHOD("get_camera"), &UTGECamera::get_camera);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera2D", PROPERTY_USAGE_SCRIPT_VARIABLE | PROPERTY_USAGE_READ_ONLY), "set_camera", "get_camera");

    ClassDB::bind_method(D_METHOD("set_zoom", "value"), &UTGECamera::set_zoom);
    ClassDB::bind_method(D_METHOD("get_zoom"), &UTGECamera::get_zoom);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "zoom"), "set_zoom", "get_zoom");

    ClassDB::bind_method(D_METHOD("set_position_smoothing_enabled", "value"), &UTGECamera::set_position_smoothing_enabled);
    ClassDB::bind_method(D_METHOD("is_position_smoothing_enabled"), &UTGECamera::is_position_smoothing_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "position_smoothing_enabled"), "set_position_smoothing_enabled", "is_position_smoothing_enabled");

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
    _set_limits();
    camera->set_zoom(zoom);
    camera->set_position_smoothing_enabled(position_smoothing_enabled);
    set_remote_node(camera->get_path());
}

void UTGECamera::_set_limits() {
    if(!camera) return;
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
    if(camera) camera->set_zoom(value);
}

Vector2 UTGECamera::get_zoom() {
    return zoom;
}

void UTGECamera::set_position_smoothing_enabled(bool value) {
    position_smoothing_enabled = value;
    if(camera) camera->set_position_smoothing_enabled(value);
}

bool UTGECamera::is_position_smoothing_enabled() {
    return position_smoothing_enabled;
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