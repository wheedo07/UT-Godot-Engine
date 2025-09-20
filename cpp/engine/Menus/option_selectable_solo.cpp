#include "option_selectable_solo.h"
#include "env.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/viewport.hpp>

OptionSelectableSolo::OptionSelectableSolo() {
    enabled = true;
    soul_offset = Vector2(0, 0);
    node_up = NodePath();
    node_down = NodePath();
    node_left = NodePath();
    node_right = NodePath();
    node_accept = NodePath();
}

OptionSelectableSolo::~OptionSelectableSolo() {}

void OptionSelectableSolo::_bind_methods() {
    ClassDB::bind_method(D_METHOD("enable"), &OptionSelectableSolo::enable);
    ClassDB::bind_method(D_METHOD("disable"), &OptionSelectableSolo::disable);

    ClassDB::bind_method(D_METHOD("set_soul_offset", "offset"), &OptionSelectableSolo::set_soul_offset);
    ClassDB::bind_method(D_METHOD("get_soul_offset"), &OptionSelectableSolo::get_soul_offset);

    ClassDB::bind_method(D_METHOD("set_node_up", "node"), &OptionSelectableSolo::set_node_up);
    ClassDB::bind_method(D_METHOD("get_node_up"), &OptionSelectableSolo::get_node_up);
    
    ClassDB::bind_method(D_METHOD("set_node_down", "node"), &OptionSelectableSolo::set_node_down);
    ClassDB::bind_method(D_METHOD("get_node_down"), &OptionSelectableSolo::get_node_down);
    
    ClassDB::bind_method(D_METHOD("set_node_left", "node"), &OptionSelectableSolo::set_node_left);
    ClassDB::bind_method(D_METHOD("get_node_left"), &OptionSelectableSolo::get_node_left);
    
    ClassDB::bind_method(D_METHOD("set_node_right", "node"), &OptionSelectableSolo::set_node_right);
    ClassDB::bind_method(D_METHOD("get_node_right"), &OptionSelectableSolo::get_node_right);
    
    ClassDB::bind_method(D_METHOD("set_node_accept", "node"), &OptionSelectableSolo::set_node_accept);
    ClassDB::bind_method(D_METHOD("get_node_accept"), &OptionSelectableSolo::get_node_accept);

    ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &OptionSelectableSolo::set_enabled);
    ClassDB::bind_method(D_METHOD("get_enabled"), &OptionSelectableSolo::get_enabled);
    
    // 프로퍼티 등록
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "soul_offset"), "set_soul_offset", "get_soul_offset");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "get_enabled");
    
    ADD_GROUP("Surrounding Options", "");
    
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "node_up", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "OptionSelectable"), "set_node_up", "get_node_up");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "node_down", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "OptionSelectable"), "set_node_down", "get_node_down");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "node_left", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "OptionSelectable"), "set_node_left", "get_node_left");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "node_right", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "OptionSelectable"), "set_node_right", "get_node_right");
    
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "node_accept", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "OptionSelectable"), "set_node_accept", "get_node_accept");
    
    ADD_SIGNAL(MethodInfo("accept_pressed"));
    ADD_SIGNAL(MethodInfo("move_soul_request", PropertyInfo(Variant::VECTOR2, "pos")));
}

void OptionSelectableSolo::_ready() {
    if(isEditor) return;
    OptionSelectable::_ready();
    set_selected(get_initial_selected());
    
    if(!get_selected()) return;
    move_soul(this);
}

void OptionSelectableSolo::_unhandled_input(const Ref<InputEvent>& event) {
    if (isEditor || !is_visible_in_tree() || !enabled) return;
    
    if (event->is_action_pressed("ui_down") && !node_down.is_empty()) {
        move_soul(Object::cast_to<OptionSelectable>(get_node_internal(node_down)));
        audio_player->play("choice");
    }
    
    if (event->is_action_pressed("ui_up") && !node_up.is_empty()) {
        move_soul(Object::cast_to<OptionSelectable>(get_node_internal(node_up)));
        audio_player->play("choice");
    }
    
    if (event->is_action_pressed("ui_left") && !node_left.is_empty()) {
        move_soul(Object::cast_to<OptionSelectable>(get_node_internal(node_left)));
        audio_player->play("choice");
    }
    
    if (event->is_action_pressed("ui_right") && !node_right.is_empty()) {
        move_soul(Object::cast_to<OptionSelectable>(get_node_internal(node_right)));
        audio_player->play("choice");
    }
    
    if (event->is_action_pressed("ui_accept")) {
        if(!node_accept.is_empty()) move_soul(Object::cast_to<OptionSelectable>(get_node_internal(node_accept)));
        audio_player->play("select");
        emit_signal("accept_pressed");
    }
}

void OptionSelectableSolo::set_selected(bool new_val) {
    OptionSelectable::set_selected(new_val);
    set_process_unhandled_input(new_val);
    
    if (new_val) {
        set_self_modulate(get_selected_color());
        return;
    }
    
    set_self_modulate(get_default_color());
}

void OptionSelectableSolo::reset() {
    set_selected(false);
}

void OptionSelectableSolo::enable() {
    enabled = true;
    set_selected(true);
}

void OptionSelectableSolo::disable() {
    enabled = false;
    set_selected(false);
}

void OptionSelectableSolo::move_soul(OptionSelectable* node) {
    get_viewport()->set_input_as_handled();
    
    emit_signal("move_soul_request", node->get_global_position() + soul_offset);
    
    set_selected(false);
    node->set_selected(true);
}

void OptionSelectableSolo::set_node_up(NodePath p_node) {
    node_up = p_node;
}

NodePath OptionSelectableSolo::get_node_up() const {
    return node_up;
}

void OptionSelectableSolo::set_node_down(NodePath p_node) {
    node_down = p_node;
}

NodePath OptionSelectableSolo::get_node_down() const {
    return node_down;
}

void OptionSelectableSolo::set_node_left(NodePath p_node) {
    node_left = p_node;
}

NodePath OptionSelectableSolo::get_node_left() const {
    return node_left;
}

void OptionSelectableSolo::set_node_right(NodePath p_node) {
    node_right = p_node;
}

NodePath OptionSelectableSolo::get_node_right() const {
    return node_right;
}

void OptionSelectableSolo::set_node_accept(NodePath p_node) {
    node_accept = p_node;
}

NodePath OptionSelectableSolo::get_node_accept() const {
    return node_accept;
}

void OptionSelectableSolo::set_soul_offset(const Vector2& p_offset) {
    soul_offset = p_offset;
}

Vector2 OptionSelectableSolo::get_soul_offset() const {
    return soul_offset;
}

void OptionSelectableSolo::set_enabled(bool p_enabled) {
    enabled = p_enabled;
}

bool OptionSelectableSolo::get_enabled() const {
    return enabled;
}