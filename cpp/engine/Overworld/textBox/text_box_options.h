#ifndef TEXT_BOX_OPTION_WRITER_H
#define TEXT_BOX_OPTION_WRITER_H

#include "engine/Text/advanced_text_typer.h"
#include "engine/resources/Dialogues/dialogues.h"
namespace godot {
    class TextBoxOptionWriter : public AdvancedTextTyper {
        GDCLASS(TextBoxOptionWriter, AdvancedTextTyper)
        
        protected:
        static void _bind_methods();
        
        private:
            void process_dialogue();
        
        public:
            TextBoxOptionWriter();
            ~TextBoxOptionWriter();

            void _ready() override;

            void type_text_advanced(const Ref<Dialogues>& dialogues) override;
    };
}

#endif