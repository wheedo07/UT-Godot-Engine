#include "armor.h"
using namespace godot;

Armor::Armor() {
    attack_amount = 0;
    defense_amount = 0;
}

Armor::~Armor() {}

void Armor::_bind_methods() {
    ADD_GROUP("Armor Stats", "");
    ClassDB::bind_method(D_METHOD("set_attack_amount", "value"), &Armor::set_attack_amount);
    ClassDB::bind_method(D_METHOD("get_attack_amount"), &Armor::get_attack_amount);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_amount"), "set_attack_amount", "get_attack_amount");

    ClassDB::bind_method(D_METHOD("set_defense_amount", "value"), &Armor::set_defense_amount);
    ClassDB::bind_method(D_METHOD("get_defense_amount"), &Armor::get_defense_amount);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "defense_amount"), "set_defense_amount", "get_defense_amount");
}

void Armor::set_attack_amount(int p_amount) {
    attack_amount = p_amount;
}

int Armor::get_attack_amount() {
    return attack_amount;
}

void Armor::set_defense_amount(int p_amount) {
    defense_amount = p_amount;
}

int Armor::get_defense_amount() {
    return defense_amount;
}