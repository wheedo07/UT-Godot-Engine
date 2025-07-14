#ifndef ATTACK_BAR_H
#define ATTACK_BAR_H

#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/color_rect.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/animation_player.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
namespace godot {
    class AttackBar : public AnimatedSprite2D {
        GDCLASS(AttackBar, AnimatedSprite2D);
        
        protected:
            static void _bind_methods();

        private:
            const float MOVE_SPEED = 210.0f;
            const float Time = 0.25f;
            const Tween::TransitionType TRANSTYPE = Tween::TRANS_CUBIC;
            float speed_mult;
            const Tween::TransitionType movetype = Tween::TRANS_LINEAR;
            bool single_bar;

            int direction;
            Vector2 critzone;
            Ref<Tween> tw;

            bool hityet;
            bool can_crit;

            ColorRect* overlay;
            AnimationPlayer* animation_player;
            AudioStreamPlayer* hit_sound;
            AudioStreamPlayer* critical_sound;

            void _on_tween_finished();
            void _on_tween_finished2();
        
        public:
            AttackBar();
            ~AttackBar();
            
            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;
            
            void set_direction(int p_direction);
            int get_direction() const;
            
            void set_speed_mult(float p_speed_mult);
            float get_speed_mult() const;
    };
}

#endif