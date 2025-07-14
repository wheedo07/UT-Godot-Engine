#ifndef GREEN_SOUL_H
#define GREEN_SOUL_H

#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/line2d.hpp>
#include<godot_cpp/classes/area2d.hpp>
#include<godot_cpp/classes/timer.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
namespace godot {
    class GreenShielding : public Node2D {
        GDCLASS(GreenShielding, Node2D)
        
        protected:
            static void _bind_methods();
        
        private:
            const Color DEF_COL = Color("0028ff");
            const Color HIT_COL = Color("ff0000");
            const int RAD = 28;
            const Color COLOR = Color(0.0f, 0.5f, 0.0f); // DARK_GREEN
            const float TIME = 0.2f;

            bool always_remove_shielded_bullets, isEditor;

            Timer* hit_timer;
            Line2D* line;
            Area2D* shield;
            Ref<Tween> shield_tween;
            AudioStreamPlayer* ding_sound;
        
        public:
            GreenShielding();
            ~GreenShielding();

            void _ready() override;
            void _draw() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            void _on_timer_timeout();
            void _change_shield_rot_deg(int to);
            void _on_shield_area_entered(Area2D* area);
    };
}

#endif