#ifndef BULLET_AREA_H
#define BULLET_AREA_H

#include "bullet.h"
#include<godot_cpp/classes/area2d.hpp>
namespace godot {
    class BulletArea : public Area2D {
        GDCLASS(BulletArea, Area2D);
        
        protected:
            static void _bind_methods();
        
        public:
            int damage_mode;
            Bullet* bullet;
            int damage;
            float iframes;
            float kr;

        private:
            NodePath bullet_path;
            bool player_hit;

            void _on_yellow_bullet_hit();

            void set_bullet_path(NodePath value);
            NodePath get_bullet_path() const;
        
        public:
            BulletArea();
            ~BulletArea();

            void _ready() override;
            void _process(double delta) override;

            // 사용 함수
            bool is_player_hit();
    };
}

#endif