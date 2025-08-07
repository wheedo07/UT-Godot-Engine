#ifndef INTRO_H
#define INTRO_H

#include "engine/Camera/camera_fx.h"
#include "engine/Text/text_typer.h"
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/texture_rect.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/variant/dictionary.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/variant/array.hpp>
namespace godot {
    class Intro : public Control {
        GDCLASS(Intro, Control)
        
        protected:
            static void _bind_methods();
        
        private:
            CameraFx* camera;
            GenericTextTyper* intro_text;
            TextureRect* intro_image;
            Ref<AudioStream> music;

            Array intro_data;
            String intro_json_path;
            String intro_completed_path;
            bool skip_intro;

            int current_index;
            bool intro_completed;

            void _on_text_completed();
            void _on_duration_timeout();
            void _intro_image_next();
            void _load_intro_data_from_json();

            void set_music(const Ref<AudioStream>& p_music);
            Ref<AudioStream> get_music() const;
        
        public:
            Intro();
            ~Intro();
            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;
            void _play_intro();
            void start_text_typing(const String& text, float sleep);
            bool is_intro_completed() const;
            void process_next_intro();
            void _on_intro_completed();

            void set_intro_json_path(const String& path);
            String get_intro_json_path() const;

            void set_intro_completed_path(const String& path);
            String get_intro_completed_path();
    };
}

#endif