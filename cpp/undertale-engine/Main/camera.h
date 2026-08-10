#pragma once

#include<godot_cpp/classes/remote_transform2d.hpp>
#include<godot_cpp/classes/camera2d.hpp>
namespace godot {
    class UTGECamera : public RemoteTransform2D {
        GDCLASS(UTGECamera, RemoteTransform2D);

        protected:
            static void _bind_methods();

        private:
            Camera2D *camera = nullptr;

        public:
            UTGECamera();
            void _utge_ready();

            /* setters / getters */
            void set_camera(Camera2D *value);
            Camera2D *get_camera();
    };
}