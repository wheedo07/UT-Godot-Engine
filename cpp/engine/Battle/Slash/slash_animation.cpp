#include "slash_animation.h"
using namespace godot;

SlashAnimation::SlashAnimation() {}

SlashAnimation::~SlashAnimation() {}

void SlashAnimation::_bind_methods() {
    ADD_SIGNAL(MethodInfo("started"));
    ADD_SIGNAL(MethodInfo("finished"));

    ClassDB::bind_method(D_METHOD("set_crit", "value"), &SlashAnimation::set_crit);
    ClassDB::bind_method(D_METHOD("get_crit"), &SlashAnimation::get_crit);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "crit", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_crit", "get_crit");

    ClassDB::bind_method(D_METHOD("set_dmg_mult", "value"), &SlashAnimation::set_dmg_mult);
    ClassDB::bind_method(D_METHOD("get_dmg_mult"), &SlashAnimation::get_dmg_mult);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dmg_mult", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_dmg_mult", "get_dmg_mult");
}

void SlashAnimation::set_crit(bool value) {
    crit = value;
}

bool SlashAnimation::get_crit() {
    return crit;
}

void SlashAnimation::set_dmg_mult(float value) {
    dmg_mult = value;
}

float SlashAnimation::get_dmg_mult() {
    return dmg_mult;
}