#pragma once

#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
namespace godot {
    class UTGELayer : public Node {
        GDCLASS(UTGELayer, Node);

        protected:
            static void _bind_methods();

        private:
            StringName layer_id = "main";

            Node *current_scene = nullptr;
            ProcessMode process_mode_cache = PROCESS_MODE_INHERIT;

        public:
            /* API 함수 */

            Error change_scene(Node *node);
            Node* get_current_scene();
            void clear();
            void pause();
            void resume();

            /* setters / getters */

            void set_layer_id(StringName value);
            StringName get_layer_id();
    };
}