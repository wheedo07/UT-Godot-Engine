#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include<godot_cpp/classes/remote_transform2d.hpp>
#include<godot_cpp/classes/color_rect.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/variant/vector2.hpp>
#include<godot_cpp/variant/color.hpp>
namespace godot {
    class CameraFx;

    class CameraController : public RemoteTransform2D {
        GDCLASS(CameraController, RemoteTransform2D)

        protected:
            static void _bind_methods();
        
        private:
            CameraFx *camerafx;
            ColorRect *fade;
            Vector2 zoom;
            bool position_smoothing_enabled;

            int limit_left;
            int limit_top;
            int limit_right;
            int limit_bottom;
        
        public:
            CameraController();
            ~CameraController();

            void _ready() override;
            void _process(double delta) override;

            // 사용함수
            CameraFx *get_global_camera();
            void force_update();

            void set_zoom(const Vector2& p_zoom);
            Vector2 get_zoom() const;

            void _on_timer_timeout();
            void set_position_smoothing_enabled(bool p_enabled);
            bool get_position_smoothing_enabled() const;

            void _set_limits();

            void set_limit_left(int p_limit);
            int get_limit_left() const;

            void set_limit_top(int p_limit);
            int get_limit_top() const;

            void set_limit_right(int p_limit);
            int get_limit_right() const;

            void set_limit_bottom(int p_limit);
            int get_limit_bottom() const;
    };
}

#endif