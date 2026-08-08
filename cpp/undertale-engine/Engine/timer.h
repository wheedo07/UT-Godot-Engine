#pragma once

#include<godot_cpp/classes/ref_counted.hpp>
namespace godot {
    class UTGETimer : public RefCounted {
        GDCLASS(UTGETimer, RefCounted);
        
        protected:
            static void _bind_methods();

        private:
            double time_left = 0.0;
            double speed_scale = 1.0;
            bool paused = false;
            bool process_in_physics = false;
            bool ignore_time_scale = false;
            bool process_always = true;
            bool finished = false;
        
        public:
            /* API 함수 */

            void set_time_left(double value);
            double get_time_left() const;

            void set_paused(bool value);
            bool is_paused() const;

            void set_speed_scale(double value);
            double get_speed_scale() const;

            /* 내부 API */

            void set_process_in_physics(bool value);
            bool is_processing_in_physics() const;
            void set_ignore_time_scale(bool value);
            void set_process_always(bool value);

            bool is_finished() const;
            bool process(double p_delta, double p_tree_time_scale, bool p_tree_paused);
    };
}
