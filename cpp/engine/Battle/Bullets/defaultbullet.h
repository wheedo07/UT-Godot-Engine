#ifndef DefaultBullet_H
#define DefaultBullet_H

#include "bullet.h"
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/interval_tweener.hpp>
#include<godot_cpp/classes/collision_shape2d.hpp>
#include<godot_cpp/classes/rectangle_shape2d.hpp>
namespace godot {
    class DefaultBullet : public Bullet {
        GDCLASS(DefaultBullet, Bullet);

        protected:
            static void _bind_methods();

        public:
            enum MovementMode {
                MOVEMENT_VELOCITY,
                MOVEMENT_TWEEN
            };
            float collision_margin;

        private:
            MovementMode fire_mode;
            Ref<Tween> velocity_tween;

            Node* sprite;
            NodePath collision_path;
            CollisionShape2D* collision;
            Tween::TransitionType tween_trans;
            Tween::EaseType tween_ease;
            Ref<Tween> tw;
            Vector2 target_position;
            
            void _start_await_fire(const Callable& fire_call, float delay);
            void _await_fire(const Callable& fire_call, float delay);

            void set_property(const Variant& value);
            
        public:
            DefaultBullet();
            ~DefaultBullet();

            void _ready() override;
            void _physics_process(double delta) override;
            Vector2 get_sprite_size();

            void fire(const Vector2& target, MovementMode movement_type, float speed = 100.0f, DamageMode mode = MODE_NULL);
            void queue_fire(float delay, const Vector2& target, MovementMode movement_type, float speed = 100.0f, DamageMode mode = MODE_NULL);

            void set_collision_path(const NodePath& p_collision_path);
            NodePath get_collision_path() const;

            void set_collision_margin(float p_collision_margin);
            float get_collision_margin() const;

            void set_tweenTrans(Tween::TransitionType value);
            Tween::TransitionType get_tweenTrans() const;

            void set_tweenEase(Tween::EaseType value);
            Tween::EaseType get_tweenEase() const;

            Vector2 get_target_position() const;
            Ref<Tween> get_velocity_tween() const;
    };
};
VARIANT_ENUM_CAST(godot::DefaultBullet::MovementMode);

#endif