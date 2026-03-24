#include "box_set.h"
using namespace godot;

BoxSet::BoxSet() {
    isCustomMeter = false;
    board_color = Color(0, 0, 0, 0.8);
    board_border_color = Color(1, 1, 1, 1);
    meter_scale = Vector2(1, 1);
}

BoxSet::~BoxSet() {}

void BoxSet::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_is_custom_meter", "is_custom"), &BoxSet::set_is_custom_meter);
    ClassDB::bind_method(D_METHOD("is_custom_meter"), &BoxSet::is_custom_meter);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_custom_meter"), "set_is_custom_meter", "is_custom_meter");

    ADD_GROUP("colors", "board_");
    ClassDB::bind_method(D_METHOD("set_board_color", "color"), &BoxSet::set_board_color);
    ClassDB::bind_method(D_METHOD("get_board_color"), &BoxSet::get_board_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "board_color"), "set_board_color", "get_board_color");
    
    ClassDB::bind_method(D_METHOD("set_board_border_color", "color"), &BoxSet::set_board_border_color);
    ClassDB::bind_method(D_METHOD("get_board_border_color"), &BoxSet::get_board_border_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "board_border_color"), "set_board_border_color", "get_board_border_color");
}

void BoxSet::_get_property_list(List<PropertyInfo> *p_list) const {
    if(isCustomMeter) {
        p_list->push_back(PropertyInfo(Variant::NIL, "Custom Meter", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));
        p_list->push_back(PropertyInfo(Variant::OBJECT, "meter_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"));
    }else {
        p_list->push_back(PropertyInfo(Variant::NIL, "Default Meter", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));
        p_list->push_back(PropertyInfo(Variant::OBJECT, "meter_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"));
        p_list->push_back(PropertyInfo(Variant::VECTOR2, "meter_scale"));
    }
}

bool BoxSet::_set(const StringName& p_name, const Variant& p_value) {
    String name = p_name;
    if(name == "meter_scene") {
        meter_scene = p_value;
        return true;
    }else if(name == "meter_texture") {
        meter_texture = p_value;
        return true;
    }else if(name == "meter_scale") {
        meter_scale = p_value;
        return true;
    }
    return false;
}

bool BoxSet::_get(const StringName& p_name, Variant& r_ret) {
    String name = p_name;
    if(name == "meter_scene") {
        r_ret = meter_scene;
        return true;
    }else if(name == "meter_texture") {
        r_ret = meter_texture;
        return true;
    }else if(name == "meter_scale") {
        r_ret = meter_scale;
        return true;
    }
    return false;
}

void BoxSet::set_is_custom_meter(bool p_is_custom) {
    isCustomMeter = p_is_custom;
    notify_property_list_changed();
}

bool BoxSet::is_custom_meter() const {
    return isCustomMeter;
}

void BoxSet::set_board_color(const Color& p_color) {
    board_color = p_color;
}

Color BoxSet::get_board_color() const {
    return board_color;
}

void BoxSet::set_board_border_color(const Color& p_color) {
    board_border_color = p_color;
}

Color BoxSet::get_board_border_color() const {
    return board_border_color;
}

void BoxSet::set_meter_scene(const Ref<PackedScene>& p_scene) {
    meter_scene = p_scene;
}

Ref<PackedScene> BoxSet::get_meter_scene() const {
    return meter_scene;
}

void BoxSet::set_meter_texture(const Ref<Texture>& p_texture) {
    meter_texture = p_texture;
}

Ref<Texture> BoxSet::get_meter_texture() const {
    return meter_texture;
}

void BoxSet::set_meter_scale(const Vector2& p_scale) {
    meter_scale = p_scale;
}

Vector2 BoxSet::get_meter_scale() const {
    return meter_scale;
}