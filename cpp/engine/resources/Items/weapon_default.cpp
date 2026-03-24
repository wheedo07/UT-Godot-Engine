#include "weapon_default.h"
using namespace godot;

WeaponDefault::WeaponDefault() {
    weapon_animation = KNIFE;
}

WeaponDefault::~WeaponDefault() {}

void WeaponDefault::_bind_methods() {
    BIND_ENUM_CONSTANT(KNIFE);
    BIND_ENUM_CONSTANT(PUNCH);
    BIND_ENUM_CONSTANT(SHOE);
    BIND_ENUM_CONSTANT(BOOK);
    BIND_ENUM_CONSTANT(PAN);
    BIND_ENUM_CONSTANT(GUN);

    ADD_GROUP("Default Weapon", "");
    ClassDB::bind_method(D_METHOD("set_weapon_animation", "value"), &WeaponDefault::set_weapon_animation);
    ClassDB::bind_method(D_METHOD("get_weapon_animation"), &WeaponDefault::get_weapon_animation);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "weapon_animation", PROPERTY_HINT_ENUM, "KNIFE,PUNCH,SHOE,BOOK,PAN,GUN"), "set_weapon_animation", "get_weapon_animation");
}

void WeaponDefault::set_weapon_animation(WeaponAnimation value) {
    weapon_animation = value;
}

WeaponDefault::WeaponAnimation WeaponDefault::get_weapon_animation() {
    return weapon_animation;
}