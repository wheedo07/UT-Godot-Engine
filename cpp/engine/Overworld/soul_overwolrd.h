#ifndef SOUL_OVERWORLD_H
#define SOUL_OVERWORLD_H

#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/area2d.hpp>
namespace godot {
    class BulletArea;

    class SoulOverworld : public Sprite2D {
        GDCLASS(SoulOverworld, Sprite2D);

        protected:
            static void _bind_methods();

        public:
            float iframes;

        private:
            bool isOn;
            float hiframes;
            Area2D *area;
            Vector2 inputs;

            void check_bullet(Area2D *area_node);
            void hurt(BulletArea *bullet_area);
            void heal(BulletArea *bullet_area);

        public:
            SoulOverworld();
            ~SoulOverworld();

            void _ready() override;
            void _process(double delta) override;
            void _physics_process(double delta) override;
           
            void set_input(Vector2 value);
            void start();
            void stop();
    };
};

#endif