#include "item_slider.h"
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

ItemSlider::ItemSlider() {
    value = 0;
    offset = 0;
    step_size = 1.0f;
}

ItemSlider::~ItemSlider() {}

void ItemSlider::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_value", "p_value"), &ItemSlider::set_value);
    ClassDB::bind_method(D_METHOD("get_value"), &ItemSlider::get_value);
    ClassDB::bind_method(D_METHOD("set_offset", "p_offset"), &ItemSlider::set_offset);
    ClassDB::bind_method(D_METHOD("get_offset"), &ItemSlider::get_offset);
    ClassDB::bind_method(D_METHOD("set_step_size", "p_step_size"), &ItemSlider::set_step_size);
    ClassDB::bind_method(D_METHOD("get_step_size"), &ItemSlider::get_step_size);
    ClassDB::bind_method(D_METHOD("set_grabber_path", "p_grabber_path"), &ItemSlider::set_grabber_path);
    ClassDB::bind_method(D_METHOD("get_grabber_path"), &ItemSlider::get_grabber_path);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "offset"), "set_offset", "get_offset");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "step_size"), "set_step_size", "get_step_size");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "grabber"), "set_grabber_path", "get_grabber_path");
}

void ItemSlider::_ready() {
    if (!grabber_path.is_empty()) {
        grabber_node = get_node_internal(grabber_path);
    }else ERR_PRINT("grabber_path 값 비어있음!");
}

void ItemSlider::_process(double delta) {
    if (grabber_node) {
        Vector2 current_pos = grabber_node->call("get_position");
        float target_y = offset + step_size * value;
        float new_y = UtilityFunctions::lerp(current_pos.y, target_y, float(delta * speed));
        
        Vector2 new_pos = current_pos;
        new_pos.y = new_y;
        grabber_node->call("set_position", new_pos);
    }
}

void ItemSlider::set_value(int p_value) {
    value = p_value;
}

int ItemSlider::get_value() const {
    return value;
}

void ItemSlider::set_offset(float p_offset) {
    offset = p_offset;
}

float ItemSlider::get_offset() const {
    return offset;
}

void ItemSlider::set_step_size(float p_step_size) {
    step_size = p_step_size;
}

float ItemSlider::get_step_size() const {
    return step_size;
}

void ItemSlider::set_grabber_path(const NodePath& p_grabber_path) {
    grabber_path = p_grabber_path;
}

NodePath ItemSlider::get_grabber_path() const {
    return grabber_path;
}