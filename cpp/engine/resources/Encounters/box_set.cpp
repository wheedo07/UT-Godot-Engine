#include "box_set.h"
using namespace godot;

BoxSet::BoxSet() {
    board_color = Color(0, 0, 0, 0.8);
    board_border_color = Color(1, 1, 1, 1);
    meter_scale = Vector2(1, 1);
}

BoxSet::~BoxSet() {}

void BoxSet::_bind_methods() {
    ADD_GROUP("colors", "board_");
    ClassDB::bind_method(D_METHOD("set_board_color", "color"), &BoxSet::set_board_color);
    ClassDB::bind_method(D_METHOD("get_board_color"), &BoxSet::get_board_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "board_color"), "set_board_color", "get_board_color");
    
    ClassDB::bind_method(D_METHOD("set_board_border_color", "color"), &BoxSet::set_board_border_color);
    ClassDB::bind_method(D_METHOD("get_board_border_color"), &BoxSet::get_board_border_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "board_border_color"), "set_board_border_color", "get_board_border_color");

    ADD_GROUP("meters", "meter_");
    ClassDB::bind_method(D_METHOD("set_meter_texture", "texture"), &BoxSet::set_meter_texture);
    ClassDB::bind_method(D_METHOD("get_meter_texture"), &BoxSet::get_meter_texture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "meter_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_meter_texture", "get_meter_texture");
    ClassDB::bind_method(D_METHOD("set_meter_scale", "scale"), &BoxSet::set_meter_scale);
    ClassDB::bind_method(D_METHOD("get_meter_scale"), &BoxSet::get_meter_scale);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "meter_scale"), "set_meter_scale", "get_meter_scale");
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