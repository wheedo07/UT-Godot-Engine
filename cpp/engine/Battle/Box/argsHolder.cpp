#include "argsHolder.h"
using namespace godot;

void ArgsHolder::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_duration", "duration"), &ArgsHolder::set_duration);
    ClassDB::bind_method(D_METHOD("get_duration"), &ArgsHolder::get_duration);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "duration"), "set_duration", "get_duration");
}

void ArgsHolder::set_duration(float p_duration) { 
    duration = p_duration; 
}

float ArgsHolder::get_duration() const { 
    return duration; 
}