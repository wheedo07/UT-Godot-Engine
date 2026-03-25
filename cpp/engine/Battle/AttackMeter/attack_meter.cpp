#include "attack_meter.h"
#include "env.h"
#include "engine/resources/Items/item.h"
#include "engine/Battle/encounter_script.h"
using namespace godot;

AttackMeter::AttackMeter() {
    enemy_id = -1;
    enemy_def = 0;
    encounter_script = nullptr;
}

AttackMeter::~AttackMeter() {}

void AttackMeter::_bind_methods() {
    ADD_SIGNAL(MethodInfo("damagetarget", 
        PropertyInfo(Variant::INT, "damage"), 
        PropertyInfo(Variant::INT, "enemy_id"), 
        PropertyInfo(Variant::BOOL, "crit")));
    ADD_SIGNAL(MethodInfo("missed", PropertyInfo(Variant::INT, "enemy_id")));

    ClassDB::bind_method(D_METHOD("set_property", "value"), &AttackMeter::set_property);
    ClassDB::bind_method(D_METHOD("get_enemy_id"), &AttackMeter::get_enemy_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "enemy_id", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_enemy_id");

    ClassDB::bind_method(D_METHOD("get_enemy_def"), &AttackMeter::get_enemy_def);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "enemy_def", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_enemy_def");

    ClassDB::bind_method(D_METHOD("get_encounter_script"), &AttackMeter::get_encounter_script);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "encounter_script", PROPERTY_HINT_NONE, "EncounterScript", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_encounter_script");

    ClassDB::bind_method(D_METHOD("get_base_damage"), &AttackMeter::get_base_damage);
}

int AttackMeter::get_base_damage() {
    int damage = global->get_player_attack();

    if(encounter_script) {
        Ref<Item> weapon = encounter_script->get_player_weapon();
        if(weapon.is_valid()) {
            int w_atk = weapon->get("attack_amount");
            if(w_atk) damage += w_atk;
        }

        Ref<Item> armor = encounter_script->get_player_armor();
        if(armor.is_valid()) {
            int a_atk = armor->get("attack_amount");
            if(a_atk) damage += a_atk;
        }
    }

    damage += global->get_temp_atk();
    return damage;
}

EncounterScript *AttackMeter::get_encounter_script() {
    return encounter_script;
}

int AttackMeter::get_enemy_id() {
    return enemy_id;
}

int AttackMeter::get_enemy_def() {
    return enemy_def;
}

void AttackMeter::set_property(Variant value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

void AttackMeter::set_encounter_script(EncounterScript *value) {
    encounter_script = value;
}

void AttackMeter::set_enemy_id(int value) {
    enemy_id = value;
}

void AttackMeter::set_enemy_def(int value) {
    enemy_def = value;
}