#ifndef BATTLE_BUTTONS_H
#define BATTLE_BUTTONS_H

#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/input_event.hpp>
namespace godot {
    class BattleButtons : public Node2D {
        GDCLASS(BattleButtons, Node2D);
        
        protected:
            static void _bind_methods();
        
        private:
            bool enabled, isEditor;
            int choice;

            AudioStreamPlayer* move_sound;
            AudioStreamPlayer* select_sound;
            Array buttons;
        
        public:
            BattleButtons();
            ~BattleButtons();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            void changepos(int action);
            void glow_choice(int id);
            void enable();
            void disable();
            void reset();

            void set_enabled(bool p_enabled);
            bool get_enabled() const;
            void set_choice(int p_choice);
            int get_choice() const;
    };
}

#endif