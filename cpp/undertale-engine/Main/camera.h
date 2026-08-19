#pragma once

#include<godot_cpp/classes/remote_transform2d.hpp>
#include<godot_cpp/classes/camera2d.hpp>
namespace godot {
    class UTGECamera : public RemoteTransform2D {
        GDCLASS(UTGECamera, RemoteTransform2D);

        protected:
            static void _bind_methods();

        private:
            Vector2 zoom = Vector2(1, 1);
            bool position_smoothing_enabled = false;
            double position_smoothing_speed = 5;
            int limit_left = -1000000000;
            int limit_top = -1000000000;
            int limit_right = 1000000000;
            int limit_bottom = 1000000000;

            Camera2D *camera = nullptr;
            void _set_limits();

        public:
            UTGECamera();
            void _utge_ready();

            /* API 함수 */
            void activate();
            void deactivate();
            bool is_active();

            /* setters / getters */

            void set_camera(Camera2D *value);
            Camera2D *get_camera();

            void set_zoom(Vector2 value);
            Vector2 get_zoom();

            void set_position_smoothing_enabled(bool value);
            bool is_position_smoothing_enabled();

            void set_position_smoothing_speed(double value);
            double get_position_smoothing_speed();

            void set_limit_left(int value);
            int get_limit_left();

            void set_limit_top(int value);
            int get_limit_top();

            void set_limit_right(int value);
            int get_limit_right();

            void set_limit_bottom(int value);
            int get_limit_bottom();
    };
}