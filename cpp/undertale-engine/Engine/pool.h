#pragma once

#include<godot_cpp/classes/node.hpp>
namespace godot {
    class UTGEPool : public Node {
        GDCLASS(UTGEPool, Node);
         
        protected:
            static void _bind_methods();
    };
}