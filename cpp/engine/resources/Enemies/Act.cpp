#include "Act.h"
#include<godot_cpp/core/class_db.hpp>
using namespace godot;

ActInfo::ActInfo() {
    act = String::utf8("* 살펴보기");
    description.push_back(String::utf8("* ENEMY - ATK 1 DEF 1 \n* 정보 몰루?."));
}

ActInfo::~ActInfo() {}

void ActInfo::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_act", "act"), &ActInfo::set_act);
    ClassDB::bind_method(D_METHOD("get_act"), &ActInfo::get_act);
    ClassDB::bind_method(D_METHOD("set_description", "description"), &ActInfo::set_description);
    ClassDB::bind_method(D_METHOD("get_description"), &ActInfo::get_description);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "act"), "set_act", "get_act");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "description", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT)),
    "set_description", "get_description");
}

void ActInfo::set_act(const String& p_act) {
    act = p_act;
}

String ActInfo::get_act() const {
    return act;
}

void ActInfo::set_description(const PackedStringArray& p_description) {
    description = p_description;
}

PackedStringArray ActInfo::get_description() const {
    return description;
}