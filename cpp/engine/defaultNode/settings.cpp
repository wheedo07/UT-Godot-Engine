#include "settings.h"
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/shader_material.hpp>
using namespace godot;

Settings::Settings() {
    enabled = false;
}

Settings::~Settings() {}

void Settings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("toggle"), &Settings::toggle);
    ClassDB::bind_method(D_METHOD("on_setting_changed", "boolBtn"), &Settings::on_setting_changed);
    ADD_SIGNAL(MethodInfo("init"));
    ADD_SIGNAL(MethodInfo("setting_changed", 
        PropertyInfo(Variant::STRING, "setting_name"),
        PropertyInfo(Variant::NIL, "to")));
}

void Settings::_ready() {
    bool isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    Darken = Object::cast_to<Panel>(get_node_internal("Darken"));
    Blur = Object::cast_to<CanvasItem>(get_node_internal("Blur"));
    BusContainer = Object::cast_to<HBoxContainer>(get_node_internal("BusContainer"));
    AnimPlayer = Object::cast_to<AnimationPlayer>(get_node_internal("AnimationPlayer"));
    BoolOptions = Object::cast_to<VBoxContainer>(get_node_internal("BoolOptions/VBoxContainer"));

    AnimPlayer->set_speed_scale(1.0f / TIME);
    Darken->set_modulate(Color(1, 1, 1, 0));
    Ref<ShaderMaterial> shader = Blur->get_material();
    shader->set_shader_parameter("lod", 0);
    AnimPlayer->play("RESET");

    TypedArray<Node> settings = BoolOptions->get_children();
    for (int i = 0; i < settings.size(); i++) {
        SettingBoolButton* setting = Object::cast_to<SettingBoolButton>(settings[i]);
        if (!setting) continue;
        setting->connect("pressed", Callable(this, "on_setting_changed").bind(setting));
    }
}

void Settings::toggle() {
    enabled = !enabled;
    get_tree()->set_pause(enabled);
    emit_signal("init");
    
    if (tw.is_valid()) {
        tw->kill();
    }

    Input::get_singleton()->set_mouse_mode(
        enabled ? Input::MOUSE_MODE_VISIBLE : Input::MOUSE_MODE_HIDDEN
    );

    tw = create_tween()->set_trans(static_cast<Tween::TransitionType>(TRANSTYPE))
    ->set_ease(static_cast<Tween::EaseType>(enabled ? 1 : 0))
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

    if (enabled) {
        AnimPlayer->play("toggle");
    } else {
        AnimPlayer->play_backwards("toggle");
    }
}

void Settings::_input(const Ref<InputEvent>& event) {
    if (event->is_action_pressed("ui_close")) {
        toggle();
    }
}

void Settings::on_setting_changed(SettingBoolButton* boolBtn) {
    if(!get_tree()->is_paused()) return;
    emit_signal("setting_changed", boolBtn->get_setting_name(), boolBtn->is_pressed());
}