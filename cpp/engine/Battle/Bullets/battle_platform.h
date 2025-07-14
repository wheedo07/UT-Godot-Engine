#ifndef BattlePlatform_H
#define BattlePlatform_H
#include "battle_object.h"
#include<godot_cpp/classes/area2d.hpp>
#include<godot_cpp/classes/nine_patch_rect.hpp>
#include<godot_cpp/classes/static_body2d.hpp>
#include<godot_cpp/classes/collision_shape2d.hpp>
#include<godot_cpp/classes/rectangle_shape2d.hpp>
namespace godot {
    class BattlePlatform : public BattleObject {
        GDCLASS(BattlePlatform, BattleObject);
        
        protected:
            static void _bind_methods();
        
        private:
            Vector2 movement;
            NinePatchRect* platform;
            StaticBody2D* platform_col;
            CollisionShape2D* collision;
            CollisionShape2D* collision_area;
            Area2D* area;
            
            void _on_tween_completed();
        
        public:
            BattlePlatform();
            ~BattlePlatform();
            
            void _ready() override;
            void _physics_process(double delta) override;

            void fire(Vector2 target, float length=40.0f, float speed=100.0f, FireMode fire_mode=FireMode::TWEEN);
    };
}

#endif