#include "option_selectable.h"
#include "env.h"
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

OptionSelectable::OptionSelectable() {
    selected = false;
    Selected = false;
    selected_color = Color(1, 1, 0); // Color.YELLOW
}

OptionSelectable::~OptionSelectable() {}

void OptionSelectable::_bind_methods() {
    ClassDB::bind_method(D_METHOD("reset"), &OptionSelectable::reset);

    ClassDB::bind_method(D_METHOD("set_selected", "new_val"), &OptionSelectable::set_selected);
    ClassDB::bind_method(D_METHOD("get_selected"), &OptionSelectable::get_selected);
    
    ClassDB::bind_method(D_METHOD("set_initial_selected", "selected"), &OptionSelectable::set_initial_selected);
    ClassDB::bind_method(D_METHOD("get_initial_selected"), &OptionSelectable::get_initial_selected);
    
    ClassDB::bind_method(D_METHOD("set_selected_color", "color"), &OptionSelectable::set_selected_color);
    ClassDB::bind_method(D_METHOD("get_selected_color"), &OptionSelectable::get_selected_color);
    
    // 프로퍼티 등록
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "selected_color"), "set_selected_color", "get_selected_color");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "selected"), "set_selected", "get_selected");
}

void OptionSelectable::_ready() {
    if(isEditor) return;
    default_color = get_self_modulate();
    
    set_selected(Selected);
}

void OptionSelectable::set_selected(bool new_val) {
    selected = new_val;
    
    if(new_val && !isEditor) {
        set_self_modulate(selected_color);
    } else {
        set_self_modulate(default_color);
    }
}

bool OptionSelectable::get_selected() const {
    return selected;
}

void OptionSelectable::set_initial_selected(bool p_selected) {
    Selected = p_selected;
}

bool OptionSelectable::get_initial_selected() const {
    return Selected;
}

void OptionSelectable::set_selected_color(const Color& p_color) {
    selected_color = p_color;
    
    if(selected && !isEditor) {
        set_self_modulate(selected_color);
    }
}

Color OptionSelectable::get_selected_color() const {
    return selected_color;
}

void OptionSelectable::reset() {
    set_selected(false);
}

Color OptionSelectable::get_default_color() const {
    return default_color;
}