#ifndef BLITTER_H
#define BLITTER_H

#include "engine/Text/text_typer.h"
#include<godot_cpp/classes/margin_container.hpp>
#include<godot_cpp/classes/viewport.hpp>
namespace godot {
    class Blitter : public GenericTextTyper {
        GDCLASS(Blitter, GenericTextTyper)
        
        protected:
            static void _bind_methods();
        
        private:
            MarginContainer* container;
            PackedStringArray flavour_texts;
            float default_volume;

            int current_text_index;
            TypedArray<String> current_texts;
            bool is_typing;
        
        public:
            Blitter();
            ~Blitter();

            void _ready() override;
            void _process(double delta) override;

            // 메서드
            void blitter(int turn);
            void blittertext(const PackedStringArray& alltext);
            void type_text(const Variant text = "Blank");

            void _on_confirm_pressed();
            void _continue_typing_next_line();

            PackedStringArray get_flavour_texts() const;
            void set_flavour_texts(const PackedStringArray& texts);
    };
}

#endif