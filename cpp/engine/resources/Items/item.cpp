#include "item.h"
using namespace godot;

Item::Item() {
    item_type = CONSUMABLE;
    item_name = "TestItem";
    weapon_speed = 1.5;
    weapon_bars = 1;
    weapon_type = KNIFE;
    critical_hits = false;
    weapon_delay = 0;
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
    ADD_PROPERTY(PropertyInfo(Variant::INT, "item_type", PROPERTY_HINT_ENUM, "CONSUMABLE,WEAPON,ARMOR"), "set_item_type", "get_item_type");
    ClassDB::bind_method(D_METHOD("set_item_name", "name"), &Item::set_item_name);
    ClassDB::bind_method(D_METHOD("get_item_name"), &Item::get_item_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_name"), "set_item_name", "get_item_name");
    
    ADD_GROUP("Item Use Messages", "");
    ClassDB::bind_method(D_METHOD("set_use_message", "message"), &Item::set_use_message);
    ClassDB::bind_method(D_METHOD("get_use_message"), &Item::get_use_message);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "use_message", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_use_message", "get_use_message");
    ClassDB::bind_method(D_METHOD("set_item_information", "info"), &Item::set_item_information);
    ClassDB::bind_method(D_METHOD("get_item_information"), &Item::get_item_information);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "item_information", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_item_information", "get_item_information");
    ClassDB::bind_method(D_METHOD("set_throw_message", "message"), &Item::set_throw_message);
    ClassDB::bind_method(D_METHOD("get_throw_message"), &Item::get_throw_message);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "throw_message", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_throw_message", "get_throw_message");
    
    ClassDB::bind_method(D_METHOD("get_item_name_tr"), &Item::get_item_name_tr);
}

void Item::_get_property_list(List<PropertyInfo> *p_list) const {
    p_list->push_back(PropertyInfo(Variant::NIL, "Item Stats", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));
    if(item_type == CONSUMABLE) {
        p_list->push_back(PropertyInfo(Variant::INT, "heal_amount"));
    }
    p_list->push_back(PropertyInfo(Variant::INT, "defense_amount"));
    p_list->push_back(PropertyInfo(Variant::INT, "attack_amount"));

    if(item_type != WEAPON) return;
    p_list->push_back(PropertyInfo(Variant::NIL, "Weapon Stats", PROPERTY_HINT_NONE, "weapon_", PROPERTY_USAGE_GROUP));
    p_list->push_back(PropertyInfo(Variant::FLOAT, "weapon_speed"));
    p_list->push_back(PropertyInfo(Variant::INT, "weapon_delay"));
    p_list->push_back(PropertyInfo(Variant::INT, "weapon_bars"));
    p_list->push_back(PropertyInfo(Variant::INT, "weapon_type", PROPERTY_HINT_ENUM, "KNIFE,PUNCH,SHOE,BOOK,PAN,GUN"));
    p_list->push_back(PropertyInfo(Variant::BOOL, "weapon_critical_hits"));
}

bool Item::_set(const StringName& p_name, const Variant& p_value) {
    String name = p_name;
    if(name == "heal_amount") {
        heal_amount = p_value;
        return true;
    }else if(name == "attack_amount") {
        attack_amount = p_value;
        return true;
    }else if(name == "defense_amount") {
        defense_amount = p_value;
        return true;
    }else if(name == "weapon_speed") {
        weapon_speed = p_value;
        return true;
    }else if(name == "weapon_delay") {
        weapon_delay = p_value;
        return true;
    }else if(name == "weapon_bars") {
        weapon_bars = p_value;
        return true;
    }else if(name == "weapon_type") {
        weapon_type = WeaponType(int(p_value));
        return true;
    }else if(name == "weapon_critical_hits") {
        critical_hits = p_value;
        return true;
    }
    return false;
}

bool Item::_get(const StringName& p_name, Variant& r_ret) {
    String name = p_name;
    if(name == "heal_amount") {
        r_ret = heal_amount;
        return true;
    }else if(name == "attack_amount") {
        r_ret = attack_amount;
        return true;
    }else if(name == "defense_amount") {
        r_ret = defense_amount;
        return true;
    }else if(name == "weapon_speed") {
        r_ret = weapon_speed;
        return true;
    }else if(name == "weapon_delay") {
        r_ret = weapon_delay;
        return true;
    }else if(name == "weapon_bars") {
        r_ret = weapon_bars;
        return true;
    }else if(name == "weapon_type") {
        r_ret = int(weapon_type);
        return true;
    }else if(name == "weapon_critical_hits") {
        r_ret = critical_hits;
        return true;
    }
    return false;
}

void Item::set_item_type(ItemType p_type) {
    item_type = p_type;
    notify_property_list_changed();
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

String Item::get_item_name_tr() const {
    return tr(item_name);
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

void Item::set_weapon_delay(int p_delay) {
    weapon_delay = p_delay;
}

int Item::get_weapon_delay() const {
    return weapon_delay;
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