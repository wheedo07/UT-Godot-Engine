#include "button_set.h"
using namespace godot;

ButtonSet::ButtonSet() {
    soul_offset = Vector2(38, 0);
}

ButtonSet::~ButtonSet() {}

void ButtonSet::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_fight_button", "frames"), &ButtonSet::set_fight_button);
    ClassDB::bind_method(D_METHOD("get_fight_button"), &ButtonSet::get_fight_button);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fight_button", PROPERTY_HINT_RESOURCE_TYPE, "SpriteFrames"), "set_fight_button", "get_fight_button");

    ClassDB::bind_method(D_METHOD("set_act_button", "frames"), &ButtonSet::set_act_button);
    ClassDB::bind_method(D_METHOD("get_act_button"), &ButtonSet::get_act_button);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "act_button", PROPERTY_HINT_RESOURCE_TYPE, "SpriteFrames"), "set_act_button", "get_act_button");

    ClassDB::bind_method(D_METHOD("set_item_button", "frames"), &ButtonSet::set_item_button);
    ClassDB::bind_method(D_METHOD("get_item_button"), &ButtonSet::get_item_button);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_button", PROPERTY_HINT_RESOURCE_TYPE, "SpriteFrames"), "set_item_button", "get_item_button");

    ClassDB::bind_method(D_METHOD("set_mercy_button", "frames"), &ButtonSet::set_mercy_button);
    ClassDB::bind_method(D_METHOD("get_mercy_button"), &ButtonSet::get_mercy_button);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mercy_button", PROPERTY_HINT_RESOURCE_TYPE, "SpriteFrames"), "set_mercy_button", "get_mercy_button");

    ClassDB::bind_method(D_METHOD("set_soul_offset", "offset"), &ButtonSet::set_soul_offset);
    ClassDB::bind_method(D_METHOD("get_soul_offset"), &ButtonSet::get_soul_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "soul_offset"), "set_soul_offset", "get_soul_offset");
}

void ButtonSet::set_fight_button(const Ref<SpriteFrames>& p_frames) {
    fight_button = p_frames;
}

Ref<SpriteFrames> ButtonSet::get_fight_button() const {
    return fight_button;
}

void ButtonSet::set_act_button(const Ref<SpriteFrames>& p_frames) {
    act_button = p_frames;
}

Ref<SpriteFrames> ButtonSet::get_act_button() const {
    return act_button;
}

void ButtonSet::set_item_button(const Ref<SpriteFrames>& p_frames) {
    item_button = p_frames;
}

Ref<SpriteFrames> ButtonSet::get_item_button() const {
    return item_button;
}

void ButtonSet::set_mercy_button(const Ref<SpriteFrames>& p_frames) {
    mercy_button = p_frames;
}

Ref<SpriteFrames> ButtonSet::get_mercy_button() const {
    return mercy_button;
}

void ButtonSet::set_soul_offset(const Vector2& p_offset) {
    soul_offset = p_offset;
}

Vector2 ButtonSet::get_soul_offset() const {
    return soul_offset;
}