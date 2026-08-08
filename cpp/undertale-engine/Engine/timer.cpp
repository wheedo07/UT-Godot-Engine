#include "timer.h"
using namespace godot;

void UTGETimer::_bind_methods() {
    ADD_SIGNAL(MethodInfo("timeout"));
    ClassDB::bind_method(D_METHOD("frame"), &UTGETimer::frame);
    ClassDB::bind_method(D_METHOD("is_frame_based"), &UTGETimer::is_frame_based);

    ClassDB::bind_method(D_METHOD("set_time_left", "time"), &UTGETimer::set_time_left);
    ClassDB::bind_method(D_METHOD("get_time_left"), &UTGETimer::get_time_left);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_left"), "set_time_left", "get_time_left");

    ClassDB::bind_method(D_METHOD("set_paused", "paused"), &UTGETimer::set_paused);
    ClassDB::bind_method(D_METHOD("is_paused"), &UTGETimer::is_paused);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "paused"), "set_paused", "is_paused");

    ClassDB::bind_method(D_METHOD("set_speed_scale", "scale"), &UTGETimer::set_speed_scale);
    ClassDB::bind_method(D_METHOD("get_speed_scale"), &UTGETimer::get_speed_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed_scale"), "set_speed_scale", "get_speed_scale");
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

void UTGETimer::set_process_in_physics(bool p_enabled) {
    process_in_physics = p_enabled;
}

bool UTGETimer::is_processing_in_physics() const {
    return process_in_physics;
}

void UTGETimer::set_ignore_time_scale(bool p_enabled) {
    ignore_time_scale = p_enabled;
}

void UTGETimer::set_process_always(bool p_enabled) {
    process_always = p_enabled;
}

bool UTGETimer::is_finished() const {
    return finished;
}

void UTGETimer::set_time_left(double p_time) {
    time_left = p_time < 0.0 ? 0.0 : p_time;
    finished = false;
}

double UTGETimer::get_time_left() const {
    return time_left;
}

void UTGETimer::set_paused(bool p_paused) {
    paused = p_paused;
}

bool UTGETimer::is_paused() const {
    return paused;
}

void UTGETimer::set_speed_scale(double p_scale) {
    speed_scale = p_scale;
}

double UTGETimer::get_speed_scale() const {
    return speed_scale;
}