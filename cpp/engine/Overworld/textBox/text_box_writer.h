#ifndef TEXT_BOX_WRITER_H
#define TEXT_BOX_WRITER_H

#include "engine/Text/advanced_text_typer.h"
#include "engine/resources/Dialogues/dialogues.h"
namespace godot {
    class TextBoxWriter : public AdvancedTextTyper {
        GDCLASS(TextBoxWriter, AdvancedTextTyper)
        
        protected:
        static void _bind_methods();
        
        private:
            bool is_last_dialogue = false;
            bool current_dialogue_finished = false;
            bool waiting_for_confirm = false;
        
        public:
            TextBoxWriter();
            ~TextBoxWriter();

            void _ready() override;
            void type_text_advanced(const Ref<Dialogues>& dialogues) override;

            void on_tween_finished_extended();
            void process_next_dialogue();
            void handle_confirm_signal();
    };
}

#endif