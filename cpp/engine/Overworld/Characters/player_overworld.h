#ifndef PLAYER_OVERWORLD_H
#define PLAYER_OVERWORLD_H

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "engine/resources/Encounters/encounter.h"
#include "engine/Overworld/Interactions/interaction_trigger.h"

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
            
            float walk_speed = 80.0f;
            float walk_speed_modifier = 1.0f;
            
            TypedArray<Encounter> encounters;
            int step_counter_needed = 200;
            int step_increase = 40;
            int step_count = 0;
            String current_animation_name;
            
            AnimatedSprite2D* sprite = nullptr;
            Area2D* interacter = nullptr;
            AnimatedSprite2D* alert_sprite = nullptr;
            AudioStreamPlayer* encounter_sound = nullptr;
            
            Dictionary interact_posx;
            Dictionary interact_posy;
            Vector2 last_dir;
            Vector2 direction;
            bool moving = false;
            Array interactables;
            
            Ref<PackedScene> player_menu;
            bool forced_walking = false;
            Vector2i forced_direction;
            
            bool waiting_for_encounter_timer = false;
            bool waiting_for_hide_timer = false;
            
            void on_encounter_timer_completed();
            void on_hide_timer_completed();
            void _step();
            void refresh_direction();
        
        public:
            PlayerOverworld();
            ~PlayerOverworld();
            
            void _ready() override;
            void _physics_process(double delta) override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            // 사용함수
            void start_walking(const Vector2i& direction = Vector2i());
            void show_alert(float duration=0.35f);
            void set_frame(int index);
            void play_anim(String key, float speed=1, bool back=false);
            
            void _enter_random_encounter();
            void force_direction(const Vector2& dir);
            void set_direction();
            
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

            void set_canmove(bool value);
    };
}

#endif