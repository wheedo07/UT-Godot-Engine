#ifndef DEATH_SOUL_H
#define DEATH_SOUL_H

#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/gpu_particles2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
namespace godot {
    class DeathSoul : public AnimatedSprite2D {
        GDCLASS(DeathSoul, AnimatedSprite2D)
            
        protected:
            static void _bind_methods();
        
        public:
            enum SoulType {
                HUMAN,
                MONSTER
            };
        
        private:
            SoulType soul_type;
            Color human_color;
            Color monster_color;
            GPUParticles2D* shards;
            AudioStreamPlayer* snap_sound;
            AudioStreamPlayer* shatter_sound;
            
            void _on_first_timer_timeout();
            void _on_second_timer_timeout();
            
            Ref<SceneTreeTimer> first_timer;
            Ref<SceneTreeTimer> second_timer;
            bool die_in_progress;
            
        public:
            DeathSoul();
            ~DeathSoul();
            
            void _ready() override;
            
            void set_soul_type(SoulType p_soul_type);
            SoulType get_soul_type() const;
            
            // 사용 함수
            void die();
    };
}
VARIANT_ENUM_CAST(godot::DeathSoul::SoulType);

#endif