#pragma once

#include<godot_cpp/classes/node.hpp>
#include "layer.h"
namespace godot {
    class UTGERoot : public Node {
        GDCLASS(UTGERoot, Node);

        protected:
            static void _bind_methods();

        private:
            HashMap<StringName, UTGELayer*> layers;
    };
}