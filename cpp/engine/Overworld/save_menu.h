#ifndef SAVE_MENU_H
#define SAVE_MENU_H

#include "engine/Menus/menu_soul.h"
#include "engine/Menus/option_selectable_solo.h"
#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/label.hpp>
#include<godot_cpp/classes/time.hpp>
#include<godot_cpp/classes/sprite2d.hpp>
namespace godot {
    class SaveMenu : public CanvasLayer {
        GDCLASS(SaveMenu, CanvasLayer)
        
        protected:
            static void _bind_methods();

        private:
            Vector2 defsize;
            bool saved;
            Node* sc;

            Ref<Tween> tw;

            Control* control_node;
            RichTextLabel* location_label;
            RichTextLabel* name_label;
            RichTextLabel* lv_label;
            RichTextLabel* time_label;
            OptionSelectableSolo* return_option;
            OptionSelectableSolo* save_option;
            MenuSoul* soul_node;
        
        public:
            SaveMenu();
            ~SaveMenu();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            void dismiss();
            void save();
            void refresh();

            void set_saved(bool value);
    };
}

#endif