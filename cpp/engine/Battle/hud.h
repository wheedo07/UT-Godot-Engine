#ifndef BATTLE_HUD_H
#define BATTLE_HUD_H

#include<godot_cpp/classes/h_box_container.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/progress_bar.hpp>
#include<godot_cpp/classes/margin_container.hpp>
namespace godot {
    class BattleHUD : public HBoxContainer {
        GDCLASS(BattleHUD, HBoxContainer);
        
        protected:
            static void _bind_methods();
        
        private:
            RichTextLabel* name_label;
            RichTextLabel* lv_label;
            ProgressBar* hp_bar;
            ProgressBar* kr_bar;
            MarginContainer* hp_bar_container;
            RichTextLabel* kr_text;
            RichTextLabel* hp_label;
        
        public:
            BattleHUD();
            ~BattleHUD();

            void _ready() override;
            void _process(double delta) override;

            void set_kr(bool to = true);
    };
}

#endif