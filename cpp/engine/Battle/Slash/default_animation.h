#ifndef __DEFAULT_ANIMATION_H__
#define __DEFAULT_ANIMATION_H__

#include "slash_animation.h"
#include<godot_cpp/classes/timer.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/label.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/animation_tree.hpp>
#include<godot_cpp/classes/gpu_particles2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/animation_node_state_machine_playback.hpp>

namespace godot {
    class DefaultAnimation : public SlashAnimation {
        GDCLASS(DefaultAnimation, SlashAnimation);
        
        protected:
            static void _bind_methods();
        
        private:
            AnimationTree *anim_tree;
            AnimationNodeStateMachinePlayback *anim_tree_playback;
            GPUParticles2D *particles;
            AudioStreamPlayer *punch_sound, *sparkle_sound;
            Label *press_z_label;
            Timer *punch_timer;
            Ref<Tween> active_tween;

            bool can_punch;
            int z_count;

            void _on_animation_finished(StringName anim_name);
            void _on_punch_timer_timeout();
            
        public:
            DefaultAnimation();
            ~DefaultAnimation();

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;

            void start();
    };
}

#endif