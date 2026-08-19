#include "timer.h"
using namespace godot;

void UTGETimer::_bind_methods() {
    ADD_SIGNAL(MethodInfo("timeout"));

    /* API 메서드 */
    ClassDB::bind_method(D_METHOD("frame"), &UTGETimer::frame);
    ClassDB::bind_method(D_METHOD("is_frame_based"), &UTGETimer::is_frame_based);

    /* 공개 속성 */
    ClassDB::bind_method(D_METHOD("set_time_left", "value"), &UTGETimer::set_time_left);
    ClassDB::bind_method(D_METHOD("get_time_left"), &UTGETimer::get_time_left);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_left"), "set_time_left", "get_time_left");

    ClassDB::bind_method(D_METHOD("set_paused", "value"), &UTGETimer::set_paused);
    ClassDB::bind_method(D_METHOD("is_paused"), &UTGETimer::is_paused);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "paused"), "set_paused", "is_paused");

    ClassDB::bind_method(D_METHOD("set_speed_scale", "value"), &UTGETimer::set_speed_scale);
    ClassDB::bind_method(D_METHOD("get_speed_scale"), &UTGETimer::get_speed_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed_scale"), "set_speed_scale", "get_speed_scale");

    ClassDB::bind_method(D_METHOD("set_layer_id", "value"), &UTGETimer::set_layer_id);
    ClassDB::bind_method(D_METHOD("get_layer_id"), &UTGETimer::get_layer_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "layer_id"), "set_layer_id", "get_layer_id");
}

bool UTGETimer::process(double p_delta, double p_tree_time_scale, bool p_tree_paused) {
    if(finished || paused) return finished;
    if(p_tree_paused && !process_always) return false;

    if(frame_based) {
        time_left -= 1;
    }else {
        double scale = speed_scale;
        if(!ignore_time_scale) {
            scale *= p_tree_time_scale;
        }

        time_left -= p_delta * scale;
    }
    if(time_left > 0) return false;

    time_left = 0;
    finished = true;
    emit_signal("timeout");
    return true;
}

Ref<UTGETimer> UTGETimer::frame() {
    frame_based = true;
    return this;
}

bool UTGETimer::is_frame_based() const {
    return frame_based;
}

void UTGETimer::set_process_in_physics(bool value) {
    process_in_physics = value;
}

bool UTGETimer::is_processing_in_physics() const {
    return process_in_physics;
}

void UTGETimer::set_ignore_time_scale(bool value) {
    ignore_time_scale = value;
}

void UTGETimer::set_process_always(bool value) {
    process_always = value;
}

bool UTGETimer::is_process_always() const {
    return process_always;
}

bool UTGETimer::is_finished() const {
    return finished;
}

void UTGETimer::set_time_left(double value) {
    time_left = value < 0.0 ? 0.0 : value;
    finished = false;
}

double UTGETimer::get_time_left() const {
    return time_left;
}

void UTGETimer::set_paused(bool value) {
    paused = value;
}

bool UTGETimer::is_paused() const {
    return paused;
}

void UTGETimer::set_speed_scale(double value) {
    speed_scale = value;
}

double UTGETimer::get_speed_scale() const {
    return speed_scale;
}

void UTGETimer::set_layer_id(StringName value) {
    layer_id = value;
}

StringName UTGETimer::get_layer_id() const {
    return layer_id;
}