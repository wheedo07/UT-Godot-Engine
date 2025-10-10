#include "character_setting.h"
using namespace godot;

CharacterSetting::CharacterSetting() {
    text_size = 28;
    extra_delay = "@#$%^&+=_-~`<>\"|\\*{}()[].,!?";
    no_sound = "@#$%^&+=_-~`<>\"|\\*{}()[].,!? ";
}

CharacterSetting::~CharacterSetting() {}

void CharacterSetting::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_font", "value"), &CharacterSetting::set_font);
    ClassDB::bind_method(D_METHOD("get_font"), &CharacterSetting::get_font);
    ClassDB::bind_method(D_METHOD("set_text_size", "value"), &CharacterSetting::set_text_size);
    ClassDB::bind_method(D_METHOD("get_text_size"), &CharacterSetting::get_text_size);
    ClassDB::bind_method(D_METHOD("set_extra_delay", "value"), &CharacterSetting::set_extra_delay);
    ClassDB::bind_method(D_METHOD("get_extra_delay"), &CharacterSetting::get_extra_delay);
    ClassDB::bind_method(D_METHOD("set_no_sound", "value"), &CharacterSetting::set_no_sound);
    ClassDB::bind_method(D_METHOD("get_no_sound"), &CharacterSetting::get_no_sound);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "font", PROPERTY_HINT_RESOURCE_TYPE, "Font"), "set_font", "get_font");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "text_size"), "set_text_size", "get_text_size");

    ADD_GROUP("advanced", "");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "extra_delay"), "set_extra_delay", "get_extra_delay");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "no_sound"), "set_no_sound", "get_no_sound");
}

void CharacterSetting::set_font(const Ref<Font>& value) {
    font = value;
}

Ref<Font> CharacterSetting::get_font() const {
    return font;
}

void CharacterSetting::set_text_size(float value) {
    text_size = value;
}

float CharacterSetting::get_text_size() const {
    return text_size;
}

String CharacterSetting::get_no_sound() {
    return no_sound;
}

void CharacterSetting::set_no_sound(String value) {
    no_sound = value;
}

String CharacterSetting::get_extra_delay() {
    return extra_delay;
}

void CharacterSetting::set_extra_delay(String value) {
    extra_delay = value;
}