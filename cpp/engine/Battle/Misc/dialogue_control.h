#ifndef DIALOGUE_CONTROL_H
#define DIALOGUE_CONTROL_H

#include "enemy_speech.h"
#include<godot_cpp/classes/texture_rect.hpp>
#include<godot_cpp/classes/tween.hpp>
namespace godot {
    class DialogueControl : public TextureRect {
        GDCLASS(DialogueControl, TextureRect)
        
        protected:
            static void _bind_methods();
        
        private:
            Character custom_character;
            int text_size;
            EnemySpeech* bubble_text;
            bool tween_in_progress;
            Ref<Tween> active_tween;
            double text_duration;
        
            void _on_ends_typing();

        public:
            void _ready() override;
            DialogueControl();
            ~DialogueControl();

            void DialogueText(const Ref<Dialogues>& dialogues);
            void on_text_click_played(bool is=true, double duration=0);
            void set_key(bool is);

            void _on_text_expression_set(const Array& expr);
            void _on_tween_finished();
            void _on_all_texts_finished();

            void set_custom_character(Character p_character);
            Character get_custom_character() const;

            void set_text_size(int size);
            int get_text_size();
    };
}

#endif