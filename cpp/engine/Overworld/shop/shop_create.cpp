#include "shop_create.h"
#include "env.h"

ShopCreate::ShopCreate() {
    shop_ui = nullptr;
    music_player = nullptr;
    room_exit = nullptr;
    keeper_dialogue = nullptr;
    can_be_sold_to = true;
    room_id = 0;
    room_path = "";
}

ShopCreate::~ShopCreate() {}

void ShopCreate::_bind_methods() {
    // 스크립트 메소드
    BIND_VIRTUAL_METHOD(ShopCreate, ready, hash_djb2("ShopCreate_ready"));
    ClassDB::bind_method(D_METHOD("print_dialogue", "dialogues"), &ShopCreate::print_dialogue);

    ClassDB::bind_method(D_METHOD("set_music", "music"), &ShopCreate::set_music);
    ClassDB::bind_method(D_METHOD("get_music"), &ShopCreate::get_music);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shop_music", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_music", "get_music");

    ClassDB::bind_method(D_METHOD("set_click_path", "click_path"), &ShopCreate::set_click_path);
    ClassDB::bind_method(D_METHOD("get_click_path"), &ShopCreate::get_click_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "click_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AudioStreamPlayer"), "set_click_path", "get_click_path");

    ADD_GROUP("room", "");
    ClassDB::bind_method(D_METHOD("set_room_id", "room_id"), &ShopCreate::set_room_id);
    ClassDB::bind_method(D_METHOD("get_room_id"), &ShopCreate::get_room_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "room_id"), "set_room_id", "get_room_id");

    ClassDB::bind_method(D_METHOD("set_room_path", "room_path"), &ShopCreate::set_room_path);
    ClassDB::bind_method(D_METHOD("get_room_path"), &ShopCreate::get_room_path);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "room_path", PROPERTY_HINT_FILE, "*.tscn"), "set_room_path", "get_room_path");

    ADD_GROUP("shop_dialogues", "");
    ClassDB::bind_method(D_METHOD("set_offerings", "offerings"), &ShopCreate::set_offerings);
    ClassDB::bind_method(D_METHOD("get_offerings"), &ShopCreate::get_offerings);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "offerings", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":ShopItem"),
    "set_offerings", "get_offerings");

    ClassDB::bind_method(D_METHOD("set_sellferings", "sellferings"), &ShopCreate::set_sellferings);
    ClassDB::bind_method(D_METHOD("get_sellferings"), &ShopCreate::get_sellferings);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "sellferings", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":ShopItem"),
    "set_sellferings", "get_sellferings");

    ClassDB::bind_method(D_METHOD("set_keeper_dialogues", "keeper_dialogues"), &ShopCreate::set_keeper_dialogues);
    ClassDB::bind_method(D_METHOD("get_keeper_dialogues"), &ShopCreate::get_keeper_dialogues);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "keeper_dialogues", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":Dialogues"),
    "set_keeper_dialogues", "get_keeper_dialogues");

    ClassDB::bind_method(D_METHOD("set_keeper_def_dialogue", "keeper_def_dialogue"), &ShopCreate::set_keeper_def_dialogue);
    ClassDB::bind_method(D_METHOD("get_keeper_def_dialogue"), &ShopCreate::get_keeper_def_dialogue);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "keeper_def_dialogue", PROPERTY_HINT_RESOURCE_TYPE, "Dialogues"), "set_keeper_def_dialogue", "get_keeper_def_dialogue");

    ClassDB::bind_method(D_METHOD("set_keeper_cannot_sell_dialogues", "keeper_cannot_sell_dialogues"), &ShopCreate::set_keeper_cannot_sell_dialogues);
    ClassDB::bind_method(D_METHOD("get_keeper_cannot_sell_dialogues"), &ShopCreate::get_keeper_cannot_sell_dialogues);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "keeper_cannot_sell_dialogues", PROPERTY_HINT_RESOURCE_TYPE, "Dialogues"), "set_keeper_cannot_sell_dialogues", "get_keeper_cannot_sell_dialogues");

    ClassDB::bind_method(D_METHOD("set_dialogue_options", "dialogue_options"), &ShopCreate::set_dialogue_options);
    ClassDB::bind_method(D_METHOD("get_dialogue_options"), &ShopCreate::get_dialogue_options);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "dialogue_options"), "set_dialogue_options", "get_dialogue_options");

    ClassDB::bind_method(D_METHOD("set_can_be_sold_to", "can_be_sold_to"), &ShopCreate::set_can_be_sold_to);
    ClassDB::bind_method(D_METHOD("get_can_be_sold_to"), &ShopCreate::get_can_be_sold_to);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_be_sold_to"), "set_can_be_sold_to", "get_can_be_sold_to");
}

void ShopCreate::_ready() {
    if(isEditor) return;
    Ref<PackedScene> shop_scene = ResourceLoader::get_singleton()->load("res://Overworld/shop.tscn");
    if(!shop_scene.is_valid()) {
        ERR_PRINT("원래 경로에 shop.tscn이 없습니다.");
        return;
    }
    shop_ui = Object::cast_to<SHOP>(shop_scene->instantiate());
    music_player = global->get_Music();
    if(offerings.is_empty() && sellferings.is_empty() && keeper_dialogues.is_empty() && !keeper_def_dialogue.is_valid() && !keeper_cannot_sell_dialogues.is_valid() && dialogue_option.is_empty()) {
        ERR_PRINT("상점 아이템이나 대화가 설정되지 않았습니다.");
        return;
    }
    if(dialogue_option.size() > 5) {
        ERR_PRINT("대화 옵션은 최대 5개까지만 설정할 수 있습니다.");
        return;
    }
    add_child(shop_ui);
    room_exit = Object::cast_to<RoomEntranceNode>(shop_ui->get_node_internal("Control/room_exit"));
    keeper_dialogue = Object::cast_to<AdvancedTextTyper>(shop_ui->get_node_internal("Control/TextBox/MarginContainer/Dialogue"));

    if(music.is_valid()) {
        music_player->set_stream(music);
        music_player->play();
    } else {
        music_player->stop();
    }

    room_exit->set_new_room_entrance_id(room_id);
    room_exit->set_new_room(room_path);
    keeper_dialogue->set_click(Object::cast_to<AudioStreamPlayer>(get_node_internal(click_path)));
    shop_ui->set_offerings(offerings);
    shop_ui->set_sellferings(sellferings);
    shop_ui->set_keeper_dialogues(keeper_dialogues);
    shop_ui->set_keeper_def_dialogue(keeper_def_dialogue);
    shop_ui->set_can_be_sold_to(can_be_sold_to);
    shop_ui->set_keeper_cannot_sell_dialogues(keeper_cannot_sell_dialogues);
    shop_ui->set_dialogue_options(dialogue_option);
    shop_ui->init();

    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    }else {
        ready();
    }
}

void ShopCreate::ready() {}

void ShopCreate::print_dialogue(const Ref<Dialogues>& dialogues) {
    if(shop_ui) shop_ui->_keeper_dialogue_temp(dialogues, 0);
}

void ShopCreate::set_offerings(const Array& p_offerings) {
    offerings = p_offerings;
    if(shop_ui) {
        shop_ui->set_offerings(p_offerings);
    }
}

Array ShopCreate::get_offerings() const {
    return offerings;
}

void ShopCreate::set_sellferings(const Array& p_sellferings) {
    sellferings = p_sellferings;
    if(shop_ui) {
        shop_ui->set_sellferings(p_sellferings);
    }
}

Array ShopCreate::get_sellferings() const {
    return sellferings;
}

void ShopCreate::set_keeper_dialogues(const Array& p_keeper_dialogues) {
    keeper_dialogues = p_keeper_dialogues;
    if(shop_ui) {
        shop_ui->set_keeper_dialogues(p_keeper_dialogues);
    }
}

Array ShopCreate::get_keeper_dialogues() const {
    return keeper_dialogues;
}

void ShopCreate::set_keeper_def_dialogue(const Ref<Dialogues>& p_keeper_def_dialogue) {
    keeper_def_dialogue = p_keeper_def_dialogue;
    if(shop_ui) {
        shop_ui->set_keeper_def_dialogue(p_keeper_def_dialogue);
    }
}

Ref<Dialogues> ShopCreate::get_keeper_def_dialogue() const {
    return keeper_def_dialogue;
}

void ShopCreate::set_can_be_sold_to(bool p_can_be_sold_to) {
    can_be_sold_to = p_can_be_sold_to;
    if(shop_ui) {
        shop_ui->set_can_be_sold_to(p_can_be_sold_to);
    }
}

bool ShopCreate::get_can_be_sold_to() const {
    return can_be_sold_to;
}

void ShopCreate::set_keeper_cannot_sell_dialogues(const Ref<Dialogues>& p_keeper_cannot_sell_dialogues) {
    keeper_cannot_sell_dialogues = p_keeper_cannot_sell_dialogues;
    if(shop_ui) {
        shop_ui->set_keeper_cannot_sell_dialogues(p_keeper_cannot_sell_dialogues);
    }
}

Ref<Dialogues> ShopCreate::get_keeper_cannot_sell_dialogues() const {
    return keeper_cannot_sell_dialogues;
}

void ShopCreate::set_music(const Ref<AudioStream>& p_music) {
    music = p_music;
    if(music_player) {
        music_player->set_stream(music);
    }
}

Ref<AudioStream> ShopCreate::get_music() const {
    return music;
}

void ShopCreate::set_room_id(int p_room_id) {
    room_id = p_room_id;
    if(room_exit) {
        room_exit->set_new_room_entrance_id(room_id);
    }
}

int ShopCreate::get_room_id() const {
    return room_id;
}

void ShopCreate::set_room_path(const String& p_room_path) {
    room_path = p_room_path;
    if(room_exit) {
        room_exit->set_new_room(room_path);
    }
}

String ShopCreate::get_room_path() const {
    return room_path;
}

void ShopCreate::set_dialogue_options(const PackedStringArray& p_dialogue_options) {
    dialogue_option = p_dialogue_options;
    if(shop_ui) {
        shop_ui->set_dialogue_options(p_dialogue_options);
    }
}

PackedStringArray ShopCreate::get_dialogue_options() const {
    return dialogue_option;
}

void ShopCreate::set_click_path(const NodePath& p_click_path) {
    click_path = p_click_path;
    if(keeper_dialogue) {
        keeper_dialogue->set_click(Object::cast_to<AudioStreamPlayer>(get_node_internal(click_path)));
    }
}

NodePath ShopCreate::get_click_path() const {
    return click_path;
}