#ifndef STAGEHAND_H
#define STAGEHAND_H

#include<godot_cpp/classes/node.hpp>
#include "audio_player.h"
#include "engine/Text/character_setting.h"
#include "engine/Overworld/textBox/text_box.h"
namespace godot {
    class Stagehand : public Node {
        GDCLASS(Stagehand, Node)

        protected:
            static void _bind_methods();
       
        public:
            AudioPlayer* audio_player;
            TextBox* summontextbox();

        private:
            HashMap<String, CharacterSetting*> characters;
            TextBox* textbox;
            Array global_effects;

            void set_property(Variant value);
            AudioPlayer* get_audio_player() const;

        public:
            Stagehand();
            ~Stagehand();
        
            void _ready() override;

            void set_global_effects(Array effects);
            Array get_global_effects() const;
            CharacterSetting* get_character(String name);
    };
}
#endif