#include "bool_setting.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>

SettingBoolButton::SettingBoolButton() {
    setting_name = "";
}

SettingBoolButton::~SettingBoolButton() {
}

void SettingBoolButton::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init"), &SettingBoolButton::init);
    ClassDB::bind_method(D_METHOD("_on_toggled", "val"), &SettingBoolButton::_on_toggled);
    ClassDB::bind_method(D_METHOD("set_setting_name", "setting_name"), &SettingBoolButton::set_setting_name);
    ClassDB::bind_method(D_METHOD("get_setting_name"), &SettingBoolButton::get_setting_name);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "setting_name"), "set_setting_name", "get_setting_name");
}

void SettingBoolButton::init() {
    if(isEditor) return;
    
    Dictionary settings = global->get_settings();
    if (settings.has(setting_name)) {
        Variant value = settings[setting_name];
        set_pressed(value);
    }
}

void SettingBoolButton::_on_toggled(bool val) {
    if(!get_tree()->is_paused()) return;
    Dictionary settings = global->get_settings();
        
    if (settings.has(setting_name)) {
        Variant value = settings[setting_name];
        if (value.get_type() != Variant::BOOL) {
            if (value.get_type() == Variant::NIL) {
                UtilityFunctions::push_error("Setting is NULL, please set it in Global singleton.");
                return;
            }
            UtilityFunctions::push_error("Setting is NOT boolean.");
            return;
        }
        settings[setting_name] = val;
        global->set_settings(settings);
    }
}

void SettingBoolButton::set_setting_name(const String& p_setting_name) {
    setting_name = p_setting_name;
}

String SettingBoolButton::get_setting_name() const {
    return setting_name;
}
