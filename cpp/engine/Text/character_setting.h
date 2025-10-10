#ifndef CHARACTER_SETTING_H
#define CHARACTER_SETTING_H

#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/font.hpp>
namespace godot {
    class CharacterSetting : public AudioStreamPlayer {
        GDCLASS(CharacterSetting, AudioStreamPlayer)

        protected:
            static void _bind_methods();

        private:
            Ref<Font> font;
            float text_size;
            String extra_delay;
            String no_sound;
        
        public:
            CharacterSetting();
            ~CharacterSetting();

            void set_font(const Ref<Font>& value);
            Ref<Font> get_font() const;

            void set_text_size(float value);
            float get_text_size() const;

            void set_extra_delay(String value);
            String get_extra_delay();

            void set_no_sound(String value);
            String get_no_sound();
    };
}

#endif