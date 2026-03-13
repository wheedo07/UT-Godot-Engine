#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "text_box_writer.h"
#include "text_box_options.h"
#include "engine/Menus/menu_soul.h"
#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/margin_container.hpp>
#include<godot_cpp/variant/variant.hpp>
namespace godot {
    class TextBox : public CanvasLayer {
        GDCLASS(TextBox, CanvasLayer)

        protected:
            static void _bind_methods();
        
        private:
            int soulpos, skip_count;
            Vector2 soul_position;
            Vector2 soul_offset;
            bool selecting;
            bool selected_option;
            double last_skip_time;
            double last_confirm_time;
            int optionamt;
            TypedArray<Dialogues> text_after_option;
            Dictionary default_settings;

            TextBoxWriter *Text;
            AnimatedSprite2D *head;
            MarginContainer *text_container;
            MenuSoul *soul;
            Array Options;

            void _on_option_selected(int option);
            void _on_option_typing_finished(int option_index, const PackedStringArray& options); 
            void _on_all_texts_finished(const PackedStringArray& options);
            void _setup_options_typing(const PackedStringArray& options);
            void _setup_soul_selection(const PackedStringArray& options);
            void _finish_dialogue();
            void abstract(const Ref<Dialogues>& text, const PackedStringArray& options = PackedStringArray(), 
            const TypedArray<Dialogues>& text_after_options = TypedArray<Dialogues>());
            void _set_head_frame(int expr);
            void finish_options();
            void _on_skip();
            void _on_confirm();
            void _setup_options_timer();
            void _reset_state();
            void _set_all_fonts(Ref<Font> font, float font_size);
        
        public:
            TextBox();
            ~TextBox();

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;

            // 사용 함수
            void generic(const Ref<Dialogues>& text, const PackedStringArray& options = PackedStringArray(), 
            const TypedArray<Dialogues>& text_after_options = TypedArray<Dialogues>());
            void character(bool head_hide, String chr, const Ref<Dialogues>& dialogues, const PackedStringArray& options = PackedStringArray(), 
            const TypedArray<Dialogues>& dialogues_after_options = TypedArray<Dialogues>());
            void set_key(bool is);

            void set_soul_offset(const Vector2& offset);
            Vector2 get_soul_offset() const;

            void _on_text_click_played();
            TextBox *_create();
    };
}

#endif