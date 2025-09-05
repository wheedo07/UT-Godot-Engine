#ifndef NAME_SELECTION_H
#define NAME_SELECTION_H

#include "engine/Menus/option_selectable.h"
#include "engine/Text/text_typer.h"
#include "engine/Camera/camera_remote_controller.h"
#include<godot_cpp/classes/h_split_container.hpp>
#include<godot_cpp/classes/label.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/classes/camera2d.hpp>
#include<godot_cpp/classes/color_rect.hpp>
#include<godot_cpp/variant/string.hpp>
#include<godot_cpp/core/binder_common.hpp>
#include<godot_cpp/core/gdvirtual.gen.inc>
namespace godot {
    class NameSelection : public Control {
        GDCLASS(NameSelection, Control)
        
        protected:
            static void _bind_methods();
        
        private:
            String name_text;
            bool confirmable, is_deny;
            int soul_pos;

            Label* name_label;
            AudioStreamPlayer* no_sound;
            GenericTextTyper* typer;
            Array choices;
            HSplitContainer* confirmation;
            ColorRect* color_rect;
            AudioStreamPlayer* cymbal;
            Node* prompt;
            CameraRemoteController* camera;

        public:
            NameSelection();
            ~NameSelection();

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;
            void check_names(String name);
            GDVIRTUAL1(check_names, String);

            void on_check_name(String name);
            void on_name_allowed();

            void on_name_input_text_changed(const String& new_text);
            void on_backspace_pressed();
            void on_name_input_text_submitted();
            void react_to_name(const String& text, bool deny = false);
            void await_confirm();

            void on_typer_finished();
            void on_choice_made(int id);

            void on_pass_name(bool is);
    };
    
}

#endif