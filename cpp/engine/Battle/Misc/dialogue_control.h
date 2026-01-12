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
            String character_name;
            EnemySpeech* bubble_text;
            bool tween_in_progress;
            Ref<Tween> active_tween;
            double text_duration;
        
            void _on_ends_typing();
            void _on_text_expression_set(Array expr);
            void _on_tween_finished();
            void _on_all_texts_finished();

        public:
            void _ready() override;
            DialogueControl();
            ~DialogueControl();

            void type_text_bubble(const Ref<Dialogues>& dialogues);
            void set_key(bool is);

            void _on_text_click_played(double duration);
            void _on_text_duration_finished();

            void set_character_name(String p_character);
            String get_character_name() const;
    };
}

#endif