#ifndef SHOP_H
#define SHOP_H

#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/nine_patch_rect.hpp>
#include<godot_cpp/classes/tween.hpp>
#include "engine/resources/ShopItems/shop_item.h"
#include "engine/resources/Dialogues/dialogues.h"
#include "engine/Camera/camera_fx.h"
#include "engine/Overworld/room_entrance_node.h"
#include "engine/Menus/item_slider.h"
#include "engine/Menus/menu_soul.h"
#include "engine/Text/advanced_text_typer.h"
namespace godot {
    class SHOP : public CanvasLayer {
        GDCLASS(SHOP, CanvasLayer);

        protected:
            static void _bind_methods();

        private:
            enum States {
                SELECTING_ACTIONS,
                SELECTING_DIALOGUE,
                BUYING_ITEMS,
                SELLING_ITEMS,
                VIEWING_DIALOGUE
            };
            States current_state;
            int soul_position;
            Dictionary soul_positions;
            Dictionary info_panel;
            Dictionary keeper_box;
            Ref<Tween> tw;
            Ref<Tween> tw2;
            PackedInt32Array option_numbers;
            Array offerings;
            Array sellferings;
            Array keeper_dialogues;
            Ref<Dialogues> keeper_def_dialogue;
            bool can_be_sold_to;
            Ref<Dialogues> keeper_cannot_sell_dialogues;
            PackedStringArray dialogue_option;

            Array dialogue_expressions;
            int temp_return_state = 0;

            MenuSoul* soul;
            CameraFx* camera;
            AdvancedTextTyper* keeper_dialogue;
            ItemSlider* item_slider_thing;
            RichTextLabel* sell_items;
            RichTextLabel* dialogue_options;
            Array rects;
            NinePatchRect* items_info_box;
            RichTextLabel* items_info;
            RichTextLabel* items;
            Array gold_inv_info;

            void _in_state(int new_state);
            void _set_item_panel();
            void _set_keeper_box();
            void _refresh_item_panel();
            void _refresh_g_info(bool red = false);
            void _set_soul_pos();
            void _exit();
            
            int _get_sell_items_count() const;
            String _get_sell_items(int id, int size = 4) const;
            void _write_sell_items(int id);
            void _write_shop_items();
            void _keeper_dialogue(Ref<Dialogues> dialogues);
            void _on_keeper_dialogue_started_typing(int index);
            void _on_keeper_dialogue_finished_all_texts();
            
        public:
            SHOP();
            ~SHOP();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;
            void _keeper_dialogue_temp(Ref<Dialogues> dialogues, int return_state);

            void init();

            void set_offerings(const Array& p_offerings);
            
            void set_sellferings(const Array& p_sellferings);
            
            void set_keeper_dialogues(const Array& p_keeper_dialogues);
            
            void set_keeper_def_dialogue(const Ref<Dialogues>& p_keeper_def_dialogue);
            
            void set_can_be_sold_to(bool p_can_be_sold_to);
            
            void set_keeper_cannot_sell_dialogues(const Ref<Dialogues>& p_keeper_cannot_sell_dialogues);

            void set_dialogue_options(const PackedStringArray& p_dialogue_options);
    };
}

#endif