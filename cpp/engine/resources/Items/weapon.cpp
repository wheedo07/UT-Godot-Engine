#include "weapon.h"
using namespace godot;

Weapon::Weapon() {
    attack_amount = 0;
    defense_amount = 0;
    slash_animation = "";
    weapon_speed = WEAPON_DEFAULT_SPEED;
    weapon_bars = WEAPON_DEFAULT_BARS;
    weapon_delay = WEAPON_DEFAULT_DELAY;
    critical_hits = WEAPON_DEFAULT_CRIT;
}

Weapon::~Weapon() {}

void Weapon:: _bind_methods() {
    ADD_GROUP("Weapon Stats", "");
    ClassDB::bind_method(D_METHOD("set_attack_amount", "value"), &Weapon::set_attack_amount);
    ClassDB::bind_method(D_METHOD("get_attack_amount"), &Weapon::get_attack_amount);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_amount"), "set_attack_amount", "get_attack_amount");

    ClassDB::bind_method(D_METHOD("set_defense_amount", "value"), &Weapon::set_defense_amount);
    ClassDB::bind_method(D_METHOD("get_defense_amount"), &Weapon::get_defense_amount);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "defense_amount"), "set_defense_amount", "get_defense_amount");

    ClassDB::bind_method(D_METHOD("set_weapon_speed", "value"), &Weapon::set_weapon_speed);
    ClassDB::bind_method(D_METHOD("get_weapon_speed"), &Weapon::get_weapon_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "weapon_speed"), "set_weapon_speed", "get_weapon_speed");

    ClassDB::bind_method(D_METHOD("set_weapon_bars", "value"), &Weapon::set_weapon_bars);
    ClassDB::bind_method(D_METHOD("get_weapon_bars"), &Weapon::get_weapon_bars);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "weapon_bars"), "set_weapon_bars", "get_weapon_bars");

    ClassDB::bind_method(D_METHOD("set_weapon_delay", "value"), &Weapon::set_weapon_delay);
    ClassDB::bind_method(D_METHOD("get_weapon_delay"), &Weapon::get_weapon_delay);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "weapon_delay"), "set_weapon_delay", "get_weapon_delay");

    ClassDB::bind_method(D_METHOD("set_critical_hits", "value"), &Weapon::set_critical_hits);
    ClassDB::bind_method(D_METHOD("get_critical_hits"), &Weapon::get_critical_hits);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "critical_hits"), "set_critical_hits", "get_critical_hits");
}

void Weapon::_get_property_list(List<PropertyInfo> *p_list) const {
    if(is_class("WeaponDefault")) return;
    p_list->push_back(PropertyInfo(Variant::NIL, "Custom Weapon", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));
    p_list->push_back(PropertyInfo(Variant::OBJECT, "slash_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"));
    p_list->push_back(PropertyInfo(Variant::STRING_NAME, "slash_animation"));
}

bool Weapon::_set(const StringName& p_name, const Variant& p_value) {
    String name = p_name;
    if(name == "slash_scene") {
        slash_scene = p_value;
        return true;
    }else if(name == "slash_animation") {
        slash_animation = p_value;
        return true;
    }
    return false;
}

bool Weapon::_get(const StringName& p_name, Variant& r_ret) {
    String name = p_name;
    if(name == "slash_scene") {
        r_ret = slash_scene;
        return true;
    }else if(name == "slash_animation") {
        r_ret = slash_animation;
        return true;
    }
    return false;
}

void Weapon::set_attack_amount(int p_amount) {
    attack_amount = p_amount;
}

int Weapon::get_attack_amount() {
    return attack_amount;
}

void Weapon::set_defense_amount(int p_amount) {
    defense_amount = p_amount;
}

int Weapon::get_defense_amount() {
    return defense_amount;
}

Ref<PackedScene> Weapon::get_slash_scene() {
    return slash_scene;
}

StringName Weapon::get_slash_animation() {
    return slash_animation;
}

void Weapon::set_weapon_speed(float value) {
    weapon_speed = value;
}

float Weapon::get_weapon_speed() {
    return weapon_speed;
}

void Weapon::set_weapon_bars(int value) {
    weapon_bars = value;
}

int Weapon::get_weapon_bars() {
    return weapon_bars;
}

void Weapon::set_weapon_delay(int value) {
    weapon_delay = value;
}

int Weapon::get_weapon_delay() {
    return weapon_delay;
}

void Weapon::set_critical_hits(bool value) {
    critical_hits = value;
}

bool Weapon::get_critical_hits() {
    return critical_hits;
}