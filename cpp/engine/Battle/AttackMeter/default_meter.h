#ifndef __DEFAULT_METER_H__
#define __DEFAULT_METER_H__

#include "attack_meter.h"
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
namespace godot {
    class DefaultMeter : public AttackMeter {
        GDCLASS(DefaultMeter, AttackMeter);
        
        protected:
            static void _bind_methods();

        private:
            const Tween::TransitionType transtype = Tween::TRANS_QUAD;

            Sprite2D *meter;
            Vector2 meter_scale;
            Ref<PackedScene> bar_scene;

            bool can_crit;
            int total_bars, delay_offset;
            float speed_mult;

            float distance;
            int score;
            int crits;

            int waiting_calculations;
            int hits, misses;
            void _on_timeout(Vector2 position, int direction);
            void _on_bar_about_to_fade_out();
            void _on_calculated();
        
        public:
            DefaultMeter();
            ~DefaultMeter();

            void _ready() override;

            void set_meter_texture(Ref<Texture> texture);
            void set_meter_scale(Vector2 scale);

            void remove_meter();
            void summonbar(Vector2 position, int direction, float delay);
            void miss();
            void calculate(int posx, bool crit, float hspeed);
            int finalcalculation();
    };
}

#endif