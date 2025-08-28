#ifndef BATTLE_BOX_H
#define BATTLE_BOX_H
#define SCROLL_THRESHOLD 3

#include "blitter.h"
#include "argsHolder.h"
#include "engine/Text/text_typer.h"
#include "engine/Menus/item_slider.h"
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/nine_patch_rect.hpp>
#include<godot_cpp/classes/margin_container.hpp>
#include<godot_cpp/classes/remote_transform2d.hpp>
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/collision_shape2d.hpp>
#include<godot_cpp/classes/progress_bar.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/line2d.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
namespace godot {
    class BattleMain;
    class Enemy;
    class BattleBoxBehaviour;

    class BattleBox : public Node2D {
        GDCLASS(BattleBox, Node2D);
            
        protected:
            static void _bind_methods();
        
        public:
            enum RelativePosition {
                RELATIVE_TOP_LEFT,
                RELATIVE_TOP_RIGHT,
                RELATIVE_BOTTOM_LEFT,
                RELATIVE_BOTTOM_RIGHT,
                RELATIVE_CENTER
            };

            enum ButtonOption {
                OPTION_FIGHT = 0,
                OPTION_ACT = 1,
                OPTION_ITEM = 2,
                OPTION_MERCY = 3
            };

            enum BattleState {
                State_Disabled,
                State_BlitteringCasual,
                State_Blittering,
                State_TargetEnemy,
                State_Acting,
                State_Iteming,
                State_Mercying,
                State_Fighting
            };
            int button_choice;
            Dictionary screens;
            Array history;
            Array enemies;
            Array enemies_hp;
            Array action_memory;
            Vector2i soul_position;
            int current_target_id;
            int current_page;
            int scroll_counter;
            int items_per_page;
            Array choices_extends;
            Array webs_array;
            Control* rect_no_clip;
            Control* rect_clip;

        private:
            const int colsize = 500;
            
            Tween::TransitionType TransType;
            Tween::EaseType EaseType;
            
            PackedStringArray mercy_texts;
            String win_text;
            Vector2 options_pos_base;
            Vector2 options_pos_step;
            
            int mercy_choice;
            
            Ref<Tween> tw;
            Array corner_positions;
            Array anchor_targets;
            Array def_anchors;
            Vector2 current_size;
            bool can_move, isEditor;
            int used_item;
            int current_web;
            
            ItemSlider* slider;
            RichTextLabel* items_label; 
            RichTextLabel* mercy_choices;
            AudioStreamPlayer* choice_sound;
            MarginContainer* blitter;
            MarginContainer* hp_bar_container;
            Blitter* blitter_text;
            BattleMain* main;
            MarginContainer* rect_container;
            NinePatchRect* rect;
            Ref<PackedScene> web_scene;
            Control* webs;
            Array collisions;
            Array hp_bars;
            RemoteTransform2D* tl;
            RemoteTransform2D* br;
            Dictionary current_state_nodes;
            BattleBoxBehaviour* current_state_node;
            RichTextLabel* column1;
            RichTextLabel* column2;
            Ref<Texture2D> box_texture;
            
            Array id_to_soul_pos(int id, int x_limit = 2);
            void refresh_options();
            void refresh_nodes();
            void _on_use_button(int choice);
            
            void tween_size(Ref<ArgsHolder> args);
            void real_rotate_by(Ref<ArgsHolder> args);
            void _on_soul_move_cooldown();
        
        public:

            BattleBox();
            ~BattleBox();
            
            void _ready() override;
            void _physics_process(double delta) override;
            void _unhandled_input(const Ref<InputEvent>& event) override;
            
            void set_mercy_options();
            int soul_pos_to_id(const Vector2& soul_pos, bool is = false, int x_limit = 2);
            void set_options();
            void set_items();
            void soul_choice(const Vector2i& action);
            void set_enemies(const Array p_enemies);
            void set_targets(bool show_hp_bar = false);
            void backout();
            void change_state(int new_state);
            void disable();
            void blitter_flavour();
            void blitter_act();
            void blitter_item();
            void blitter_mercy();
            RemoteTransform2D* get_tl() const;
            RemoteTransform2D* get_br() const;
            Blitter* get_blitter_text() const;

            void set_wintext(String value);
            String get_wintext() const;

            int get_used_item();
            void set_used_item(int value);
            
            // 사용 함수
            void blitter_print(PackedStringArray texts);
            void reset_box(float duration = 0.6f);
            void change_size(const Vector2& new_size, bool relative = false, float duration=0.6f);
            void change_position(const Vector2& new_position, bool relative = false, float duration=0.6f);
            void advanced_change_size(RelativePosition relative_to, 
                const Vector2& new_position = Vector2(), 
                const Vector2& new_size = Vector2(100, 100), 
                bool position_relative = false, 
                bool size_relative = false, float duration = 0.6f);
            void rotate_by(float rot, bool relative = false, float duration = 0.6f);
            void box_show();
            void box_hide();
            Vector2 get_size();
            Vector2 get_tl_anchor();
            Vector2 get_br_anchor();
            void clear_webs();
            void set_webs(int n, float separation = -1, int margin = 0);
            float get_web_y_pos(int id);
    };
}
VARIANT_ENUM_CAST(godot::BattleBox::RelativePosition);
VARIANT_ENUM_CAST(godot::BattleBox::ButtonOption);
VARIANT_ENUM_CAST(godot::BattleBox::BattleState);

#endif