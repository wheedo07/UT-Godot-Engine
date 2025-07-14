#ifndef SETTING_BOOL_BUTTON_H
#define SETTING_BOOL_BUTTON_H

#include<godot_cpp/classes/check_button.hpp>
namespace godot {
    class SettingBoolButton : public CheckButton {
        GDCLASS(SettingBoolButton, CheckButton);
        
        protected:
            static void _bind_methods();
        
        private:
            String setting_name;
        
        public:
            SettingBoolButton();
            ~SettingBoolButton();
        
            void init();
            void _on_toggled(bool val);
        
            void set_setting_name(const String& p_setting_name);
            String get_setting_name() const;
    };
}

#endif