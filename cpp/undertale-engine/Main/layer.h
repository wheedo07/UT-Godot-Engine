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

        public:
            /* API 함수 */

            Node *change_scene(Ref<PackedScene> scene);
            Node* get_current_scene();
            void clear();

            void pause();
            void resume();

            /* setters / getters */
            void set_layer_id(StringName value);
            StringName get_layer_id();
    };
}