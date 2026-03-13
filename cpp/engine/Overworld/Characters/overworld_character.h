#ifndef OVERWORLD_CHARACTER_NODE_H
#define OVERWORLD_CHARACTER_NODE_H

#include "overworld_sprite.h"
#include "engine/Overworld/textBox/text_box.h"
#include "engine/resources/Dialogues/dialogue_asset.h"
#include "engine/Overworld/Interactions/interaction_trigger.h"
#include<godot_cpp/classes/character_body2d.hpp>
#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/timer.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/variant/array.hpp>
#include<godot_cpp/variant/vector2i.hpp>
#include<godot_cpp/variant/typed_array.hpp>
namespace godot {
    class OverworldCharacterNode : public CharacterBody2D {
        GDCLASS(OverworldCharacterNode, CharacterBody2D)

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
            Ref<DialogueAsset> dialogues;
            String character;

            int counter;
            int frame_alert;

            Vector2 walk_direction = Vector2(0, 0);
            bool canmove;
            int walk_speed, current_index;
            float walk_speed_mod;

            OverworldSprite *sprite;
            InteractionTrigger *area_interact;

            AnimatedSprite2D *alert;
            AudioStreamPlayer *encounter;

            void _on_area_interacted();

        public:
            OverworldCharacterNode();
            ~OverworldCharacterNode();

            void _ready() override;
            void _physics_process(double delta) override;
            void set_walk_direction(const Vector2i& direction);

            // 사용 함수
            void start_walking(const Vector2i& direction = Vector2i(0, 0));
            void force_direction(const Vector2& dir);
            void show_alert(float duration=0.35f);
            void set_frame(int index);
            void play_anim(String key, float speed=1, bool from_end=false);

            void set_dialogues(Ref<DialogueAsset> p_dialogues);
            Ref<DialogueAsset> get_dialogues() const;

            void set_walk_speed(int p_speed);
            int get_walk_speed() const;

            void set_character(String p_character);
            String get_character() const;

            void set_current_index(int index);
            int get_current_index();

            void set_frame_alert(int value);
            int get_frame_alert();

            void set_sprite(OverworldSprite *p_sprite);
            OverworldSprite *get_sprite() const;

            void set_area_interact(InteractionTrigger *p_area);
            InteractionTrigger *get_area_interact() const;

            void _set_canmove(bool value);
    };
}

#endif