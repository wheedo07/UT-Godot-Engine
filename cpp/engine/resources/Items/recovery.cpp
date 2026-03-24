#include "recovery.h"
using namespace godot;

RecoveryItem::RecoveryItem() {
    heal_amount = 0;
    attack_amount = 0;
    defense_amount = 0;
}

RecoveryItem::~RecoveryItem() {}

void RecoveryItem::_bind_methods() {
    ADD_GROUP("Recovery Properties", "");
    ClassDB::bind_method(D_METHOD("set_heal_amount", "value"), &RecoveryItem::set_heal_amount);
    ClassDB::bind_method(D_METHOD("get_heal_amount"), &RecoveryItem::get_heal_amount);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "heal_amount"), "set_heal_amount", "get_heal_amount");

    ClassDB::bind_method(D_METHOD("set_attack_amount", "value"), &RecoveryItem::set_attack_amount);
    ClassDB::bind_method(D_METHOD("get_attack_amount"), &RecoveryItem::get_attack_amount);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_amount"), "set_attack_amount", "get_attack_amount");

    ClassDB::bind_method(D_METHOD("set_defense_amount", "value"), &RecoveryItem::set_defense_amount);
    ClassDB::bind_method(D_METHOD("get_defense_amount"), &RecoveryItem::get_defense_amount);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "defense_amount"), "set_defense_amount", "get_defense_amount");
}

void RecoveryItem::set_heal_amount(int p_amount) {
    heal_amount = p_amount;
}

int RecoveryItem::get_heal_amount() {
    return heal_amount;
}

void RecoveryItem::set_attack_amount(int p_amount) {
    attack_amount = p_amount;
}

int RecoveryItem::get_attack_amount() {
    return attack_amount;
}

void RecoveryItem::set_defense_amount(int p_amount) {
    defense_amount = p_amount;
}

int RecoveryItem::get_defense_amount() {
    return defense_amount;
}