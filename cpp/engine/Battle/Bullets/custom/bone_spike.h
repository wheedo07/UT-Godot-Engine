#ifndef BONE_SPIKE_H
#define BONE_SPIKE_H

#include "../bullet.h"
#include<godot_cpp/classes/reference_rect.hpp>
#include<godot_cpp/classes/panel.hpp>
#include<godot_cpp/classes/nine_patch_rect.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/rectangle_shape2d.hpp>
#include<godot_cpp/classes/interval_tweener.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
namespace godot {
    class BoneSpike : public Bullet {
        GDCLASS(BoneSpike, Bullet)
        
        protected:
            static void _bind_methods();

        private:
            float collision_margin;

            ReferenceRect* warning;
            CollisionShape2D* collision;
            Panel* mode_hint;
            NinePatchRect* sprite_rect;

            const float spike_time = 0.3f;

            void on_warn_time_timeout(const Vector2& size, float remain_time);

            void set_collision_margin(float p_collision_margin);
            float get_collision_margin() const;

            void spike(float remain_time);

        public:
            BoneSpike();
            ~BoneSpike();

            void ready() override;

            void fire(const Vector2& size, float warn_time = 0.4f, float remain_time = 1.0f, DamageMode mode = MODE_WHITE);
    };
}

#endif