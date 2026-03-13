#ifndef NAME_SELECTION_H
#define NAME_SELECTION_H

#include "engine/Menus/option_selectable.h"
#include "engine/Text/text_typer.h"
#include "engine/Camera/camera_controller.h"
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
            bool confirmable;
            int soul_pos;
            Ref<Tween> tween_namepos;
            Ref<Tween> tween_shake;

            Control *name;
            Label *name_label;
            AudioStreamPlayer *no_sound, *cymbal;
            GenericTextTyper *typer;
            Array choices;
            HSplitContainer *confirmation;
            ColorRect *color_rect;
            RichTextLabel *prompt;
            CameraController *camera;

            void _on_typer_finished(bool is_deny);
            void _on_choice_made(int id);
            void _on_check_name(String player_name);
            void _on_name_input_text_changed(const String& new_text);
            void _on_backspace_pressed();
            void _on_name_input_text_submitted();
            void _on_pass_name(bool is);

        public:
            NameSelection();
            ~NameSelection();

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;
            void check_names(String name);
            GDVIRTUAL1(check_names, String);

            // 사용 함수
            void react_to_name(const String& text, bool deny = false);
            void on_name_allowed();
    };
    
}

#endif