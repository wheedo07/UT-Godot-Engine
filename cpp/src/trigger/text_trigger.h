#ifndef TEXT_TRIGGER_H
#define TEXT_TRIGGER_H

#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include "engine/Text/character.h"
namespace godot {
    class TextTrigger : public Node2D {
        GDCLASS(TextTrigger, Node2D);
        
        protected:
            static void _bind_methods();
        
        private:
            PackedStringArray text;
            Character current_character;

            void _on_interact_text();

        public:
            TextTrigger();
            ~TextTrigger();

            void set_text(PackedStringArray value);
            PackedStringArray get_text();

            void set_current_character(Character value);
            Character get_current_character() const;
    };
}

#endif