#ifndef YELLOW_BULLET_H
#define YELLOW_BULLET_H

#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/area2d.hpp>
namespace godot {
    class YellowBullet : public AnimatedSprite2D {
        GDCLASS(YellowBullet, AnimatedSprite2D)
            
        protected:
            static void _bind_methods();
        
        private:
            float speed;
            Vector2 velocity;
        
        public:
            YellowBullet();
            ~YellowBullet();

            void _physics_process(double delta) override;
            void _on_area_2d_area_entered(Area2D* area);

            void set_velocity(const Vector2& p_velocity);
            void set_speed(float value);
            float get_speed();
    };
}

#endif