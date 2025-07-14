#ifndef BULLET_AREA_YH_H
#define BULLET_AREA_YH_H

#include "bullet_area.h"
namespace godot {
    class BulletAreaYellowHittable : public BulletArea {
        GDCLASS(BulletAreaYellowHittable, BulletArea)
        
        protected:
            static void _bind_methods();
        
        public:
            void _on_yellow_bullet_hit();
    };
}

#endif