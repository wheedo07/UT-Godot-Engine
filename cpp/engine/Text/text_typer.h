#ifndef TEXT_TYPER_H
#define TEXT_TYPER_H

#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/font_file.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/callback_tweener.hpp>
namespace godot {
    class GenericTextTyper : public RichTextLabel {
        GDCLASS(GenericTextTyper, RichTextLabel)

        protected:
            static void _bind_methods();
        
        public:
            enum TextQueueMode {
                AWAIT_FINISH,
                OVERRIDE_CURRENT,
                VOID_QUEUED
            };
        
        private:
            NodePath click_path;
            AudioStreamPlayer* click;
            float interval;
            TextQueueMode queued_texts_handling;
            Ref<Tween> pause_tween;
            Ref<Tween> visible_tween;
            Ref<Tween> sound_tween;
            String chache_parsed_text;
            bool typing;
            String entire_text_bbcode;
            String extra_delay;
            String no_sound;
            PackedStringArray queued_texts;
            int current_line_index;
            bool line_typing;
        
        public:
            GenericTextTyper();
            ~GenericTextTyper();

            virtual void _process(double delta) override;
            virtual void _unhandled_input(const Ref<InputEvent>& event) override;
            virtual void _ready() override;
            virtual void type_text(const PackedStringArray& text);
            void set_click_path(const NodePath& p_path);
            NodePath get_click_path() const;

            void set_interval(float p_interval);
            float get_interval() const;

            void set_queued_texts_handling(TextQueueMode p_mode);
            TextQueueMode get_queued_texts_handling() const;

            void set_entire_text_bbcode(const String& p_text);
            String get_entire_text_bbcode() const;

            void kill_tweens(bool complete_text = false);
            void create_tweeners();
            bool _type_one_line(const String& line);
            void play_click();
            void _process_typing();

            void set_typing(bool value);
            bool get_typing();

            void set_chache_parsed_text(String value);
            String get_chache_parsed_text();

            void set_visible_tween(Ref<Tween> value);
            Ref<Tween> get_visible_tween();

            void set_sound_tween(Ref<Tween> value);
            Ref<Tween> get_sound_tween();

            void set_pause_tween(Ref<Tween> value);
            Ref<Tween> get_pause_tween();

            void set_extra_delay(String value);
            String get_extra_delay();

            void set_no_sound(String value);
            String get_no_sound();

            void set_click(AudioStreamPlayer* value);
            AudioStreamPlayer* get_click();
    };
}
VARIANT_ENUM_CAST(godot::GenericTextTyper::TextQueueMode);

#endif