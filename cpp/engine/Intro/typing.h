#ifndef TYPING_H
#define TYPING_H

#include "engine/Menus/option_selectable.h"
#include<godot_cpp/classes/reference_rect.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/input_event_key.hpp>
#include<godot_cpp/classes/theme.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/variant/vector2.hpp>
#include<godot_cpp/variant/vector2i.hpp>
#include<godot_cpp/variant/string.hpp>
namespace godot {
    class Typing : public ReferenceRect {
        GDCLASS(Typing, ReferenceRect)
        
        protected:
            static void _bind_methods();
        
        private:
            Vector2 margin_letters;
            String LETTERS;
            Array letters;
            
            Vector2i current_pos;
            bool shift_pressed;
            OptionSelectable* shift_button;
        
        public:
            Typing();
            ~Typing();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            void enable_input(int x);
            void disable_input();
            void refresh_thing(const Vector2i& action = Vector2i(0, 0));

            Vector2i get_current_pos() const;

            void set_margin_letters(Vector2 value);
            Vector2 get_margin_letters();

            void set_current_pos(const Vector2i& pos);
            void _create_letters();
            void _on_shift_pressed(bool shift);
    };
}

#endif