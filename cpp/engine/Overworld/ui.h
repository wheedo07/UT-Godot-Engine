#ifndef UI_H
#define UI_H

#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/variant/vector2.hpp>
#include<godot_cpp/variant/dictionary.hpp>
#include<godot_cpp/variant/packed_string_array.hpp>
#include "engine/Menus/menu_soul.h"
#include "textBox/text_box.h"
#include "ui_box.h"
namespace godot {
    class UI : public CanvasLayer {
        GDCLASS(UI, CanvasLayer)
        
        protected:
            static void _bind_methods();
        
        public:
            enum States {
                OPTIONS,
                STATS,
                ITEM,
                ITEM_ACTION,
                CELL,
                ITEM_USE_DISABLE_MOVEMENT
            };

        private:
            Dictionary sizethingys;

            MenuSoul* soul;
            UI_Box* stats;
            UI_Box* items;
            UI_Box* cells;
            Dictionary item_actions;

            Vector2 soulposition;
            Vector2 soulposition_item;
            Dictionary optionsize;
            Ref<PackedScene> textboxscene;
            TextBox* textbox;
            Vector2 soultarget;
            States current_state;

            TypedArray<bool> enabled_options;
            Dictionary options_dict;
            Dictionary pos_history;

            void _on_animation_finished();
            void _on_item_dialogue_finished();

        public:
            UI();
            ~UI();

            void set_is_kill(bool value);

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            void _in_state(States state);
            void _set_enabled_options();
            void _write_options();
            void _set_overview();
            void _set_detailed();
            void _set_items();
            void _set_cells();
            void _close_menu();
            bool soul_move(const Vector2& action);

            void set_items_seperation(const Vector2& p_sep);
            Vector2 get_items_seperation() const;

            void set_option_seperation(const Vector2& p_sep);
            Vector2 get_option_seperation() const;

            void set_options_dict(const Dictionary& p_dict);
            Dictionary get_options_dict() const;

            void set_enabled_options(const Array& options);
            Array get_enabled_options() const;
        
        private:
            Vector2 items_seperation;
            Vector2 option_seperation;
    };
}
VARIANT_ENUM_CAST(godot::UI::States);

#endif