#include "encounter_script.h"
#include "env.h"
#include "engine/resources/Items/item.h"

EncounterScript::EncounterScript() {}

EncounterScript::~EncounterScript() {}

void EncounterScript::_bind_methods() {
    GDVIRTUAL_BIND(ready);

    ClassDB::bind_method(D_METHOD("set_property", "value"), &EncounterScript::set_property);
    ClassDB::bind_method(D_METHOD("get_main"), &EncounterScript::get_main);
    ClassDB::bind_method(D_METHOD("get_box"), &EncounterScript::get_box);
    ClassDB::bind_method(D_METHOD("get_hub"), &EncounterScript::get_hub);
    ClassDB::bind_method(D_METHOD("get_soul"), &EncounterScript::get_soul);
    ClassDB::bind_method(D_METHOD("get_bg"), &EncounterScript::get_bg);
    ClassDB::bind_method(D_METHOD("get_attacks"), &EncounterScript::get_attacks);
    ClassDB::bind_method(D_METHOD("get_camera"), &EncounterScript::get_camera);
    ClassDB::bind_method(D_METHOD("get_player_turn"), &EncounterScript::get_player_turn);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "main", PROPERTY_HINT_NONE, "BattleMain", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_main");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "box", PROPERTY_HINT_NONE, "BattleBox", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_box");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "hub", PROPERTY_HINT_NONE, "BattleHUD", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_hub");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "soul", PROPERTY_HINT_NONE, "SoulBattle", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_soul");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bg", PROPERTY_HINT_NONE, "TextureRect", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_bg");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attacks", PROPERTY_HINT_NONE, "AttackManager", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_attacks");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NONE, "CameraController", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_camera");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "player_turn", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_player_turn");

    ClassDB::bind_method(D_METHOD("get_player_weapon"), &EncounterScript::get_player_weapon);
    ClassDB::bind_method(D_METHOD("get_player_armor"), &EncounterScript::get_player_armor);
}

void EncounterScript::_ready() {
    main = get_node<BattleMain>("..");
    box = main->box;
    hub = main->hud;
    soul = main->soul_battle;
    bg = main->bg;
    attacks = main->attacks;
    camera = main->camera;

    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    }else {
        ready();
    }
}

void EncounterScript::ready() {}

Ref<Item> EncounterScript::get_player_weapon() {
    return global->get_item_list()[global->get_equipment()["weapon"]];
}

Ref<Item> EncounterScript::get_player_armor() {
    return global->get_item_list()[global->get_equipment()["armor"]];
}

void EncounterScript::set_property(Variant value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

BattleMain *EncounterScript::get_main() {
    return main;
}

BattleBox *EncounterScript::get_box() {
    return box;
}

BattleHUD *EncounterScript::get_hub() {
    return hub;
}

SoulBattle *EncounterScript::get_soul() {
    return soul;
}

TextureRect *EncounterScript::get_bg() {
    return bg;
}

AttackManager *EncounterScript::get_attacks() {
    return attacks;
}

CameraController *EncounterScript::get_camera() {
    return camera;
}

bool EncounterScript::get_player_turn() {
    return main->player_turn;
}