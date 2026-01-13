#ifndef ENEMY_SPEECH_H
#define ENEMY_SPEECH_H

#include "engine/Text/advanced_text_typer.h"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/font.hpp>
namespace godot {
    class EnemySpeech : public AdvancedTextTyper {
        GDCLASS(EnemySpeech, AdvancedTextTyper)
        
        protected:
            static void _bind_methods();
        
        private:
            String current_character;
            void handle_confirm_signal();
        
        public:
            EnemySpeech();
            ~EnemySpeech();

            void _ready() override;

            void character_customize();
            void process_next_dialogue();
            void type_text_advanced(const Ref<Dialogues>& dialogues) override;

            void set_current_character(String p_character);
    };
}

#endif