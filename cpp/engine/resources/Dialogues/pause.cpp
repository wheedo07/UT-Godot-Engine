#include "pause.h"
using namespace godot;

DialoguePause::DialoguePause() {
    pause_index = 0;
    pause_duration = 0.1;
}

DialoguePause::~DialoguePause() {}

void DialoguePause::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_pause_index", "index"), &DialoguePause::set_pause_index);
    ClassDB::bind_method(D_METHOD("get_pause_index"), &DialoguePause::get_pause_index);
    ClassDB::bind_method(D_METHOD("set_pause_duration", "duration"), &DialoguePause::set_pause_duration);
    ClassDB::bind_method(D_METHOD("get_pause_duration"), &DialoguePause::get_pause_duration);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "pause_index"), "set_pause_index", "get_pause_index");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pause_duration"), "set_pause_duration", "get_pause_duration");
}

void DialoguePause::set_pause_index(int p_index) {
    pause_index = p_index;
}

int DialoguePause::get_pause_index() const {
    return pause_index;
}

void DialoguePause::set_pause_duration(float p_duration) {
    pause_duration = p_duration;
}

float DialoguePause::get_pause_duration() const {
    return pause_duration;
}