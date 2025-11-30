#include "camera_controller.h"
#include "camera_fx.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/control.hpp>

CameraController::CameraController() {
    fade = nullptr;
    camerafx = nullptr;
    zoom = Vector2(1, 1);
    position_smoothing_enabled = false;

    limit_left = -1000000000;
    limit_top = -1000000000;
    limit_right = 1000000000;
    limit_bottom = 1000000000;
}

CameraController::~CameraController() {}

void CameraController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("force_update"), &CameraController::force_update);
    ClassDB::bind_method(D_METHOD("get_global_camera"), &CameraController::get_global_camera);

    ClassDB::bind_method(D_METHOD("_on_timer_timeout"), &CameraController::_on_timer_timeout);
    ClassDB::bind_method(D_METHOD("_set_limits"), &CameraController::_set_limits);

    ClassDB::bind_method(D_METHOD("set_zoom", "zoom"), &CameraController::set_zoom);
    ClassDB::bind_method(D_METHOD("get_zoom"), &CameraController::get_zoom);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "zoom"), "set_zoom", "get_zoom");
    
    ClassDB::bind_method(D_METHOD("set_position_smoothing_enabled", "enabled"), &CameraController::set_position_smoothing_enabled);
    ClassDB::bind_method(D_METHOD("get_position_smoothing_enabled"), &CameraController::get_position_smoothing_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "position_smoothing_enabled"), "set_position_smoothing_enabled", "get_position_smoothing_enabled");

    ClassDB::bind_method(D_METHOD("set_limit_left", "limit"), &CameraController::set_limit_left);
    ClassDB::bind_method(D_METHOD("get_limit_left"), &CameraController::get_limit_left);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_left"), "set_limit_left", "get_limit_left");
    
    ClassDB::bind_method(D_METHOD("set_limit_top", "limit"), &CameraController::set_limit_top);
    ClassDB::bind_method(D_METHOD("get_limit_top"), &CameraController::get_limit_top);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_top"), "set_limit_top", "get_limit_top");
    
    ClassDB::bind_method(D_METHOD("set_limit_right", "limit"), &CameraController::set_limit_right);
    ClassDB::bind_method(D_METHOD("get_limit_right"), &CameraController::get_limit_right);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_right"), "set_limit_right", "get_limit_right");
    
    ClassDB::bind_method(D_METHOD("set_limit_bottom", "limit"), &CameraController::set_limit_bottom);
    ClassDB::bind_method(D_METHOD("get_limit_bottom"), &CameraController::get_limit_bottom);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_bottom"), "set_limit_bottom", "get_limit_bottom");
}

void CameraController::_ready() {
    force_update();
    _set_limits();
    
    fade = memnew(ColorRect);
    Color fade_color = Color(0, 0, 0, 0);
    fade->set_color(fade_color);
    fade->set_anchors_preset(Control::PRESET_FULL_RECT);
    fade->set_z_index(10);
    add_child(fade);
    
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(2, false);
    timer->connect("timeout", Callable(this, "_on_timer_timeout"));
}

void CameraController::force_update() {
    camerafx = global->get_scene_container()->get_camera();
    set_remote_node(get_path_to(camerafx, true));
    camerafx->set_position_smoothing_enabled(position_smoothing_enabled);
    force_update_cache();
}

void CameraController::_process(double delta) {
    if(!camerafx) return;
    camerafx->set_zoom(zoom);
}

void CameraController::_on_timer_timeout() {
    if (!fade) return;
    
    Ref<Tween> tween = create_tween();
    tween->tween_property(fade, "color:a", 0.0, 0.5);
    
    tween->connect("finished", Callable(fade, "queue_free"));
}

void CameraController::_set_limits() {
    camerafx = global->get_scene_container()->get_camera();
    camerafx->set_limit(Side::SIDE_LEFT, limit_left);
    camerafx->set_limit(Side::SIDE_TOP, limit_top);
    camerafx->set_limit(Side::SIDE_RIGHT, limit_right);
    camerafx->set_limit(Side::SIDE_BOTTOM, limit_bottom);
}

void CameraController::set_zoom(const Vector2& p_zoom) {
    zoom = p_zoom;
}

Vector2 CameraController::get_zoom() const {
    return zoom;
}

void CameraController::set_position_smoothing_enabled(bool p_enabled) {
    position_smoothing_enabled = p_enabled;
}

bool CameraController::get_position_smoothing_enabled() const {
    return position_smoothing_enabled;
}

void CameraController::set_limit_left(int p_limit) {
    limit_left = p_limit;
    if(!isEditor) _set_limits();
}

int CameraController::get_limit_left() const {
    return limit_left;
}

void CameraController::set_limit_top(int p_limit) {
    limit_top = p_limit;
    if(!isEditor) _set_limits();
}

int CameraController::get_limit_top() const {
    return limit_top;
}

void CameraController::set_limit_right(int p_limit) {
    limit_right = p_limit;
    if(!isEditor) _set_limits();
}

int CameraController::get_limit_right() const {
    return limit_right;
}

void CameraController::set_limit_bottom(int p_limit) {
    limit_bottom = p_limit;
    if(!isEditor) _set_limits();
}

int CameraController::get_limit_bottom() const {
    return limit_bottom;
}

CameraFx* CameraController::get_global_camera() {
    return camerafx;
}