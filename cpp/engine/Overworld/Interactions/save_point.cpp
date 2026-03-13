#include "save_point.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/resource_loader.hpp>

SavePoint::SavePoint() {}

SavePoint::~SavePoint() {}

void SavePoint::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_interact_save"), &SavePoint::_on_interact_save);
    ClassDB::bind_method(D_METHOD("_on_dialogue_finished"), &SavePoint::_on_dialogue_finished);

    ClassDB::bind_method(D_METHOD("set_dialogue", "dialogue"), &SavePoint::set_dialogue);
    ClassDB::bind_method(D_METHOD("get_dialogue"), &SavePoint::get_dialogue);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "dialogue", PROPERTY_HINT_RESOURCE_TYPE, "Dialogues"), "set_dialogue", "get_dialogue");
}

void SavePoint::_ready() {
    ResourceLoader *loader = ResourceLoader::get_singleton();
    save_menu = loader->load("res://Engine/Overworld/save_menu.tscn");
}

void SavePoint::_on_interact_save() {
    global->heal(global->get_player_max_hp());
    stagehand->audio_player->play("heal");
   
    if(dialogues.is_null()) {
        _on_dialogue_finished();
    }else {
        TextBox *textbox = stagehand->summontextbox();
        textbox->connect("dialogue_finished", Callable(this, "_on_dialogue_finished"), CONNECT_ONE_SHOT);
        textbox->generic(dialogues);
    }
}

void SavePoint::_on_dialogue_finished() {
    Node *current_scene = global->get_scene_container()->get_current_scene();
    
    Node *save_menu_instance = save_menu->instantiate();
    if (save_menu_instance) {
        current_scene->add_child(save_menu_instance);
    }
}

void SavePoint::set_dialogue(Ref<Dialogues> value) {
    dialogues = value;
}

Ref<Dialogues> SavePoint::get_dialogue() const {
    return dialogues;
}