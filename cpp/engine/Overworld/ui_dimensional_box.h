#ifndef UI_DIMENSIONAL_BOX_H
#define UI_DIMENSIONAL_BOX_H

#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include "engine/Menus/menu_soul.h"
#include "ui_box.h"
namespace godot {
    class UIdimensionalBox : public CanvasLayer {
        GDCLASS(UIdimensionalBox, CanvasLayer);
        
        protected:
            static void _bind_methods();

        private:
            enum BoxState {
                INVENTORY_SIDE,
                BOX_SIDE
            };
            BoxState current_side;
            Vector2 soulposition;
            Vector2 seperation;

            UI_Box *main;
            MenuSoul *soul;
            RichTextLabel *inventory_items, *box_items;

            bool soul_move(Vector2 action);
            void _set_items();
            void _close_box();

        public:
            UIdimensionalBox();
            ~UIdimensionalBox();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            void set_seperation(Vector2 value);
            Vector2 get_seperation() const;
    };
}

#endif