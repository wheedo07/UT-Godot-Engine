#include "encounter.h"
#define Enemysize_Limit 3
using namespace godot;

Encounter::Encounter() {
    encounter_name = "name";
    offset = Vector2(0,0);
    flee_chance = 0.2f;
    
    mercy_options.push_back("UT_SPARE");
    mercy_options.push_back("UT_FLEE");

    kr_text = "KR";
    kr_color = Color(1, 0, 1);
}

Encounter::~Encounter() {}

void Encounter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_encounter_name", "name"), &Encounter::set_encounter_name);
    ClassDB::bind_method(D_METHOD("get_encounter_name"), &Encounter::get_encounter_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "encounter_name"), "set_encounter_name", "get_encounter_name");

    ClassDB::bind_method(D_METHOD("set_flavour_text", "text"), &Encounter::set_flavour_text);
    ClassDB::bind_method(D_METHOD("get_flavour_text"), &Encounter::get_flavour_text);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "flavour_text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_flavour_text", "get_flavour_text");
    
    ClassDB::bind_method(D_METHOD("set_enemies", "enemies"), &Encounter::set_enemies);
    ClassDB::bind_method(D_METHOD("get_enemies"), &Encounter::get_enemies);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "enemies", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + "/" + ":PackedScene")
    , "set_enemies", "get_enemies");

    ClassDB::bind_method(D_METHOD("set_encounter_script", "script"), &Encounter::set_encounter_script);
    ClassDB::bind_method(D_METHOD("get_encounter_script"), &Encounter::get_encounter_script);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "encounter_script", PROPERTY_HINT_RESOURCE_TYPE, "Script"), "set_encounter_script", "get_encounter_script");

    ADD_GROUP("background", "");
    ClassDB::bind_method(D_METHOD("set_background", "background"), &Encounter::set_background);
    ClassDB::bind_method(D_METHOD("get_background"), &Encounter::get_background);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "background", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_background", "get_background");
    ClassDB::bind_method(D_METHOD("set_offset", "offset"), &Encounter::set_offset);
    ClassDB::bind_method(D_METHOD("get_offset"), &Encounter::get_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset"), "set_offset", "get_offset");

    ADD_GROUP("mercy", "");
    ClassDB::bind_method(D_METHOD("set_mercy_options", "options"), &Encounter::set_mercy_options);
    ClassDB::bind_method(D_METHOD("get_mercy_options"), &Encounter::get_mercy_options);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "mercy_options", PROPERTY_HINT_ARRAY_TYPE, "String"), "set_mercy_options", "get_mercy_options");
    ClassDB::bind_method(D_METHOD("set_flee_chance", "chance"), &Encounter::set_flee_chance);
    ClassDB::bind_method(D_METHOD("get_flee_chance"), &Encounter::get_flee_chance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "flee_chance", PROPERTY_HINT_RANGE, "0,1"), "set_flee_chance", "get_flee_chance");

    ADD_GROUP("Music", "");
    ClassDB::bind_method(D_METHOD("set_music", "music"), &Encounter::set_music);
    ClassDB::bind_method(D_METHOD("get_music"), &Encounter::get_music);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "music", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_music", "get_music");

    ADD_GROUP("Battle UI", "");
    ClassDB::bind_method(D_METHOD("set_button_set", "button_set"), &Encounter::set_button_set);
    ClassDB::bind_method(D_METHOD("get_button_set"), &Encounter::get_button_set);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "button_set", PROPERTY_HINT_RESOURCE_TYPE, "ButtonSet"), "set_button_set", "get_button_set");
    ClassDB::bind_method(D_METHOD("set_box_set", "box_set"), &Encounter::set_box_set);
    ClassDB::bind_method(D_METHOD("get_box_set"), &Encounter::get_box_set);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "box_set", PROPERTY_HINT_RESOURCE_TYPE, "BoxSet"), "set_box_set", "get_box_set");
    ClassDB::bind_method(D_METHOD("set_kr_text", "text"), &Encounter::set_kr_text);
    ClassDB::bind_method(D_METHOD("get_kr_text"), &Encounter::get_kr_text);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "kr_text"), "set_kr_text", "get_kr_text");
    ClassDB::bind_method(D_METHOD("set_kr_color", "color"), &Encounter::set_kr_color);
    ClassDB::bind_method(D_METHOD("get_kr_color"), &Encounter::get_kr_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "kr_color"), "set_kr_color", "get_kr_color");
}

void Encounter::set_encounter_name(const StringName& p_name) {
    encounter_name = p_name;
}

StringName Encounter::get_encounter_name() const {
    return encounter_name;
}

void Encounter::set_background(const Ref<Texture2D>& p_background) {
    background = p_background;
}

Ref<Texture2D> Encounter::get_background() const {
    return background;
}

void Encounter::set_offset(const Vector2& p_offset) {
    offset = p_offset;
}

Vector2 Encounter::get_offset() const {
    return offset;
}

void Encounter::set_enemies(const TypedArray<PackedScene>& p_enemies) {
    if (p_enemies.size() <= Enemysize_Limit) {
        enemies = p_enemies;
    }
}

TypedArray<PackedScene> Encounter::get_enemies() const {
    return enemies;
}

void Encounter::set_music(const Ref<AudioStream>& p_music) {
    music = p_music;
}

Ref<AudioStream> Encounter::get_music() const {
    return music;
}

void Encounter::set_mercy_options(const PackedStringArray& p_options) {
    mercy_options = p_options;
}

PackedStringArray Encounter::get_mercy_options() const {
    return mercy_options;
}

void Encounter::set_flee_chance(float p_chance) {
    flee_chance = CLAMP(p_chance, 0, 1);
}

float Encounter::get_flee_chance() const {
    return flee_chance;
}

void Encounter::set_button_set(const Ref<ButtonSet>& p_set) {
    button_set = p_set;
}

Ref<ButtonSet> Encounter::get_button_set() const {
    return button_set;
}

void Encounter::set_box_set(const Ref<BoxSet>& p_set) {
    box_set = p_set;
}

Ref<BoxSet> Encounter::get_box_set() const {
    return box_set;
}

void Encounter::set_kr_text(const String& p_text) {
    kr_text = p_text;
}

String Encounter::get_kr_text() const {
    return kr_text;
}

void Encounter::set_kr_color(const Color& p_color) {
    kr_color = p_color;
}

Color Encounter::get_kr_color() const {
    return kr_color;
}

void Encounter::set_encounter_script(const Ref<Script>& p_script) {
    script = p_script;
}

Ref<Script> Encounter::get_encounter_script() const {
    return script;
}

void Encounter::set_flavour_text(const PackedStringArray& p_text) {
    flavour_text = p_text;
}

PackedStringArray Encounter::get_flavour_text() const {
    return flavour_text;
}