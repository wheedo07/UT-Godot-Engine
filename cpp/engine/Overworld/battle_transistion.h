#ifndef BATTLE_TRANSITION_H
#define BATTLE_TRANSITION_H

#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/color_rect.hpp>
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/gpu_particles2d.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/engine.hpp>
namespace godot {
    class BattleTransition : public CanvasLayer {
        GDCLASS(BattleTransition, CanvasLayer)
        
        protected:
            static void _bind_methods();
        
        private:
            Vector2 target;
            
            // 비동기 처리를 위한 상태 변수들
            enum TransitionState {
                IDLE,
                BLINK_START,
                BLINK_HIDE_1,
                BLINK_SHOW_1,
                BLINK_HIDE_2,
                BLINK_SHOW_2,
                SOUL_DROP,
                COMPLETED
            };
            
            TransitionState current_state = IDLE;
            bool state_processing = false;
            
            ColorRect* black_rect = nullptr;
            Node2D* soul = nullptr;
            GPUParticles2D* ghost = nullptr;
            AudioStreamPlayer* noise_player = nullptr;
            AudioStreamPlayer* drop_player = nullptr;
            
        public:
            BattleTransition();
            ~BattleTransition();

            void _ready() override;
            void transition();
            
            void set_target(const Vector2& p_target);
            Vector2 get_target() const;

            void advance_transition_state();
            void process_blink_start();
            void process_blink_hide_1();
            void process_blink_show_1();
            void process_blink_hide_2();
            void process_blink_show_2();
            void process_soul_drop();
            void on_tween_finished();
            
            void on_timer_completed();
    };
}

#endif