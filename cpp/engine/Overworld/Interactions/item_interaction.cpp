#include "item_interaction.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>

ItemInteraction::ItemInteraction() {
    item_id = 0;
    one_item = true;
    
    discover_text.push_back(String::utf8("* 뭐뭐가 있다"));
    discover_text.push_back(String::utf8("* 가져갈까?"));
    
    accept_text.push_back(String::utf8("* 당신은 [color=yellow]뭐뭐[/color]을 얻었다"));
    
    reject_text.push_back(String::utf8("* 당신은 가져갈 필요성을 못느낀다"));
    
    full_text.push_back(String::utf8("* 당신의 인벤토리가 가득찼다"));
    
    option_names.push_back(String::utf8("네"));
    option_names.push_back(String::utf8("아니요"));
}

ItemInteraction::~ItemInteraction() {}

void ItemInteraction::_bind_methods() {
    BIND_ENUM_CONSTANT(ITEM_PICK_UP);
    BIND_ENUM_CONSTANT(ITEM_LEAVE_ALONE);
    
    ClassDB::bind_method(D_METHOD("discover"), &ItemInteraction::discover);
    ClassDB::bind_method(D_METHOD("disable_item"), &ItemInteraction::disable_item);
    ClassDB::bind_method(D_METHOD("_on_text_box_selected_option", "decision"), &ItemInteraction::_on_text_box_selected_option);
    
    ClassDB::bind_method(D_METHOD("set_discover_text", "text"), &ItemInteraction::set_discover_text);
    ClassDB::bind_method(D_METHOD("get_discover_text"), &ItemInteraction::get_discover_text);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "discover_text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_discover_text", "get_discover_text");
    
    ClassDB::bind_method(D_METHOD("set_accept_text", "text"), &ItemInteraction::set_accept_text);
    ClassDB::bind_method(D_METHOD("get_accept_text"), &ItemInteraction::get_accept_text);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "accept_text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_accept_text", "get_accept_text");
    
    ClassDB::bind_method(D_METHOD("set_reject_text", "text"), &ItemInteraction::set_reject_text);
    ClassDB::bind_method(D_METHOD("get_reject_text"), &ItemInteraction::get_reject_text);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "reject_text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_reject_text", "get_reject_text");
    
    ClassDB::bind_method(D_METHOD("set_full_text", "text"), &ItemInteraction::set_full_text);
    ClassDB::bind_method(D_METHOD("get_full_text"), &ItemInteraction::get_full_text);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "full_text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_full_text", "get_full_text");
    
    ClassDB::bind_method(D_METHOD("set_option_names", "names"), &ItemInteraction::set_option_names);
    ClassDB::bind_method(D_METHOD("get_option_names"), &ItemInteraction::get_option_names);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "option_names"), "set_option_names", "get_option_names");
    
    ClassDB::bind_method(D_METHOD("set_item_id", "id"), &ItemInteraction::set_item_id);
    ClassDB::bind_method(D_METHOD("get_item_id"), &ItemInteraction::get_item_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "item_id"), "set_item_id", "get_item_id");
    
    ClassDB::bind_method(D_METHOD("set_one_item", "one_item"), &ItemInteraction::set_one_item);
    ClassDB::bind_method(D_METHOD("get_one_item"), &ItemInteraction::get_one_item);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "one_item"), "set_one_item", "get_one_item");
    
    ADD_SIGNAL(MethodInfo("took_item"));
}

void ItemInteraction::_ready() {
    if(isEditor) return;
    text_box = ResourceLoader::get_singleton()->load("res://Overworld/text_box.tscn");
}

void ItemInteraction::discover() {
    TextBox* txtbox = Object::cast_to<TextBox>(text_box->instantiate());
    add_child(txtbox);
    
    bool inventory_full = global->get_items().size() == 8;
    
    Ref<Dialogues> discover_dialogues = memnew(Dialogues);
    discover_dialogues->from(discover_text);
    
    Ref<Dialogues> accept_dialogues = memnew(Dialogues);
    accept_dialogues->from(inventory_full ? full_text : accept_text);
    
    if (reject_text.size() == 0) {
        Array options;
        options.push_back(accept_dialogues);
        txtbox->generic(discover_dialogues, option_names, options);
    } else {
        Ref<Dialogues> reject_dialogues = memnew(Dialogues);
        reject_dialogues->from(reject_text);
        
        Array options;
        options.push_back(accept_dialogues);
        options.push_back(reject_dialogues);
        txtbox->generic(discover_dialogues, option_names, options);
    }
    
    txtbox->connect("selected_option", Callable(this, "_on_text_box_selected_option"));
}

void ItemInteraction::_on_text_box_selected_option(int decision) {
    if (decision == ITEM_PICK_UP && global->get_items().size() != 8) {
        Array items = global->get_items();
        items.append(item_id);
        global->set_items(items);
        
        emit_signal("took_item");
        
        Object::cast_to<AudioStreamPlayer>(get_node_internal("PickUp"))->play();
        
        if (one_item) {
            disable_item();
        }
    }
}

void ItemInteraction::disable_item() {
    set_position(Vector2(INFINITY, INFINITY));
    set_monitoring(false);
    set_monitorable(false);
}

void ItemInteraction::set_discover_text(const PackedStringArray& p_text) {
    discover_text = p_text;
}

PackedStringArray ItemInteraction::get_discover_text() const {
    return discover_text;
}

void ItemInteraction::set_accept_text(const PackedStringArray& p_text) {
    accept_text = p_text;
}

PackedStringArray ItemInteraction::get_accept_text() const {
    return accept_text;
}

void ItemInteraction::set_reject_text(const PackedStringArray& p_text) {
    reject_text = p_text;
}

PackedStringArray ItemInteraction::get_reject_text() const {
    return reject_text;
}

void ItemInteraction::set_full_text(const PackedStringArray& p_text) {
    full_text = p_text;
}

PackedStringArray ItemInteraction::get_full_text() const {
    return full_text;
}

void ItemInteraction::set_option_names(const PackedStringArray& p_names) {
    option_names = p_names;
}

PackedStringArray ItemInteraction::get_option_names() const {
    return option_names;
}

void ItemInteraction::set_item_id(int p_id) {
    item_id = p_id;
}

int ItemInteraction::get_item_id() const {
    return item_id;
}

void ItemInteraction::set_one_item(bool p_one_item) {
    one_item = p_one_item;
}

bool ItemInteraction::get_one_item() const {
    return one_item;
}