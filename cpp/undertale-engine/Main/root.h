#pragma once

#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/camera2d.hpp>
#include "layer.h"
namespace godot {
    class UTGERoot : public Node {
        GDCLASS(UTGERoot, Node);

        protected:
            static void _bind_methods();

        private:
            Node *layer_parent = this;
            Camera2D *camera = nullptr;

            HashMap<StringName, UTGELayer*> layers;

        public:
            UTGERoot();
            void _utge_ready();

            /* API 함수 */

            UTGELayer *get_layer(StringName layer_id = "main");

            /* setters / getters */

            void set_layer_parent(Node *value);
            Node *get_layer_parent();

            void set_camera(Camera2D *value);
            Camera2D *get_camera();
    };
}