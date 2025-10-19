#ifndef PLAYER_OVERWORLD_H
#define PLAYER_OVERWORLD_H

#include<godot_cpp/classes/character_body2d.hpp>
#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/area2d.hpp>
#include<godot_cpp/classes/timer.hpp>
#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/progress_bar.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/shader_material.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
#include "engine/resources/Encounters/encounter.h"
#include "engine/Overworld/Interactions/interaction_trigger.h"
#include "../soul_overwolrd.h"

namespace godot {
    class PlayerOverworld : public CharacterBody2D {
        GDCLASS(PlayerOverworld, CharacterBody2D)

        protected:
            static void _bind_methods();
        
        public:
            enum AnimationState {
                IDLE,
                MOVING
            };
        
        private:
            AnimationState current_anim_state;
            int frame_alert;
            bool wolrd_encounter, is_interact;
            Ref<ShaderMaterial> sprite_material;
            Ref<Tween> hpbar_tween;
            
            float walk_speed;
            float walk_speed_modifier = 1.0f;
            
            TypedArray<Encounter> encounters;
            int step_counter_needed = 200;
            int step_increase = 40;
            int step_count = 0;
            String current_animation_name;
            
            AnimatedSprite2D* sprite;
            Area2D* interacter;
            SoulOverworld* soul;
            AnimatedSprite2D* alert_sprite;
            AudioStreamPlayer* encounter_sound;
            ProgressBar* hp_bar;
            
            Dictionary interact_posx;
            Dictionary interact_posy;
            Vector2 last_dir;
            Vector2 direction;
            bool moving;
            Array interactables;
            
            Ref<PackedScene> player_menu;
            Ref<PackedScene> hit_label;
            bool forced_walking;
            Vector2i forced_direction;
            
            bool waiting_for_encounter_timer, waiting_for_hide_timer;
            
            void _on_encounter_timer_completed();
            void _on_hide_timer_completed();
            void _step();
            void refresh_direction();
            void _enter_random_encounter();
            void _on_hurt(int damage, bool heal=false);
            void set_direction();
        
        public:
            PlayerOverworld();
            ~PlayerOverworld();
            
            void _ready() override;
            void _physics_process(double delta) override;
            void _process(double delta) override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            // 사용함수
            void start_walking(const Vector2i& direction = Vector2i());
            void show_alert(float duration=0.35f);
            void set_frame(int index);
            void play_anim(String key, float speed=1);
            void force_direction(const Vector2& dir);
            void off_interact();
            void on_interact();
            bool is_interacting();
            bool is_overworld_encounter();

            void on_overwolrd_encounter();
            void off_overwolrd_encounter();
            
            void set_walk_speed(float p_walk_speed);
            float get_walk_speed() const;
            
            void set_step_counter_needed(int p_steps);
            int get_step_counter_needed() const;
            
            void set_step_increase(int p_increase);
            int get_step_increase() const;
            
            void set_encounters(const TypedArray<Encounter>& p_encounters);
            TypedArray<Encounter> get_encounters() const;
            
            void set_interact_posx(const Dictionary& p_positions);
            Dictionary get_interact_posx() const;
            
            void set_interact_posy(const Dictionary& p_positions);
            Dictionary get_interact_posy() const;

            void set_frame_alert(int value);
            int get_frame_alert();

            void _set_canmove(bool value);
    };
}

#endif