#include "save_loader.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/time.hpp>

SaveLoader::SaveLoader() {
    reset_level = 0;
}

SaveLoader::~SaveLoader() {}

void SaveLoader::_bind_methods() {
    ADD_SIGNAL(MethodInfo("reset_options"));
    ADD_SIGNAL(MethodInfo("disable"));
    
    ClassDB::bind_method(D_METHOD("load_save"), &SaveLoader::load_save);
    ClassDB::bind_method(D_METHOD("warn"), &SaveLoader::warn);
    ClassDB::bind_method(D_METHOD("reset"), &SaveLoader::reset);
    ClassDB::bind_method(D_METHOD("_hide"), &SaveLoader::_hide);
    ClassDB::bind_method(D_METHOD("_show"), &SaveLoader::_show);
    ClassDB::bind_method(D_METHOD("refresh"), &SaveLoader::refresh);
    ClassDB::bind_method(D_METHOD("_on_reset_accept_pressed"), &SaveLoader::_on_reset_accept_pressed);
}

void SaveLoader::_ready() {
    if(isEditor) return;
    control_node = Object::cast_to<Control>(get_node_internal("Control"));
    defsize = control_node->get_size();
    
    location_text = Object::cast_to<RichTextLabel>(get_node_internal("Control/Texts/Location"));
    name_text = Object::cast_to<RichTextLabel>(get_node_internal("Control/Texts/Name"));
    lv_text = Object::cast_to<RichTextLabel>(get_node_internal("Control/Texts/Lv"));
    time_text = Object::cast_to<RichTextLabel>(get_node_internal("Control/Texts/Time"));
    reset_text = Object::cast_to<OptionSelectableSolo>(get_node_internal("Control/Options/Reset"));
    warn_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("warn"));

    Object::cast_to<OptionSelectableSolo>(get_node_internal("Control/Options/Load"))->set_selected(true);
   
    refresh();
}

void SaveLoader::load_save() {
    emit_signal("disable");

    Dictionary overworld_data = global->get_overworld_data();
    Variant room = overworld_data.has("room") ? overworld_data["room"] : Variant();
    
    String room_path;
    if (room.get_type() == Variant::STRING) {
        room_path = room;
    } else {
        room_path = scene_changer->get_default_scene();
    }
    
    Dictionary empty_dict;
    scene_changer->enter_room_path(room_path, empty_dict);
}

void SaveLoader::warn() {
    warn_sound->play();
    reset_text->set_text(String("[color=red]")+tr("CONTINUE_CONFIRM"));
    reset_level = 1;
}

void SaveLoader::reset() {
    emit_signal("disable");
    global->resetgame();
    global->get_scene_container()->change_scene_to_file("res://Intro/name_selection.tscn");
}

void SaveLoader::_hide() {
    global->set_player_in_menu(false);
    
    Ref<Tween> tween = control_node->create_tween();
    tween->set_parallel(true);
    tween->set_ease(Tween::EASE_OUT);
    tween->set_trans(Tween::TRANS_EXPO);
    
    tween->tween_property(control_node, "size:y", 0.0, 0.6);
    tween->tween_property(control_node, "modulate:a", 0.0, 0.6);
}

void SaveLoader::_show() {
    global->set_player_in_menu(true);
    
    Ref<Tween> tween = control_node->create_tween();
    tween->set_parallel(true);
    tween->set_ease(Tween::EASE_OUT);
    tween->set_trans(Tween::TRANS_EXPO);
    
    tween->tween_property(control_node, "size:y", defsize.y, 0.6);
    tween->tween_property(control_node, "modulate:a", 1.0, 0.6);
}

void SaveLoader::refresh() {
    location_text->set_text(global->get_overworld_data()["room_name"]);
    name_text->set_text(global->get_player_name());
    lv_text->set_text("LV" + String::num_int64(global->get_player_lv()));

    String time_string = Time::get_singleton()->get_time_string_from_unix_time(global->get_cache_playtime());
    time_text->set_text(time_string);
}

void SaveLoader::_on_reset_accept_pressed() {
    if (reset_level) {
        reset();
        return;
    }
    
    warn();
}