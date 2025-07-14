#ifndef SLASH_H
#define SLASH_H

#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/animation_tree.hpp>
#include<godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include<godot_cpp/classes/gpu_particles2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/timer.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/label.hpp>
namespace godot {
    class Slash : public AnimatedSprite2D {
        GDCLASS(Slash, AnimatedSprite2D);
        
        protected:
            static void _bind_methods();
        
        private:
            AnimationTree* anim_tree;
            AnimationNodeStateMachinePlayback* anim_tree_playback;
            GPUParticles2D* particles;
            AudioStreamPlayer* punch_sound;
            AudioStreamPlayer* sparkle_sound;
            Label* press_z_label;
            Timer* punch_timer;
            
            bool can_punch;
            int z_count;
            bool crit;
            float dmg_mult;
            bool punch_finished;
            bool anim_finished;
            Ref<Tween> active_tween;
            
            void _on_punch_timer_timeout();
            void _on_animation_finished(const StringName& anim_name);
            void process_weapon_animation();
            void cleanup_and_free();
        
        public:
            Slash();
            ~Slash();

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;

            void set_crit(bool p_crit);
            bool get_crit() const;
            void set_dmg_mult(float p_dmg_mult);
            float get_dmg_mult() const;
    };
}

#endif