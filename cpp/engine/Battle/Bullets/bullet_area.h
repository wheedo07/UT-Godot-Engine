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
            bool isEditor;
            NodePath bullet_path;

            void set_bullet_path(NodePath value);
            NodePath get_bullet_path() const;
        
        public:
            BulletArea();
            ~BulletArea();

            void _ready() override;
            void _process(double delta) override;
    };
}

#endif