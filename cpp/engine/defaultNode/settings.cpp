#include "settings.h"
#include "env.h"
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/shader_material.hpp>

#define TIME 0.6f
Settings::Settings() {
    enabled = false;
}

Settings::~Settings() {}

void Settings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("toggle"), &Settings::toggle);
    ClassDB::bind_method(D_METHOD("on_setting_changed", "btn"), &Settings::on_setting_changed);
    ClassDB::bind_method(D_METHOD("_scene_input", "text"), &Settings::_scene_input);
    ClassDB::bind_method(D_METHOD("_change_process", "value"), &Settings::_change_process);
    ADD_SIGNAL(MethodInfo("init"));
    ADD_SIGNAL(MethodInfo("setting_changed", 
        PropertyInfo(Variant::STRING, "setting_name"),
        PropertyInfo(Variant::NIL, "to")));
}

void Settings::_ready() {
    Darken = Object::cast_to<Panel>(get_node_internal("Darken"));
    Blur = Object::cast_to<CanvasItem>(get_node_internal("Blur"));
    BusContainer = Object::cast_to<HBoxContainer>(get_node_internal("BusContainer"));
    AnimPlayer = Object::cast_to<AnimationPlayer>(get_node_internal("AnimationPlayer"));
    Options = Object::cast_to<VBoxContainer>(get_node_internal("Options/VBoxContainer"));
    process_edit = Object::cast_to<SpinBox>(get_node_internal("Options2/NinePatchRect/process_edit"));

    AnimPlayer->set_speed_scale(1.0f / TIME);
    Darken->set_modulate(Color(1, 1, 1, 0));
    Ref<ShaderMaterial> shader = Blur->get_material();
    shader->set_shader_parameter("lod", 0);
    AnimPlayer->play("RESET");

    TypedArray<Node> settings = Options->get_children();
    for(int i=0; i < settings.size(); i++) {
        Node* setting = Object::cast_to<Node>(settings[i]);
        setting->connect("pressed", Callable(this, "on_setting_changed").bind(setting));
    }
}

void Settings::toggle() {
    enabled = !enabled;
    global->isSetting = enabled;
    process_edit->set_value(Engine::get_singleton()->get_time_scale());
    emit_signal("init");
    
    if (tw.is_valid()) {
        tw->kill();
    }

    Input::get_singleton()->set_mouse_mode(
        enabled ? Input::MOUSE_MODE_VISIBLE : Input::MOUSE_MODE_HIDDEN
    );

    tw = create_tween()->set_trans(TRANSTYPE)
    ->set_ease(Tween::EaseType(enabled ? 1 : 0))
    ->set_parallel(true);

    tw->tween_property(
        Blur,
        "material:shader_parameter/lod",
        enabled ? 1 : 0,
        TIME
    );

    tw->tween_property(
        Darken,
        "modulate:a",
        enabled ? 1 : 0,
        TIME
    );

    if(enabled) {
        AnimPlayer->play("toggle");
    } else {
        AnimPlayer->play_backwards("toggle");
    }
}

void Settings::_unhandled_input(const Ref<InputEvent>& event) {
    if(!global) return;
    if(event->is_action_pressed("ui_setting") && global->get_debugmode()) {
        toggle();
    }
}

void Settings::on_setting_changed(Node* btn) {
    if(!enabled) return;
    emit_signal("setting_changed", btn->call("get_setting_name"), btn->call("is_pressed"));
}

void Settings::_scene_input(String text) {
    if(!enabled || !global) return;
    ResourceLoader* loader = ResourceLoader::get_singleton();
    if(loader->exists(text)) {
        Ref<Resource> res = loader->load(text);
        if(res.is_null()) {
            global->alert(String::utf8("해당 경로에 리소스가 없습니다."), "Error");
            return;
        }
        String type = res->get_class();
        if(type == "Encounter") {
            scene_changer->load_battle(res, false);
        }else if(type == "PackedScene") {
            global->get_scene_container()->change_scene_to_file(text);
        }else {
            global->alert(String::utf8("해당 경로의 리소스는 씬이나 인카운터가 아닙니다."), "Error");
        }
    }else {
        global->alert(String::utf8("해당 경로에 리소스가 없습니다."), "Error");
    }
}

void Settings::_change_process(double value) {
    if(!enabled || !global) return;
    Engine::get_singleton()->set_time_scale(value);
}