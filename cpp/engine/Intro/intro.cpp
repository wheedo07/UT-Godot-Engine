#include "intro.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/texture2d.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/file_access.hpp>
#include<godot_cpp/classes/json.hpp>
#include<godot_cpp/classes/translation_server.hpp>

Intro::Intro() {
    skip_intro = false;
    current_index = 0;
    intro_completed = false;
    intro_data = Array();
    intro_json_path = "res://Intro/intro_data.json";
    intro_completed_path = "";
    enable_auto = true;
}

Intro::~Intro() {}

void Intro::_bind_methods() {
    GDVIRTUAL_BIND(ready);
    ClassDB::bind_method(D_METHOD("next"), &Intro::next);
    ClassDB::bind_method(D_METHOD("is_intro_completed"), &Intro::is_intro_completed);

    ClassDB::bind_method(D_METHOD("_play_intro"), &Intro::_play_intro);
    ClassDB::bind_method(D_METHOD("_on_intro_completed", "skipped"), &Intro::_on_intro_completed);
    ClassDB::bind_method(D_METHOD("_intro_image_next"), &Intro::_intro_image_next);
    ClassDB::bind_method(D_METHOD("_on_text_completed"), &Intro::_on_text_completed);
    ClassDB::bind_method(D_METHOD("_on_next"), &Intro::_on_next);

    ClassDB::bind_method(D_METHOD("set_intro_json_path", "path"), &Intro::set_intro_json_path);
    ClassDB::bind_method(D_METHOD("get_intro_json_path"), &Intro::get_intro_json_path);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "intro_json_path", PROPERTY_HINT_FILE, "*.json"), "set_intro_json_path", "get_intro_json_path");

    ClassDB::bind_method(D_METHOD("set_intro_completed_path", "path"), &Intro::set_intro_completed_path);
    ClassDB::bind_method(D_METHOD("get_intro_completed_path"), &Intro::get_intro_completed_path);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "intro_completed_path", PROPERTY_HINT_FILE, "*.tscn"), "set_intro_completed_path", "get_intro_completed_path");

    ClassDB::bind_method(D_METHOD("set_music", "music"), &Intro::set_music);
    ClassDB::bind_method(D_METHOD("get_music"), &Intro::get_music);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "music", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_music", "get_music");

    ClassDB::bind_method(D_METHOD("set_enable_auto", "enable"), &Intro::set_enable_auto);
    ClassDB::bind_method(D_METHOD("get_enable_auto"), &Intro::get_enable_auto);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_auto"), "set_enable_auto", "get_enable_auto");
    
    ADD_SIGNAL(MethodInfo("intro_completed", PropertyInfo(Variant::BOOL, "skipped")));
    ADD_SIGNAL(MethodInfo("started_intro", PropertyInfo(Variant::INT, "index")));
}

void Intro::_ready() {
    if(!music.is_null()) {
        global->get_Music()->set_stream(music);
        global->get_Music()->play();
    }
    intro_text = Object::cast_to<GenericTextTyper>(get_node_internal("IntroText"));
    intro_image = Object::cast_to<TextureRect>(get_node_internal("Panel"));
    
    camera = global->get_scene_container()->get_camera();
    intro_text->set_process_unhandled_input(false);
    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    } else {
        ready();
    }
    _load_intro_data_from_json();
    _play_intro();
}

void Intro::ready() {}

void Intro::_input(const Ref<InputEvent>& event) {
    if (event->is_action_pressed("ui_accept")) {
        skip_intro = true;
        intro_text->kill_tweens(true);
    }
}

void Intro::_play_intro() {
    if (intro_data.size() == 0) {
        intro_completed = true;
        emit_signal("intro_completed", skip_intro);
        return;
    }
    Dictionary data = intro_data[current_index];
    
    if (data.has("image")) {
        Ref<Texture2D> img = data["image"];
        intro_image->set_texture(img);
        intro_image->set_visible(true);
    }else {
        intro_image->set_visible(false);
    }
    
    process_next_intro();
}

void Intro::process_next_intro() {
    if (current_index >= intro_data.size() || skip_intro) {
        intro_completed = true;
        emit_signal("intro_completed", skip_intro);
        return;
    }
    emit_signal("started_intro", current_index);
    Ref<Tween> tw = create_tween();
    tw->tween_property(intro_image, "modulate:a", 1, 0.15);
    
    Dictionary data = intro_data[current_index];
    
    String text = data["text"];
    float speed = data["speed"];

    intro_text->kill_tweens();
    
    if(!text.is_empty()) {
        intro_text->set_interval(speed);
        
        intro_text->connect("finished_all_texts", Callable(this, "_on_text_completed"), CONNECT_ONE_SHOT);
        intro_text->type_text({ text });
    } else {
        intro_text->set_text("");
        _on_text_completed();
    }
}

void Intro::_on_text_completed() {
    if (skip_intro) {
        intro_completed = true;
        emit_signal("intro_completed", skip_intro);
        return;
    }
    if(!enable_auto) return;
    Dictionary data = intro_data[current_index];

    SceneTree* tree = get_tree();
    Ref<SceneTreeTimer> timer = tree->create_timer(data.get("duration", 0), false);
    timer->connect("timeout", Callable(this, "_on_next"), CONNECT_ONE_SHOT);
}

void Intro::next() {
    if(enable_auto) return;
    _on_next();
}

void Intro::_on_next() {
    Ref<Tween> tw = create_tween();
    tw->tween_property(intro_image, "modulate:a", 0, 0.2); 
    tw->connect("finished", Callable(this, "_intro_image_next"), CONNECT_ONE_SHOT);
}

void Intro::_intro_image_next() {
    current_index++;
    if (current_index >= intro_data.size() || skip_intro) {
        intro_completed = true;
        emit_signal("intro_completed", skip_intro);
        return;
    }

    Dictionary data = intro_data[current_index];
    if (data.has("image")) {
        Ref<Texture2D> img = data["image"];
        intro_image->set_texture(img);
        intro_image->set_visible(true);
    }else {
        intro_image->set_visible(false);
    }
    process_next_intro();
}

void Intro::_on_intro_completed(bool skipped) {
    if(intro_completed_path.is_empty()) return;
    camera->blind(0.6, 1);
    camera->connect("finished_tween", Callable(global->get_Music(), "stop"), CONNECT_ONE_SHOT);
    camera->connect("finished_tween", Callable(camera, "blind").bind(0.1, 0), CONNECT_ONE_SHOT);
    camera->connect("finished_tween", Callable(global->get_scene_container(), "change_scene_to_file").bind(intro_completed_path), CONNECT_ONE_SHOT);
}

bool Intro::is_intro_completed() const {
    return intro_completed;
}

void Intro::_load_intro_data_from_json() {
    if (!FileAccess::file_exists(intro_json_path)) {
        ERR_PRINT("json 파일을 찾을수 없습니다");
        return;
    }
    
    Ref<FileAccess> file = FileAccess::open(intro_json_path, FileAccess::READ);
    if (file.is_null()) {
        return;
    }
    
    String json_text = file->get_as_text();
    String locale = TranslationServer::get_singleton()->get_locale();
    if(locale.find("_") != -1) locale = locale.get_slice("_", 0);
    
    intro_data = JSON::parse_string(json_text);
    for(int i = 0; i < intro_data.size(); i++) {
        Dictionary entry = intro_data[i];
        if (entry.has("image_path")) {
            String image_path = entry["image_path"];
            Ref<Texture2D> texture = ResourceLoader::get_singleton()->load(image_path);
            if (texture.is_valid()) {
                entry["image"] = texture;
            }
        }
        
        float default_speed = 0.05;
        if(entry.has("speed")) {
            Dictionary speed = entry.get("speed", Dictionary());
            if(speed.has(locale)) {
                default_speed = speed[locale];
            }else if(speed.has("en")) {
                default_speed = speed["en"];
            }
            entry["speed"] = default_speed;
        }else if(!entry.has("speed")) {
            entry["speed"] = default_speed;
        }
        
        if(entry.has("text")) {
            Dictionary text_dict = entry["text"];
            String localized_text = "";
            
            if(text_dict.has(locale)) {
                localized_text = text_dict[locale];
            }else if(text_dict.has("en")) {
                localized_text = text_dict["en"];
            }
            
            entry["text"] = localized_text;
        }else if(!entry.has("text")) {
            entry["text"] = "";
        }

        intro_data[i] = entry;
    }
}

void Intro::set_music(const Ref<AudioStream>& p_music) {
    music = p_music;
}

Ref<AudioStream> Intro::get_music() const {
    return music;
}

void Intro::set_intro_json_path(const String& path) {
    intro_json_path = path;
}

String Intro::get_intro_json_path() const {
    return intro_json_path;
}

void Intro::set_intro_completed_path(const String& path) {
    intro_completed_path = path;
}

String Intro::get_intro_completed_path() {
    return intro_completed_path;
}

void Intro::set_enable_auto(bool p_enable) {
    enable_auto = p_enable;
}

bool Intro::get_enable_auto() const {
    return enable_auto;
}