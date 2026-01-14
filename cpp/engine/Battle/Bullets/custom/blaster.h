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
        GDCLASS(Blaster, Bullet);

        protected:
            static void _bind_methods();

        private:
            Vector2 target_position;
            Control* beam;
            NinePatchRect* rect;
            AnimationPlayer* anim_player;
            CollisionShape2D* collision;
            Tween::TransitionType tween_trans;
            Tween::EaseType tween_ease;
            Ref<Tween> velocity_tween;

            void set_property(const Variant& value);
            void _blast(float duration, float up_delay);
            void _on_blast_finished();

        public:
            Blaster();
            ~Blaster();

            void _ready() override;
            Blaster* fire(const Vector2& target, float size = 1.0f, float time = 0.7f, float delay = 0.5f, float duration = 0.5f, float up_delay = 0.1f);

            void set_tweenTrans(Tween::TransitionType value);
            Tween::TransitionType get_tweenTrans() const;

            void set_tweenEase(Tween::EaseType value);
            Tween::EaseType get_tweenEase() const;

            Vector2 get_target_position() const;
    };
}

#endif