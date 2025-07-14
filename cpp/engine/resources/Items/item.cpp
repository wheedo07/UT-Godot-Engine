#include "item.h"
using namespace godot;

Item::Item() {
    item_type = CONSUMABLE;
    item_name = "TestItem";
    weapon_speed = 1.5;
    weapon_bars = 1;
    weapon_type = KNIFE;
    critical_hits = false;
    use_message.push_back(String::utf8("* 당신은 아이템을 사용했다!"));
    item_information.push_back(String::utf8("* Item - 0 hp 회복 \n* 버터다 그냥 파이도 아닌 버터다"));
    throw_message.push_back(String::utf8("* 딩신은 아이템을 바닥의 버렸다"));
    heal_amount = 0;
    attack_amount = 0;
    defense_amount = 0;
}

Item::~Item() {}

void Item::_bind_methods() {
    BIND_ENUM_CONSTANT(KNIFE);
    BIND_ENUM_CONSTANT(PUNCH);
    BIND_ENUM_CONSTANT(SHOE);
    BIND_ENUM_CONSTANT(BOOK);
    BIND_ENUM_CONSTANT(PAN);
    BIND_ENUM_CONSTANT(GUN);

    BIND_ENUM_CONSTANT(CONSUMABLE);
    BIND_ENUM_CONSTANT(WEAPON);
    BIND_ENUM_CONSTANT(ARMOR);

    ClassDB::bind_method(D_METHOD("set_item_type", "type"), &Item::set_item_type);
    ClassDB::bind_method(D_METHOD("get_item_type"), &Item::get_item_type);
    ClassDB::bind_method(D_METHOD("set_item_name", "name"), &Item::set_item_name);
    ClassDB::bind_method(D_METHOD("get_item_name"), &Item::get_item_name);
    ClassDB::bind_method(D_METHOD("set_weapon_speed", "speed"), &Item::set_weapon_speed);
    ClassDB::bind_method(D_METHOD("get_weapon_speed"), &Item::get_weapon_speed);
    ClassDB::bind_method(D_METHOD("set_weapon_bars", "bars"), &Item::set_weapon_bars);
    ClassDB::bind_method(D_METHOD("get_weapon_bars"), &Item::get_weapon_bars);
    ClassDB::bind_method(D_METHOD("set_weapon_type", "type"), &Item::set_weapon_type);
    ClassDB::bind_method(D_METHOD("get_weapon_type"), &Item::get_weapon_type);
    ClassDB::bind_method(D_METHOD("set_critical_hits", "critical"), &Item::set_critical_hits);
    ClassDB::bind_method(D_METHOD("get_critical_hits"), &Item::get_critical_hits);
    ClassDB::bind_method(D_METHOD("set_use_message", "message"), &Item::set_use_message);
    ClassDB::bind_method(D_METHOD("get_use_message"), &Item::get_use_message);
    ClassDB::bind_method(D_METHOD("set_item_information", "info"), &Item::set_item_information);
    ClassDB::bind_method(D_METHOD("get_item_information"), &Item::get_item_information);
    ClassDB::bind_method(D_METHOD("set_throw_message", "message"), &Item::set_throw_message);
    ClassDB::bind_method(D_METHOD("get_throw_message"), &Item::get_throw_message);
    ClassDB::bind_method(D_METHOD("set_heal_amount", "amount"), &Item::set_heal_amount);
    ClassDB::bind_method(D_METHOD("get_heal_amount"), &Item::get_heal_amount);
    ClassDB::bind_method(D_METHOD("set_attack_amount", "amount"), &Item::set_attack_amount);
    ClassDB::bind_method(D_METHOD("get_attack_amount"), &Item::get_attack_amount);
    ClassDB::bind_method(D_METHOD("set_defense_amount", "amount"), &Item::set_defense_amount);
    ClassDB::bind_method(D_METHOD("get_defense_amount"), &Item::get_defense_amount);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "item_type", PROPERTY_HINT_ENUM, "CONSUMABLE,WEAPON,ARMOR"), "set_item_type", "get_item_type");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_name"), "set_item_name", "get_item_name");
    
    ADD_GROUP("Weapon Stats", "weapon_");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "weapon_speed"), "set_weapon_speed", "get_weapon_speed");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "weapon_bars"), "set_weapon_bars", "get_weapon_bars");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "weapon_type", PROPERTY_HINT_ENUM, "KNIFE,PUNCH,SHOE,BOOK,PAN,GUN"), "set_weapon_type", "get_weapon_type");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "critical_hits"), "set_critical_hits", "get_critical_hits");
    
    ADD_GROUP("Item Use Stats", "item_use_");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "use_message", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT)),
    "set_use_message", "get_use_message");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "item_information", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT)),
    "set_item_information", "get_item_information");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "throw_message", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT)),
    "set_throw_message", "get_throw_message");
    
    ADD_GROUP("", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "heal_amount"), "set_heal_amount", "get_heal_amount");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_amount"), "set_attack_amount", "get_attack_amount");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "defense_amount"), "set_defense_amount", "get_defense_amount");
}

void Item::set_item_type(ItemType p_type) {
    item_type = p_type;
}

Item::ItemType Item::get_item_type() const {
    return item_type;
}

void Item::set_item_name(const String& p_name) {
    item_name = p_name;
}

String Item::get_item_name() const {
    return item_name;
}

void Item::set_weapon_speed(float p_speed) {
    weapon_speed = p_speed;
}

float Item::get_weapon_speed() const {
    return weapon_speed;
}

void Item::set_weapon_bars(int p_bars) {
    weapon_bars = p_bars;
}

int Item::get_weapon_bars() const {
    return weapon_bars;
}

void Item::set_weapon_type(WeaponType p_type) {
    weapon_type = p_type;
}

Item::WeaponType Item::get_weapon_type() const {
    return weapon_type;
}

void Item::set_critical_hits(bool p_critical) {
    critical_hits = p_critical;
}

bool Item::get_critical_hits() const {
    return critical_hits;
}

void Item::set_use_message(const PackedStringArray& p_message) {
    use_message = p_message;
}

PackedStringArray Item::get_use_message() const {
    return use_message;
}

void Item::set_item_information(const PackedStringArray& p_info) {
    item_information = p_info;
}

PackedStringArray Item::get_item_information() const {
    return item_information;
}

void Item::set_throw_message(const PackedStringArray& p_message) {
    throw_message = p_message;
}

PackedStringArray Item::get_throw_message() const {
    return throw_message;
}

void Item::set_heal_amount(int p_amount) {
    heal_amount = p_amount;
}

int Item::get_heal_amount() const {
    return heal_amount;
}

void Item::set_attack_amount(int p_amount) {
    attack_amount = p_amount;
}

int Item::get_attack_amount() const {
    return attack_amount;
}

void Item::set_defense_amount(int p_amount) {
    defense_amount = p_amount;
}

int Item::get_defense_amount() const {
    return defense_amount;
}