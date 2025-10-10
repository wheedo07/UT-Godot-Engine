#ifndef STAGEHAND_H
#define STAGEHAND_H

#include<godot_cpp/classes/node.hpp>
#include "audio_player.h"
#include "engine/Text/character_setting.h"
namespace godot {

    class Stagehand : public Node {
        GDCLASS(Stagehand, Node)

        protected:
            static void _bind_methods();
       
        public:
            AudioPlayer* audio_player;
            HashMap<String, CharacterSetting*> characters;

        private:
            void set_property(Variant value);
            AudioPlayer* get_audio_player() const;

        public:
            Stagehand();
            ~Stagehand();
        
            void _ready() override;

            CharacterSetting* get_character(String name);
    };
}
#endif