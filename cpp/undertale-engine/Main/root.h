#pragma once

#include<godot_cpp/classes/node.hpp>
#include "layer.h"
namespace godot {
    class UTGERoot : public Node {
        GDCLASS(UTGERoot, Node);

        protected:
            static void _bind_methods();

        private:
            Node *layer_parent = this;

            HashMap<StringName, UTGELayer*> layers;

        public:
            void _ready() override;

            /* API 함수 */

            UTGELayer *get_layer(StringName layer_id = "main");

            /* setters / getters */

            void set_layer_parent(Node *value);
            Node *get_layer_parent();
    };
}