#ifndef AUDIOBUS_SETTINGS_H
#define AUDIOBUS_SETTINGS_H

#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/v_slider.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/audio_server.hpp>
#include<godot_cpp/variant/string.hpp>
#include<godot_cpp/variant/string_name.hpp>
namespace godot {
    class AudiobusSettings : public Control {
        GDCLASS(AudiobusSettings, Control)
        
        protected:
            static void _bind_methods();
        
        private:
            String audio_bus_name;
            StringName audio_bus_key;
            RichTextLabel* Name;
            RichTextLabel* Info;
            VSlider* VolumeSlider;
            int audiobus_id;
            Ref<Tween> tw;
        
        public:
            AudiobusSettings();
            ~AudiobusSettings();
        
            void init();
        
            void _on_volume_slider_value_changed(float value);
            void _on_v_slider_drag_started();
            void _on_v_slider_drag_ended(bool is=false);
            void _on_bypass_toggled(bool toggled_on);
            void _on_mute_toggled(bool toggled_on);
            void _on_solo_toggled(bool toggled_on);
        
            void set_audio_bus_name(const String& p_name);
            String get_audio_bus_name() const;
        
            void set_audio_bus_key(const StringName& p_key);
            StringName get_audio_bus_key() const;
    };
}

#endif