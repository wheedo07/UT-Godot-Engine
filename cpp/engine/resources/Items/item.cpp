#include "item.h"
using namespace godot;

Item::Item() {
    item_name = "TestItem";
    isConsumable = true;
    use_message.push_back(String::utf8("* 당신은 아이템을 사용했다!"));
    item_information.push_back(String::utf8("* Item - 0 hp 회복 \n* 버터다 그냥 파이도 아닌 버터다"));
    throw_message.push_back(String::utf8("* 딩신은 아이템을 바닥의 버렸다"));
}

Item::~Item() {}

void Item::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_item_name_tr"), &Item::get_item_name_tr);

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
   
    ADD_GROUP("Item Properties", "");
    ClassDB::bind_method(D_METHOD("set_item_name", "name"), &Item::set_item_name);
    ClassDB::bind_method(D_METHOD("get_item_name"), &Item::get_item_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_name"), "set_item_name", "get_item_name");
}

void Item::_get_property_list(List<PropertyInfo> *p_list) const {
    if(get_class() != "Item") return;
    p_list->push_back(PropertyInfo(Variant::BOOL, "is_consumable"));
}

bool Item::_set(const StringName& p_name, const Variant& p_value) {
    String name = p_name;
    if(name == "is_consumable") {
        isConsumable = p_value;
        return true;
    }
    return false;
}

bool Item::_get(const StringName& p_name, Variant& r_ret) {
    String name = p_name;
    if(name == "is_consumable") {
        r_ret = isConsumable;
        return true;
    }
    return false;
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

void Item::set_consumable(bool p_consumable) {
    isConsumable = p_consumable;
}

bool Item::is_consumable() const {
    return isConsumable;
}