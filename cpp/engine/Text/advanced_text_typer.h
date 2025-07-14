#ifndef ADVANCED_TEXT_TYPER_H
#define ADVANCED_TEXT_TYPER_H

#include "text_typer.h"
#include "engine/resources/Dialogues/pause.h"
#include "engine/resources/Dialogues/dialogues.h"
#include<godot_cpp/classes/interval_tweener.hpp>
namespace godot {
    class AdvancedTextTyper : public GenericTextTyper {
        GDCLASS(AdvancedTextTyper, GenericTextTyper)
        
        protected:
            static void _bind_methods();

        public:
            bool type_buffer_text_complete;
        
        private:
            TypedArray<DialoguePause> pauses;
            int text_size_counter = 0;
            int pauses_done_counter = 0;

            bool type_buffer_complete = false;
            int current_dialogue_index = 0;

            Ref<Dialogues> queued_dialogues;
            Array expressions;
        
        public:
            AdvancedTextTyper();
            ~AdvancedTextTyper();

            virtual void _ready() override;

            void on_tween_finished_advanced();
            void type_text(const PackedStringArray& text) override;
            virtual void type_text_advanced(const Ref<Dialogues>& dialogues);

            void type_buffer(const Ref<Dialogues>& dialogues, int index);
            void process_next_buffer();

            void type_buffer_text(const String& txt);

            void playclick_advanced();

            void on_confirm_advanced();
            
            int get_current_dialogue_index() const;
            void set_current_dialogue_index(int p_index);
            
            Ref<Dialogues> get_queued_dialogues() const;
            void set_queued_dialogues(Ref<Dialogues> value);

            void set_pauses(TypedArray<DialoguePause> value);
    };
}

#endif