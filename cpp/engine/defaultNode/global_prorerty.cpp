#include "global.h"
using namespace godot;

void Global::_bind_methods() {
    ADD_SIGNAL(MethodInfo("saved"));
    ADD_SIGNAL(MethodInfo("fullscreen_toggled", PropertyInfo(Variant::BOOL, "to")));

    ClassDB::bind_method(D_METHOD("alert", "text", "title"), &Global::alert, DEFVAL("Alert!"));
    ClassDB::bind_method(D_METHOD("item_use_text", "item_id"), &Global::item_use_text);
    ClassDB::bind_method(D_METHOD("equip_item", "item_id"), &Global::equip_item);
    ClassDB::bind_method(D_METHOD("heal", "amt"), &Global::heal);
    ClassDB::bind_method(D_METHOD("toggle_fullscreen"), &Global::toggle_fullscreen);
    ClassDB::bind_method(D_METHOD("resetgame"), &Global::resetgame);
    ClassDB::bind_method(D_METHOD("save_game", "is_sys"), &Global::save_game);
    ClassDB::bind_method(D_METHOD("save_settings"), &Global::save_settings);
    ClassDB::bind_method(D_METHOD("load_game"), &Global::load_game);
    ClassDB::bind_method(D_METHOD("refresh_audio_busses"), &Global::refresh_audio_busses);
    ClassDB::bind_method(D_METHOD("toggle_collision_shape_visibility"), &Global::toggle_collision_shape_visibility);
    ClassDB::bind_method(D_METHOD("_on_kr_tick"), &Global::_on_kr_tick);
    ClassDB::bind_method(D_METHOD("check_level_up"), &Global::check_level_up);
    ClassDB::bind_method(D_METHOD("loop_Music"), &Global::loop_Music);
    
    ClassDB::bind_method(D_METHOD("set_item_list", "value"), &Global::set_item_list);
    ClassDB::bind_method(D_METHOD("get_item_list"), &Global::get_item_list);

    ClassDB::bind_method(D_METHOD("set_savepath", "value"), &Global::set_savepath);
    ClassDB::bind_method(D_METHOD("get_savepath"), &Global::get_savepath);
    
    ClassDB::bind_method(D_METHOD("set_boxesinmenu", "value"), &Global::set_boxesinmenu);
    ClassDB::bind_method(D_METHOD("get_boxesinmenu"), &Global::get_boxesinmenu);
    
    ClassDB::bind_method(D_METHOD("set_unlockedboxes", "value"), &Global::set_unlockedboxes);
    ClassDB::bind_method(D_METHOD("get_unlockedboxes"), &Global::get_unlockedboxes);
    
    ClassDB::bind_method(D_METHOD("set_equipment", "value"), &Global::set_equipment);
    ClassDB::bind_method(D_METHOD("get_equipment"), &Global::get_equipment);
    
    ClassDB::bind_method(D_METHOD("set_cells", "value"), &Global::set_cells);
    ClassDB::bind_method(D_METHOD("get_cells"), &Global::get_cells);
    
    ClassDB::bind_method(D_METHOD("set_items", "value"), &Global::set_items);
    ClassDB::bind_method(D_METHOD("get_items"), &Global::get_items);
    
    ClassDB::bind_method(D_METHOD("set_boxitems", "value"), &Global::set_boxitems);
    ClassDB::bind_method(D_METHOD("get_boxitems"), &Global::get_boxitems);
    
    ClassDB::bind_method(D_METHOD("set_settings", "value"), &Global::set_settings);
    ClassDB::bind_method(D_METHOD("get_settings"), &Global::get_settings);
    
    ClassDB::bind_method(D_METHOD("set_player_name", "value"), &Global::set_player_name);
    ClassDB::bind_method(D_METHOD("get_player_name"), &Global::get_player_name);
    
    ClassDB::bind_method(D_METHOD("set_player_gold", "value"), &Global::set_player_gold);
    ClassDB::bind_method(D_METHOD("get_player_gold"), &Global::get_player_gold);
    
    ClassDB::bind_method(D_METHOD("set_player_lv", "value"), &Global::set_player_lv);
    ClassDB::bind_method(D_METHOD("get_player_lv"), &Global::get_player_lv);
    
    ClassDB::bind_method(D_METHOD("set_player_exp", "value"), &Global::set_player_exp);
    ClassDB::bind_method(D_METHOD("get_player_exp"), &Global::get_player_exp);
    
    ClassDB::bind_method(D_METHOD("set_player_hp", "value"), &Global::set_player_hp);
    ClassDB::bind_method(D_METHOD("get_player_hp"), &Global::get_player_hp);
    
    ClassDB::bind_method(D_METHOD("set_player_max_hp", "value"), &Global::set_player_max_hp);
    ClassDB::bind_method(D_METHOD("get_player_max_hp"), &Global::get_player_max_hp);
    
    ClassDB::bind_method(D_METHOD("set_player_defense", "value"), &Global::set_player_defense);
    ClassDB::bind_method(D_METHOD("get_player_defense"), &Global::get_player_defense);
    
    ClassDB::bind_method(D_METHOD("set_player_attack", "value"), &Global::set_player_attack);
    ClassDB::bind_method(D_METHOD("get_player_attack"), &Global::get_player_attack);
    
    ClassDB::bind_method(D_METHOD("set_player_kr", "value"), &Global::set_player_kr);
    ClassDB::bind_method(D_METHOD("get_player_kr"), &Global::get_player_kr);
    
    ClassDB::bind_method(D_METHOD("set_player_kills", "value"), &Global::set_player_kills);
    ClassDB::bind_method(D_METHOD("get_player_kills"), &Global::get_player_kills);

    ClassDB::bind_method(D_METHOD("set_flags", "value"), &Global::set_flags);
    ClassDB::bind_method(D_METHOD("get_flags"), &Global::get_flags);

    // gdscript에서 호출을 위한 메서드 바인딩
    ClassDB::bind_method(D_METHOD("get_player_in_menu"), &Global::get_player_in_menu);
    ClassDB::bind_method(D_METHOD("set_player_in_menu", "value"), &Global::set_player_in_menu);

    ClassDB::bind_method(D_METHOD("get_player_can_move"), &Global::get_player_can_move);
    ClassDB::bind_method(D_METHOD("set_player_can_move", "value"), &Global::set_player_can_move);

    ClassDB::bind_method(D_METHOD("get_player_set_menu"), &Global::get_player_set_menu);
    ClassDB::bind_method(D_METHOD("set_player_set_menu", "value"), &Global::set_player_set_menu);

    ClassDB::bind_method(D_METHOD("set_player_move", "value"), &Global::set_player_move);
    ClassDB::bind_method(D_METHOD("get_player_move"), &Global::get_player_move);

    ClassDB::bind_method(D_METHOD("get_battle_text_box"), &Global::get_battle_text_box);
    ClassDB::bind_method(D_METHOD("set_battle_text_box", "value"), &Global::set_battle_text_box);

    ClassDB::bind_method(D_METHOD("get_player_text_box"), &Global::get_player_text_box);
    ClassDB::bind_method(D_METHOD("set_player_text_box", "value"), &Global::set_player_text_box);

    ClassDB::bind_method(D_METHOD("save_flag", "key", "value"), &Global::save_flag);
    ClassDB::bind_method(D_METHOD("set_flag", "key", "value"), &Global::set_flag);
    ClassDB::bind_method(D_METHOD("get_flag", "key", "defaultValue"), &Global::get_flag, DEFVAL(false));

    ClassDB::bind_method(D_METHOD("get_g_flags", "key", "defaultValue"), &Global::get_g_flags, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("set_g_flags", "key", "value"), &Global::set_g_flags);

    ClassDB::bind_method(D_METHOD("get_frist"), &Global::get_first);
    ClassDB::bind_method(D_METHOD("get_scene_container"), &Global::get_scene_container);
    ClassDB::bind_method(D_METHOD("get_fullscreen"), &Global::get_fullscreen);
    ClassDB::bind_method(D_METHOD("get_Music"), &Global::get_Music);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "item_list", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":Item")
    , "set_item_list", "get_item_list");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "flags"), "set_flags", "get_flags");

    // 멤버 변수 프로퍼티 등록
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "savepath"), "set_savepath", "get_savepath");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "boxesinmenu"), "set_boxesinmenu", "get_boxesinmenu");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "equipment"), "set_equipment", "get_equipment");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "cells"), "set_cells", "get_cells");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_ARRAY_TYPE, String::num(Variant::INT) + ":"), "set_items", "get_items");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "boxitems"), "set_boxitems", "get_boxitems");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "settings"), "set_settings", "get_settings");
    
    // 플레이어 스탯 프로퍼티
    ADD_GROUP("Player Stats", "player_");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "player_name"), "set_player_name", "get_player_name");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "player_gold"), "set_player_gold", "get_player_gold");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "player_lv"), "set_player_lv", "get_player_lv");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "player_exp"), "set_player_exp", "get_player_exp");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "player_hp"), "set_player_hp", "get_player_hp");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "player_max_hp"), "set_player_max_hp", "get_player_max_hp");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "player_defense"), "set_player_defense", "get_player_defense");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "player_attack"), "set_player_attack", "get_player_attack");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "player_kills"), "set_player_kills", "get_player_kills");
}

void Global::set_first(bool value) {
    first = value;
}

bool Global::get_first() const {
    return first;
}

void Global::set_fullscreen(bool value) {
    fullscreen = value;
}

bool Global::get_fullscreen() const {
    return fullscreen;
}

void Global::set_debugmode(bool value) {
    debugmode = value;
}

bool Global::get_debugmode() const {
    return debugmode;
}

void Global::set_battle_text_box(bool value) {
    battle_text_box = value;
}

bool Global::get_battle_text_box() const {
    return battle_text_box;
}

void Global::set_battle_start(bool value) {
    battle_start = value;
}

bool Global::get_battle_start() const {
    return battle_start;
}

void Global::set_boxesinmenu(bool value) {
    boxesinmenu = value;
}

bool Global::get_boxesinmenu() const {
    return boxesinmenu;
}

void Global::set_unlockedboxes(int value) {
    unlockedboxes = value;
}

int Global::get_unlockedboxes() const {
    return unlockedboxes;
}

void Global::set_equipment(const Dictionary& value) {
    equipment = value;
}

Dictionary Global::get_equipment() const {
    return equipment;
}

void Global::set_cells(const Array& value) {
    cells = value;
}

Array Global::get_cells() const {
    return cells;
}

void Global::set_items(const Array& value) {
    items = value;
}

Array Global::get_items() const {
    return items;
}

void Global::set_boxitems(const Array& value) {
    boxitems = value;
}

Array Global::get_boxitems() const {
    return boxitems;
}

void Global::set_settings(const Dictionary& value) {
    settings = value;
}

Dictionary Global::get_settings() const {
    return settings;
}

void Global::set_player_in_menu(bool value) {
    player_in_menu = value;
}

bool Global::get_player_in_menu() const {
    return player_in_menu || player_set_menu;
}

void Global::set_player_can_move(bool value) {
    player_can_move = value;
}

bool Global::get_player_can_move() const {
    return player_can_move;
}

void Global::set_player_move(bool value) {
    player_move = value;
}

bool Global::get_player_move() const {
    return player_move;
}

void Global::set_player_text_box(bool value) {
    player_text_box = value;
}

bool Global::get_player_text_box() const {
    return player_text_box;
}

void Global::set_paused(bool value) {
    paused = value;
}

bool Global::get_paused() const {
    return paused;
}

void Global::set_player_name(const String& value) {
    player_name = value;
}

String Global::get_player_name() const {
    return player_name;
}

void Global::set_player_gold(int value) {
    player_gold = value;
}

int Global::get_player_gold() const {
    return player_gold;
}

void Global::set_player_lv(int value) {
    player_lv = value;
}

int Global::get_player_lv() const {
    return player_lv;
}

void Global::set_player_exp(int value) {
    player_exp = value;
}

int Global::get_player_exp() const {
    return player_exp;
}

void Global::set_player_hp(int value) {
    player_hp = value;
}

int Global::get_player_hp() const {
    return player_hp;
}

void Global::set_player_max_hp(int value) {
    player_max_hp = value;
}

int Global::get_player_max_hp() const {
    return player_max_hp;
}

void Global::set_player_defense(int value) {
    player_defense = value;
}

int Global::get_player_defense() const {
    return player_defense;
}

void Global::set_player_attack(int value) {
    player_attack = value;
}

int Global::get_player_attack() const {
    return player_attack;
}

void Global::set_player_kr(int value) {
    player_kr = value;
}

int Global::get_player_kr() const {
    return player_kr;
}

void Global::set_player_kills(int value) {
    player_kills = value;
}

int Global::get_player_kills() const {
    return player_kills;
}

void Global::set_item_list(TypedArray<Item> value) {
    item_list = value;
}

TypedArray<Item> Global::get_item_list() const {
    return item_list;
}

void Global::set_scene_container(SceneContainer* value) {
    scene_container = value;
}

SceneContainer* Global::get_scene_container() const {
    return scene_container;
}

AudioStreamPlayer* Global::get_Music() {
    return Music;
}

void Global::set_just_died(bool value) {
    just_died = value;
}
			
bool Global::get_just_died() {
    return just_died;
}

void Global::set_flags(Dictionary value) {
    flags = value;
}

Dictionary Global::get_flags() {
    return flags;
}

void Global::set_flag(String key, Variant value) {
    flags_tmp[key] = value;
}

Variant Global::get_flag(String key, Variant defaultValue) {
    if(flags_tmp.has(key)) {
        return flags_tmp[key];
    }else if(flags.has(key)) {
        return flags[key];
    }else return defaultValue;
}

void Global::set_player_position(Vector2 value) {
    player_position = value;
}

Vector2 Global::get_player_position() {
    return player_position;
}

void Global::set_overworld_data(Dictionary value) {
    overworld_data = value;
}

Dictionary Global::get_overworld_data() {
    return overworld_data;
}

void Global::set_cache_playtime(double value) {
    cache_playtime = value;
}

double Global::get_cache_playtime() {
    return cache_playtime;
}

void Global::set_temp_atk(int value) {
    temp_atk = value;
}

int Global::get_temp_atk() {
    return temp_atk;
}

void Global::set_temp_def(int value) {
    temp_def = value;
}

int Global::get_temp_def() {
    return temp_def;
}

void Global::set_savepath(String value) {
    savepath = value;
}

String Global::get_savepath() {
    return savepath;
}

void Global::set_g_flags(String key, Variant value) {
    g_flags[key] = value;
    save_settings();
}

Variant Global::get_g_flags(String key, Variant defaultValue) {
    if(g_flags.has(key)) {
        return g_flags[key];
    }else {
        return defaultValue;
    }
}

void Global::set_player_set_menu(bool value) {
    player_set_menu = value;
}

bool Global::get_player_set_menu() const {
    return player_set_menu;
}