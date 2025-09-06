#include "save_menu.h"
#include "env.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/variant/string.hpp>

SaveMenu::SaveMenu() {
    saved = false;
}

SaveMenu::~SaveMenu() {}

void SaveMenu::_bind_methods() {
    ClassDB::bind_method(D_METHOD("dismiss"), &SaveMenu::dismiss);
    ClassDB::bind_method(D_METHOD("save"), &SaveMenu::save);
    ClassDB::bind_method(D_METHOD("refresh"), &SaveMenu::refresh);
    ClassDB::bind_method(D_METHOD("set_saved", "value"), &SaveMenu::set_saved);
    
    ADD_SIGNAL(MethodInfo("_on_save"));
    ADD_SIGNAL(MethodInfo("choiced"));
}

void SaveMenu::_ready() {
    if(isEditor) return;
    control_node = Object::cast_to<Control>(get_node_internal("Control"));
    location_label = Object::cast_to<RichTextLabel>(get_node_internal("Control/Texts/Location"));
    name_label = Object::cast_to<RichTextLabel>(get_node_internal("Control/Texts/Name"));
    lv_label = Object::cast_to<RichTextLabel>(get_node_internal("Control/Texts/Lv"));
    time_label = Object::cast_to<RichTextLabel>(get_node_internal("Control/Texts/Time"));
    return_option = Object::cast_to<OptionSelectableSolo>(get_node_internal("Control/Options/Return"));
    save_option = Object::cast_to<OptionSelectableSolo>(get_node_internal("Control/Options/Save"));
    soul_node = Object::cast_to<MenuSoul>(get_node_internal("Control/Options/MenuSoul"));
    defsize = control_node->get_size();

    global->set_player_in_menu(true);
    sc = global->get_scene_container()->get_current_scene();
    
    if(sc->has_method("_on_saved")) {
        connect("_on_save", Callable(sc, "_on_saved"), CONNECT_ONE_SHOT);
    }
    save_option->set_selected(true);
    set_visible(true);

    if(global->get_first()) return;
    Dictionary overworld_data = global->get_overworld_data();
    location_label->set_text(overworld_data["room_name"]);
    refresh();
}

void SaveMenu::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    if ((event->is_action_pressed("ui_accept") && saved) || event->is_action_pressed("ui_cancel")) {
        dismiss();
        get_viewport()->set_input_as_handled();
    }
}

void SaveMenu::dismiss() {
    emit_signal("choiced");
    
    set_process_unhandled_input(false);
    global->call_deferred("set_player_in_menu", false);
    set_visible(false);
    queue_free();
}

void SaveMenu::save() {
    emit_signal("choiced");
    emit_signal("_on_save");
    
    return_option->set_visible(false);
    save_option->set_text(String("[color=yellow]")+tr("UT_FILE_SAVED"));
    soul_node->set_visible(false);
   
    Dictionary overworld_data = global->get_overworld_data();
    overworld_data["room_name"] = sc->get("world_name");
    global->set_overworld_data(overworld_data);

    global->save_game(false);
    refresh();
    location_label->set_text(sc->get("world_name"));
    
    audio_player->play("save");
    call_deferred("set_saved", true);
}

void SaveMenu::set_saved(bool value) {
    saved = value;
}

void SaveMenu::refresh() {
    if(!global) return;
    
    name_label->set_text(global->get_player_name());
    
    String lv_text = "LV" + String::num_int64(global->get_player_lv());
    lv_label->set_text(lv_text);
    
    String time_text = Time::get_singleton()->get_time_string_from_unix_time(global->get_cache_playtime());
    time_label->set_text(time_text);
}
