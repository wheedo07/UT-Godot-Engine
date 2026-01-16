#ifndef DEATH_SCREEN_H
#define DEATH_SCREEN_H

#include "death_soul.h"
#include "engine/Camera/camera_fx.h"
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/core/binder_common.hpp>
#include<godot_cpp/core/gdvirtual.gen.inc>
namespace godot {
    class DeathScreen : public Node2D {
        GDCLASS(DeathScreen, Node2D)
        
        protected:
            static void _bind_methods();
        
        private:
            DeathSoul* death_soul;

            void _setup_death_animation();
            void set_property(Variant value);
        
        public:
            DeathScreen();
            ~DeathScreen();

            void _ready() override;

            virtual void ready();
            GDVIRTUAL0(ready);

            // 사용 함수
            void end_death();
            PackedStringArray get_gameOver_text();

            void set_death_soul(DeathSoul* p_death_soul);
            DeathSoul* get_death_soul() const;
            
            CameraFx* get_camera_fx() const;
    };
}

#endif