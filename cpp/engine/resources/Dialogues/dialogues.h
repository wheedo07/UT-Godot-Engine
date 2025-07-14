#ifndef DIALOGUES_H
#define DIALOGUES_H
#include <godot_cpp/classes/resource.hpp>
#include "dialogue.h"
namespace godot {
    class Dialogues : public Resource {
        GDCLASS(Dialogues, Resource)

        protected:
            static void _bind_methods();
        
        private:
            TypedArray<Dialogue> dialogues;
        
        public:
            enum DialogueType {
                DIALOGUE_TEXT,
                DIALOGUE_EXPRESSIONS,
                DIALOGUE_EXPRESSION_HEAD,
                DIALOGUE_EXPRESSION_BODY,
                DIALOGUE_PAUSES,
                DIALOGUE_SPEED
            };
            Dialogues();
            ~Dialogues();
            
            TypedArray<Dialogue> get_dialogues();
            void set_dialogues(TypedArray<Dialogue> p_dialogues);
            Array get_dialogues_single(DialogueType dialog_type) const;
            Dialogues* from(const PackedStringArray& text);
            Dialogues* set_expressions(const Array& heads, const Array& bodies = Array());
            Dialogues* set_speed(const Array& speeds);
    };
}
VARIANT_ENUM_CAST(godot::Dialogues::DialogueType);

#endif