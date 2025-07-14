#include "encounter.h"
using namespace godot;

Encounter::Encounter() {
    encounter_name = String::utf8("이름");
    flee_chance = 0.2f;
    
    mercy_options.push_back(String::utf8("* 자비"));
    mercy_options.push_back(String::utf8("* 도망"));
}

Encounter::~Encounter() {}

void Encounter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_encounter_name", "name"), &Encounter::set_encounter_name);
    ClassDB::bind_method(D_METHOD("get_encounter_name"), &Encounter::get_encounter_name);
    
    ClassDB::bind_method(D_METHOD("set_background", "background"), &Encounter::set_background);
    ClassDB::bind_method(D_METHOD("get_background"), &Encounter::get_background);
    
    ClassDB::bind_method(D_METHOD("set_enemies", "enemies"), &Encounter::set_enemies);
    ClassDB::bind_method(D_METHOD("get_enemies"), &Encounter::get_enemies);
    
    ClassDB::bind_method(D_METHOD("set_music", "music"), &Encounter::set_music);
    ClassDB::bind_method(D_METHOD("get_music"), &Encounter::get_music);
    
    ClassDB::bind_method(D_METHOD("set_mercy_options", "options"), &Encounter::set_mercy_options);
    ClassDB::bind_method(D_METHOD("get_mercy_options"), &Encounter::get_mercy_options);
    
    ClassDB::bind_method(D_METHOD("set_flee_chance", "chance"), &Encounter::set_flee_chance);
    ClassDB::bind_method(D_METHOD("get_flee_chance"), &Encounter::get_flee_chance);

    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "encounter_name"), "set_encounter_name", "get_encounter_name");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "background", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_background", "get_background");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "enemies", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + "/" + ":PackedScene")
    , "set_enemies", "get_enemies");
    ADD_GROUP("Music", "music_");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "music", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_music", "get_music");
    ADD_GROUP("", "");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "mercy_options", PROPERTY_HINT_ARRAY_TYPE, "String"), "set_mercy_options", "get_mercy_options");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "flee_chance", PROPERTY_HINT_RANGE, "0,1"), "set_flee_chance", "get_flee_chance");
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

void Encounter::set_enemies(const TypedArray<PackedScene>& p_enemies) {
    if (p_enemies.size() <= enemysizelimit) {
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
    flee_chance = CLAMP(p_chance, 0.0f, 1.0f);
}

float Encounter::get_flee_chance() const {
    return flee_chance;
}