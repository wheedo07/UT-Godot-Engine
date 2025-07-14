#include "save_point.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/node.hpp>

SavePoint::SavePoint() {
    save_text.push_back(String::utf8("* 의지!!!"));
}

SavePoint::~SavePoint() {}

void SavePoint::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_interact_save"), &SavePoint::_on_interact_save);

    ClassDB::bind_method(D_METHOD("on_dialogue_finished"), &SavePoint::on_dialogue_finished);
    ClassDB::bind_method(D_METHOD("set_save_text", "text"), &SavePoint::set_save_text);
    ClassDB::bind_method(D_METHOD("get_save_text"), &SavePoint::get_save_text);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "save_text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT)),
    "set_save_text", "get_save_text");
}

void SavePoint::_ready() {
    if(isEditor) return;
    ResourceLoader* loader = ResourceLoader::get_singleton();
    txt_box = loader->load("res://Overworld/text_box.tscn");
    save_menu = loader->load("res://Overworld/save_menu.tscn");
    
    bool isGenocide = global->get_flag("isGenocide");
    if(!isGenocide) save_text.push_back(String::utf8("* (HP 가 가득찼다.)"));
}

void SavePoint::_on_interact_save() {
    global->heal(global->get_player_max_hp());
    
    TextBox* textbox = Object::cast_to<TextBox>(txt_box->instantiate());
    global->get_scene_container()->get_current_scene()->add_child(textbox);
    
    Ref<Dialogues> dialogues = memnew(Dialogues);
    dialogues->from(save_text);
    
    textbox->connect("dialogue_finished", Callable(this, "on_dialogue_finished"), CONNECT_ONE_SHOT);
    
    textbox->generic(dialogues);
}

void SavePoint::on_dialogue_finished() {
    Node* current_scene = global->get_scene_container()->get_current_scene();
    
    Node* save_menu_instance = save_menu->instantiate();
    if (save_menu_instance) {
        current_scene->add_child(save_menu_instance);
    }
}

void SavePoint::set_save_text(const PackedStringArray& p_text) {
    save_text = p_text;
}

PackedStringArray SavePoint::get_save_text() const {
    return save_text;
}