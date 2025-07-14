#ifndef ATTACK_METER_H
#define ATTACK_METER_H

#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
namespace godot {
    class AttackMeter : public CanvasLayer {
        GDCLASS(AttackMeter, CanvasLayer);
        
        protected:
            static void _bind_methods();
        
        private:
            const float TIME = 0.5f;
            const Tween::TransitionType transtype = Tween::TRANS_QUAD;

            int targetdef;
            Sprite2D* meter;
            Ref<PackedScene> bar_scene;
            int total_bars;
            float speed_mult;
            int targetid;

            int target;
            float distance;
            int score;
            int misses;
            int crits;
            int hits;

            bool can_crit;

            void _on_calculated();
            void _on_bar_about_to_fade_out();
            void _on_timeout(const Vector2& position, int direction);

            // 비동기 작업을 위한 상태 추적
            int waiting_calculations;
            bool initial_setup_complete;
        
        public:
            AttackMeter();
            ~AttackMeter();

            void _ready() override;

            void remove_meter();
            void summonbar(const Vector2& position, int direction, float delay);
            void miss();
            void calculate(int posx, bool crit, float hspeed);
            int finalcalculation();

            void set_targetdef(int p_targetdef);
            int get_targetdef() const;

            void set_target(int p_target);
            int get_target() const;

            void set_targetid(int p_targetid);
            int get_targetid() const;
    };
}

#endif