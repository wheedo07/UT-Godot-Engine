#ifndef CYAN_DETECTION_H
#define CYAN_DETECTION_H

#include<godot_cpp/classes/area2d.hpp>
#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/gpu_particles2d.hpp>
namespace godot {
    class CyanDetection : public Area2D {
        GDCLASS(CyanDetection, Area2D)

        protected:
            static void _bind_methods();
        
        private:
            bool can_move, isEditor;
            Sprite2D* sprite;
            GPUParticles2D* glow;

            const float SPEED = 0.4f;
            const float AMT = 0.2f;

            void set_property(bool value);

        public:
            CyanDetection();
            ~CyanDetection();

            void _ready() override;
            void _process(double delta) override;

            bool get_can_move() const;
    };
}

#endif