#ifndef SAVE_LOADER_H
#define SAVE_LOADER_H

#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/label.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/tween.hpp>
#include "engine/Menus/option_selectable_solo.h"
namespace godot {
    class SaveLoader : public CanvasLayer {
        GDCLASS(SaveLoader, CanvasLayer)
        
        protected:
            static void _bind_methods();
        
        private:
            Vector2 defsize;
            int reset_level;
            
            Control* control_node;
            RichTextLabel* location_text;
            RichTextLabel* name_text;
            RichTextLabel* lv_text;
            RichTextLabel* time_text;
            OptionSelectableSolo* reset_text;
            AudioStreamPlayer* warn_sound;
        
        public:
            SaveLoader();
            ~SaveLoader();

            void _ready() override;

            void load_save();
            void warn();
            void reset();
            void _hide();
            void _show();
            void refresh();
            void _on_reset_accept_pressed();
    };
}

#endif