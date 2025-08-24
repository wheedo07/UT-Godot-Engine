#ifndef BULLET_H
#define BULLET_H

#include<godot_cpp/classes/character_body2d.hpp>
#include<godot_cpp/classes/canvas_item.hpp>
#include<godot_cpp/classes/collision_shape2d.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/callback_tweener.hpp>
#include<godot_cpp/core/binder_common.hpp>
#include<godot_cpp/core/gdvirtual.gen.inc>
namespace godot {
    class BulletArea;

    class Bullet : public CharacterBody2D {
        GDCLASS(Bullet, CharacterBody2D);
        
        protected:
            static void _bind_methods();
        
        public:
            enum MovementMode {
                MOVEMENT_VELOCITY,
                MOVEMENT_TWEEN
            };
            enum DamageMode {
                MODE_WHITE,
                MODE_GREEN,
                MODE_BLUE,
                MODE_ORANGE
            };
            bool isEditor;
            MovementMode fire_mode;
            Vector2 target_position;
            Ref<Tween> velocity_tween;
            DamageMode damage_mode;
            bool delete_upon_hit_value;

        private:
            int damage_value;
            int iframe_grant_value;
            int kr_amount_value;

            NodePath sprite_path;
            CanvasItem* sprite;

            Array colors;
            Array overlapping_areas;

            void _on_exit_screen();
        
        public:
            Bullet();
            ~Bullet();

            void _ready() override;
            void _physics_process(double delta) override;

            virtual void ready();
            virtual void on_hit_yellow();
            GDVIRTUAL0(ready);
            GDVIRTUAL0(on_hit_yellow);

            void _on_hit_player();
            void _on_hit_player_shield();

            Bullet* set_mode(DamageMode mode = MODE_BLUE);
            void kill();
            void fade();

            void set_damage(int p_damage);
            int get_damage() const;

            void set_iframe_grant(int p_iframe_grant);
            int get_iframe_grant() const;

            void set_kr_amount(int p_kr_amount);
            int get_kr_amount() const;

            void set_delete_upon_hit(bool p_delete_upon_hit);
            bool get_delete_upon_hit() const;

            void set_sprite_path(const NodePath& p_sprite_path);
            NodePath get_sprite_path() const;

            void set_colors(const Array& p_colors);
            Array get_colors() const;
    };
}
VARIANT_ENUM_CAST(godot::Bullet::MovementMode);
VARIANT_ENUM_CAST(godot::Bullet::DamageMode);

#endif