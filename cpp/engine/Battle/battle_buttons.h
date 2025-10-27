#ifndef BATTLE_BUTTONS_H
#define BATTLE_BUTTONS_H

#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include "engine/resources/Encounters/button_set.h"
namespace godot {
    class BattleButtons : public Node2D {
        GDCLASS(BattleButtons, Node2D);
        
        protected:
            static void _bind_methods();
        
        private:
            bool enabled;
            int choice;

            AudioStreamPlayer* move_sound;
            AudioStreamPlayer* select_sound;
            Ref<Tween> tween;
            Ref<ButtonSet> current_button_set;
            Array buttons;
            Array button_enabled;
        
            void changepos(int action);

        public:
            BattleButtons();
            ~BattleButtons();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            void set_button(Ref<ButtonSet> button_set);
            void _enable();
            void disable();
            void _reset();

            // 사용함수
            void glow_choice(int id);
            void play(int id, String anim, float custom_speed = 1, bool from_end=false);
            void hide_button(int id);
            void show_button(int id);

            void set_button_enabled(Array enabled);
            Array get_button_enabled() const;
    };
}

#endif