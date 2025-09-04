#include "toggle_setting.h"
#include "env.h"

SettingToggleButton::SettingToggleButton() {
    setting_name = "";
    current_index = 0;
}

SettingToggleButton::~SettingToggleButton() {}

void SettingToggleButton::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init"), &SettingToggleButton::init);
    ClassDB::bind_method(D_METHOD("_on_pressed"), &SettingToggleButton::_on_pressed);
    ClassDB::bind_method(D_METHOD("set_setting_name", "setting_name"), &SettingToggleButton::set_setting_name);
    ClassDB::bind_method(D_METHOD("get_setting_name"), &SettingToggleButton::get_setting_name);
    ClassDB::bind_method(D_METHOD("set_values", "values"), &SettingToggleButton::set_values);
    ClassDB::bind_method(D_METHOD("get_values"), &SettingToggleButton::get_values);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "setting_name"), "set_setting_name", "get_setting_name");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "values"), "set_values", "get_values");
}

void SettingToggleButton::init() {
    if(isEditor) return;
    Dictionary settings = global->get_settings();
    String value = values[0];
    if (settings.has(setting_name)) {
        value = settings[setting_name];
    }else ERR_PRINT(vformat("%s 설정이 존재하지 않습니다", setting_name));
    current_index = values.find(value);
    set_text(value);
}

void SettingToggleButton::_on_pressed() {
    if(!get_tree()->is_paused()) return;
    current_index = (current_index + 1) % values.size();
    String value = values[current_index];
    Dictionary settings = global->get_settings();
    settings[setting_name] = value;
    global->set_settings(settings);
    set_text(value);
}

void SettingToggleButton::set_setting_name(const String& p_setting_name) {
    setting_name = p_setting_name;
}

String SettingToggleButton::get_setting_name() const {
    return setting_name;
}

void SettingToggleButton::set_values(const PackedStringArray& p_values) {
    values = p_values;
}

PackedStringArray SettingToggleButton::get_values() const {
    return values;
}