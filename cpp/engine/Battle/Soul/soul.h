#ifndef SOUL_BATTLE_H
#define SOUL_BATTLE_H

#include "engine/Battle/Soul/green_soul.h"
#include "engine/Battle/Soul/cyan_detection.h"
#include<godot_cpp/classes/character_body2d.hpp>
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/gpu_particles2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/area2d.hpp>
#include<godot_cpp/classes/collision_shape2d.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/input.hpp>
namespace godot {
    class BattleMain;
    class BulletArea;

    class SoulBattle : public CharacterBody2D {
        GDCLASS(SoulBattle, CharacterBody2D)
        
        protected:
            static void _bind_methods();
        
        public:
            enum SoulMode {
                RED,
                BLUE,
                ORANGE,
                YELLOW,
                GREEN,
                CYAN,
                PURPLE,
                DISABLE_MOVEMENT
            };

            enum SoulType {
                SOUL_HUMAN,
                SOUL_MONSTER
            };

            enum SpecialBulletMode {
                ARROW_KEYS,
                VELOCITY,
                ARROW_KEYS_AND_MOVING
            };

        private:
            Input* input;
            Array jump;
            float speed;
            float gravity;
            float gravity_multiplier;

            int soul_type;
            SoulMode mode;
            Vector2 inputs;
            int slow_down;

            Vector2 gravity_direction;
            int special_bullet_mode;

            Node2D* sprite;
            GPUParticles2D* ghost;
            GPUParticles2D* afterimage;
            AudioStreamPlayer* shoot;
            AudioStreamPlayer* mode_change_sound;
            BattleMain* main;
            Area2D* area;
            CollisionShape2D* collision;
            AudioStreamPlayer* wallhit;

            // 소울 모드 관련
            Ref<PackedScene> green_mode_scene;
            GreenShielding* green_shield;

            Ref<PackedScene> yellow_bullet_scene;

            Ref<PackedScene> cyan_detect_scene;
            CyanDetection* cyan_detector;

            Dictionary mode_nodes;

            // 무적 프레임 및 충돌 관련
            float hiframes;
            float iframes;
            bool invulnerable;
            TypedArray<Area2D> overlapping_areas;

            // 모션 관련
            Vector2 motion;
            float changed_direction_time;

            // 보라색 소울 관련
            int purple_pos;
            Ref<Tween> p_tween;

            // 메뉴 관련
            Ref<Tween> move_tween;
            Ref<Tween> _able_tween;
            Ref<Tween> fade_tw;

            const float TIME_GRANT = 0.08f;
            const float FADE_TIME = 0.2f;
            const float TIME = 0.2f;

            void set_mode_silent(int new_mode);
            void set_gravity_direction_silent(const Vector2& new_direction);
            void red();
            void blue();
            void green();
            void yellow();
            void purple();
            void orange();
            void cyan();
            void four_dir_movement();
            void _motion_align_gravity_direction();
            void update_purple_pos();
            void _kill_able_tween();
            void enable_tween();
            void check_bullet(Area2D* area);
            void hurt(BulletArea* area);
            void heal(BulletArea* area);
            void fade_tw_calle(Node* node_ref, Node* parent_ref);
            void _on_death();
        
        public:
            SoulBattle();
            ~SoulBattle();

            void _ready() override;
            void _process(double delta) override;
            void _physics_process(double delta) override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            // 사용 함수
            SoulMode get_mode() const;
            void set_mode(SoulMode new_mode = RED);
            void set_gravity_multiplier(float value);
            void set_gravity_direction(const Vector2& new_direction, bool force_blue_mode = true);

            void disable();
            void enable();
            void menu_enable();
            void _on_move_soul(const Vector2& newpos);

            void set_soul_type(int p_soul_type);
            int get_soul_type() const;

            void set_special_bullet_mode(int p_mode);
            int get_special_bullet_mode() const;
    };
}
VARIANT_ENUM_CAST(godot::SoulBattle::SoulMode);
VARIANT_ENUM_CAST(godot::SoulBattle::SoulType);
VARIANT_ENUM_CAST(godot::SoulBattle::SpecialBulletMode);

#endif