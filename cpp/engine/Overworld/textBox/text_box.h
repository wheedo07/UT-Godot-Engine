#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "text_box_writer.h"
#include "text_box_options.h"
#include "engine/Text/character.h"
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
            Vector2 soul_position = Vector2(0, 0);
            bool selecting = false;
            double last_skip_time = 0.0;
            double last_confirm_time = 0.0;
            int optionamt = 0;
            TypedArray<Dialogues> text_after_option;

            Character talking_character = DEFAULT;

            bool selected_option = false;
            bool text_typing_completed = false;
            bool options_typing_completed = false;
            bool selection_completed = false;
            bool post_selection_typing_completed = false;

            Dictionary char_sound;
            Dictionary char_font;

            TextBoxWriter* Text;
            AnimatedSprite2D* head;
            MarginContainer* text_container;
            MenuSoul* soul;
            Array Options;
            Vector2 defpos = Vector2(0, 0);
            Vector2 defsize = Vector2(0, 0);

            void on_option_selected(int option);
            void on_text_typing_finished();
            void on_option_typing_finished(int option_index, const PackedStringArray& options); 
            void on_all_texts_finished(const PackedStringArray& options);

            void setup_options_typing(const PackedStringArray& options);
            void setup_soul_selection(const PackedStringArray& options);
            void finish_dialogue();
            void on_finish_dialogue();
            void abstract(const Ref<Dialogues>& text, const PackedStringArray& options = PackedStringArray(), 
            const TypedArray<Dialogues>& text_after_options = TypedArray<Dialogues>());
            void set_head_frame(const Array& expr);
            void finish_options();
            void _on_skip();
            void _on_confirm();
            void _setup_options_timer();
        
        public:
            TextBox();
            ~TextBox();

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;

            // 사용 함수
            void generic(const Ref<Dialogues>& text, const PackedStringArray& options = PackedStringArray(), 
            const TypedArray<Dialogues>& text_after_options = TypedArray<Dialogues>());
            void character(bool head_hide, Character chr, const Ref<Dialogues>& dialogues, const PackedStringArray& options = PackedStringArray(), 
            const TypedArray<Dialogues>& dialogues_after_options = TypedArray<Dialogues>());
            void set_key(bool is);

            void on_text_click_played();
    };
}

#endif