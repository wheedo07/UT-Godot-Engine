#ifndef OVERWORLD_ENEMY_H
#define OVERWORLD_ENEMY_H

#include "engine/Text/character.h"
#include "engine/resources/Dialogues/dialogues.h"
#include "engine/Overworld/textBox/text_box.h"
#include<godot_cpp/classes/character_body2d.hpp>
#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/timer.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/variant/array.hpp>
#include<godot_cpp/variant/vector2i.hpp>
#include<godot_cpp/variant/typed_array.hpp>
namespace godot {
    class EnemyOverworld : public CharacterBody2D {
        GDCLASS(EnemyOverworld, CharacterBody2D)

        protected:
            static void _bind_methods();
        
        public:
            enum AnimationState {
                IDLE,
                MOVING
            };
        
        private:
            String current_animation_name;
            AnimationState current_anim_state;
            TypedArray<Dialogues> dialogues;
            Ref<PackedScene> text_box;
            Character character;

            int counter;
            int frame_alert;

            Vector2 walk_direction = Vector2(0, 0);
            bool canmove;
            int walk_speed, current_index;
            float walk_speed_mod;

            AnimatedSprite2D* sprite;
            AnimatedSprite2D* alert;
            AudioStreamPlayer* encounter;

            void _on_area_interacted();

        public:
            EnemyOverworld();
            ~EnemyOverworld();

            void _ready() override;
            void _physics_process(double delta) override;
            void set_walk_direction(const Vector2i& direction);

            // 사용 함수
            void start_walking(const Vector2i& direction = Vector2i(0, 0));
            void force_direction(const Vector2& dir);
            void show_alert(float duration=0.35f);
            void set_frame(int index);
            void play_anim(String key, float speed=1, bool back=false);

            void set_dialogues(const TypedArray<Dialogues>& p_dialogues);
            TypedArray<Dialogues> get_dialogues() const;

            void set_walk_speed(int p_speed);
            int get_walk_speed() const;

            void set_character(Character p_character);
            Character get_character() const;

            void set_current_index(int index);
            int get_current_index();

            void set_frame_alert(int value);
            int get_frame_alert();

            void set_canmove(bool value);
    };
}

#endif