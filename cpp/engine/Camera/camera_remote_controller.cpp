#include "camera_remote_controller.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/control.hpp>

CameraRemoteController::CameraRemoteController() {
    fade = nullptr;
    zoom = Vector2(1, 1);
    position_smoothing_enabled = true;

    limit_left = -1000000000;
    limit_top = -1000000000;
    limit_right = 1000000000;
    limit_bottom = 1000000000;
}

CameraRemoteController::~CameraRemoteController() {}

void CameraRemoteController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_set_limits"), &CameraRemoteController::_set_limits);
    ClassDB::bind_method(D_METHOD("force_update"), &CameraRemoteController::force_update);
    ClassDB::bind_method(D_METHOD("add_shake", "amount"), &CameraRemoteController::add_shake);
    ClassDB::bind_method(D_METHOD("_on_timer_timeout"), &CameraRemoteController::_on_timer_timeout);
    
    ClassDB::bind_method(D_METHOD("set_zoom", "zoom"), &CameraRemoteController::set_zoom);
    ClassDB::bind_method(D_METHOD("get_zoom"), &CameraRemoteController::get_zoom);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "zoom"), "set_zoom", "get_zoom");
    
    ClassDB::bind_method(D_METHOD("set_position_smoothing_enabled", "enabled"), &CameraRemoteController::set_position_smoothing_enabled);
    ClassDB::bind_method(D_METHOD("get_position_smoothing_enabled"), &CameraRemoteController::get_position_smoothing_enabled);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "position_smoothing_enabled"), "set_position_smoothing_enabled", "get_position_smoothing_enabled");

    ClassDB::bind_method(D_METHOD("set_limit_left", "limit"), &CameraRemoteController::set_limit_left);
    ClassDB::bind_method(D_METHOD("get_limit_left"), &CameraRemoteController::get_limit_left);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_left"), "set_limit_left", "get_limit_left");
    
    ClassDB::bind_method(D_METHOD("set_limit_top", "limit"), &CameraRemoteController::set_limit_top);
    ClassDB::bind_method(D_METHOD("get_limit_top"), &CameraRemoteController::get_limit_top);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_top"), "set_limit_top", "get_limit_top");
    
    ClassDB::bind_method(D_METHOD("set_limit_right", "limit"), &CameraRemoteController::set_limit_right);
    ClassDB::bind_method(D_METHOD("get_limit_right"), &CameraRemoteController::get_limit_right);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_right"), "set_limit_right", "get_limit_right");
    
    ClassDB::bind_method(D_METHOD("set_limit_bottom", "limit"), &CameraRemoteController::set_limit_bottom);
    ClassDB::bind_method(D_METHOD("get_limit_bottom"), &CameraRemoteController::get_limit_bottom);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "limit_bottom"), "set_limit_bottom", "get_limit_bottom");
}

void CameraRemoteController::_ready() {
    if(isEditor) return;
    force_update();
    _set_limits();
    
    fade = memnew(ColorRect);
    Color fade_color = Color(0, 0, 0, 0);
    fade->set_color(fade_color);
    fade->set_anchors_preset(Control::PRESET_FULL_RECT);
    fade->set_z_index(10);
    add_child(fade);
    
    SceneTree* tree = get_tree();
    if (tree) {
        Ref<SceneTreeTimer> timer = tree->create_timer(2.0);
        timer->connect("timeout", Callable(this, "_on_timer_timeout"));
    }
}

void CameraRemoteController::force_update() {
    CameraFx* camera = global->get_scene_container()->get_camera();
    set_remote_node(get_path_to(camera, true));
    camera->set_position_smoothing_enabled(position_smoothing_enabled);
    force_update_cache();
}

void CameraRemoteController::_process(double delta) {
    if(isEditor) return;
    CameraFx* camera = global->get_scene_container()->get_camera();
    if (camera) {
        camera->set_zoom(zoom);
    }
}

void CameraRemoteController::_on_timer_timeout() {
    if (!fade) return;
    
    Ref<Tween> tween = create_tween();
    tween->tween_property(fade, "color:a", 0.0, 0.5);
    
    tween->connect("finished", Callable(fade, "queue_free"));
}

void CameraRemoteController::_set_limits() {
    CameraFx* camera = global->get_scene_container()->get_camera();
    if (camera) {
        camera->set_limit(Side::SIDE_LEFT, limit_left);
        camera->set_limit(Side::SIDE_TOP, limit_top);
        camera->set_limit(Side::SIDE_RIGHT, limit_right);
        camera->set_limit(Side::SIDE_BOTTOM, limit_bottom);
    }
}

void CameraRemoteController::add_shake(float amount) {
    CameraFx* camera = global->get_scene_container()->get_camera();
    if(camera) camera->add_shake(amount);
}

void CameraRemoteController::set_zoom(const Vector2& p_zoom) {
    zoom = p_zoom;
}

Vector2 CameraRemoteController::get_zoom() const {
    return zoom;
}

void CameraRemoteController::set_position_smoothing_enabled(bool p_enabled) {
    position_smoothing_enabled = p_enabled;
}

bool CameraRemoteController::get_position_smoothing_enabled() const {
    return position_smoothing_enabled;
}

void CameraRemoteController::set_limit_left(int p_limit) {
    limit_left = p_limit;
}

int CameraRemoteController::get_limit_left() const {
    return limit_left;
}

void CameraRemoteController::set_limit_top(int p_limit) {
    limit_top = p_limit;
}

int CameraRemoteController::get_limit_top() const {
    return limit_top;
}

void CameraRemoteController::set_limit_right(int p_limit) {
    limit_right = p_limit;
}

int CameraRemoteController::get_limit_right() const {
    return limit_right;
}

void CameraRemoteController::set_limit_bottom(int p_limit) {
    limit_bottom = p_limit;
}

int CameraRemoteController::get_limit_bottom() const {
    return limit_bottom;
}