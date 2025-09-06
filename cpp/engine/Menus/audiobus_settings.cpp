#include "audiobus_settings.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/core/math.hpp>

AudiobusSettings::AudiobusSettings() {
    audio_bus_name = "UT_SETTING_MASTER";
    audio_bus_key = "Master";
}

AudiobusSettings::~AudiobusSettings() {}

void AudiobusSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init"), &AudiobusSettings::init);
    ClassDB::bind_method(D_METHOD("set_audio_bus_name", "name"), &AudiobusSettings::set_audio_bus_name);
    ClassDB::bind_method(D_METHOD("get_audio_bus_name"), &AudiobusSettings::get_audio_bus_name);
    ClassDB::bind_method(D_METHOD("set_audio_bus_key", "key"), &AudiobusSettings::set_audio_bus_key);
    ClassDB::bind_method(D_METHOD("get_audio_bus_key"), &AudiobusSettings::get_audio_bus_key);
    
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "audio_bus_name"), "set_audio_bus_name", "get_audio_bus_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "audio_bus_key"), "set_audio_bus_key", "get_audio_bus_key");
    
    ClassDB::bind_method(D_METHOD("_on_volume_slider_value_changed", "value"), &AudiobusSettings::_on_volume_slider_value_changed);
    ClassDB::bind_method(D_METHOD("_on_v_slider_drag_started"), &AudiobusSettings::_on_v_slider_drag_started);
    ClassDB::bind_method(D_METHOD("_on_v_slider_drag_ended", "is"), &AudiobusSettings::_on_v_slider_drag_ended, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("_on_bypass_toggled", "toggled_on"), &AudiobusSettings::_on_bypass_toggled);
    ClassDB::bind_method(D_METHOD("_on_mute_toggled", "toggled_on"), &AudiobusSettings::_on_mute_toggled);
    ClassDB::bind_method(D_METHOD("_on_solo_toggled", "toggled_on"), &AudiobusSettings::_on_solo_toggled);
}

void AudiobusSettings::init() {
    if(isEditor) return;
    Name = Object::cast_to<RichTextLabel>(get_node_internal("Margin/Name"));
    Info = Object::cast_to<RichTextLabel>(get_node_internal("Margin/Info"));
    VolumeSlider = Object::cast_to<VSlider>(get_node_internal("Margin/VSlider"));
    audiobus_id = AudioServer::get_singleton()->get_bus_index(audio_bus_key);

    Name->set_text("[center]" + tr(audio_bus_name));
    
    Dictionary settings = global->get_settings();
    
    double volume_value = 100;
    if (settings.has(audio_bus_key)) {
        volume_value = static_cast<double>(settings[audio_bus_key]);
    }
    VolumeSlider->set_value_no_signal(volume_value);
}

void AudiobusSettings::_on_volume_slider_value_changed(float value) {
    if(!get_tree()->is_paused()) return;
    Vector2 slider_pos = VolumeSlider->get_position();
    Vector2 slider_size = VolumeSlider->get_size();
    
    float normalized_value = 1.0f - VolumeSlider->get_value() / VolumeSlider->get_max();
    float interpolated_y = Math::lerp(normalized_value, 0.5f, 0.3f) * slider_size.y - 10.0f;
    
    Info->set_position(Vector2(
        slider_pos.x + slider_size.x / 2.0f + 20.0f,
        interpolated_y + slider_pos.y
    ));

    double v = UtilityFunctions::linear_to_db(value / 100.0f);
    AudioServer::get_singleton()->set_bus_volume_db(
        audiobus_id, 
        v
    );
    
    Info->set_text(vformat("%3.0f", v));
    
    Dictionary settings = global->get_settings();
    settings[audio_bus_key] = value;
    global->set_settings(settings);
    
    _on_v_slider_drag_started();
    
    if (tw.is_valid()) {
        tw->kill();
    }
    
    tw = create_tween();
    tw->tween_callback(Callable(this, "_on_v_slider_drag_ended"))->set_delay(0.5);
}

void AudiobusSettings::_on_v_slider_drag_started() {
    Info->show();
}

void AudiobusSettings::_on_v_slider_drag_ended(bool is) {
    Info->hide();
}

void AudiobusSettings::_on_bypass_toggled(bool toggled_on) {
    AudioServer::get_singleton()->set_bus_bypass_effects(audiobus_id, toggled_on);
}

void AudiobusSettings::_on_mute_toggled(bool toggled_on) {
    AudioServer::get_singleton()->set_bus_mute(audiobus_id, toggled_on);
}

void AudiobusSettings::_on_solo_toggled(bool toggled_on) {
    AudioServer::get_singleton()->set_bus_solo(audiobus_id, toggled_on);
}

void AudiobusSettings::set_audio_bus_name(const String& p_name) {
    audio_bus_name = p_name;
}

String AudiobusSettings::get_audio_bus_name() const {
    return audio_bus_name;
}

void AudiobusSettings::set_audio_bus_key(const StringName& p_key) {
    audio_bus_key = p_key;
}

StringName AudiobusSettings::get_audio_bus_key() const {
    return audio_bus_key;
}