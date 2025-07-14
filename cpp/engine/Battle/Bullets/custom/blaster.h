#ifndef BLASTER_H
#define BLASTER_H

#include "../bullet.h"
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/nine_patch_rect.hpp>
#include<godot_cpp/classes/animation_player.hpp>
#include<godot_cpp/classes/rectangle_shape2d.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/interval_tweener.hpp>
namespace godot {
    class Blaster : public Bullet {
        GDCLASS(Blaster, Bullet)

        protected:
            static void _bind_methods();

        private:
            const float TIME = 0.8f;
            const float BEAM_COLLISION_MARGIN = 6.0f;
            const float GROW_TIME = 0.2f;
            const int SPEED = 1000;

            Control* beam;
            NinePatchRect* rect;
            AnimationPlayer* anim_player;
            CollisionShape2D* collision;
            Tween::TransitionType tween_trans;
            Tween::EaseType tween_ease;

            void set_tweenTrans(Tween::TransitionType value);
            Tween::TransitionType get_tweenTrans() const;

            void set_tweenEase(Tween::EaseType value);
            Tween::EaseType get_tweenEase() const;

        public:
            Blaster();
            ~Blaster();

            void ready() override;

            Blaster* fire(const Vector2& target, float size = 1.0f, float delay = 0.5f, float duration = 0.5f);
            void _blast(float duration);
    };
}

#endif