#ifndef SETTING_TOGGLE_BUTTON_H
#define SETTING_TOGGLE_BUTTON_H

#include<godot_cpp/classes/button.hpp>
namespace godot {
    class SettingToggleButton : public Button {
        GDCLASS(SettingToggleButton, Button);
        
        protected:
            static void _bind_methods();
        
        private:
            String setting_name;
            PackedStringArray values;
            int current_index;
            
        public:
            SettingToggleButton();
            ~SettingToggleButton();

            void init();
            void _on_pressed();

            void set_setting_name(const String& p_setting_name);
            String get_setting_name() const;

            void set_values(const PackedStringArray& p_values);
            PackedStringArray get_values() const;
    };
}

#endif