#pragma once

#include<godot_cpp/classes/object.hpp>
#include "undertale-engine/Main/scene_tree.h"
namespace godot {
    class UT : public Object {
        GDCLASS(UT, Object);
       
        protected:
            static void _bind_methods();
        
        public:
            static UTGESceneTree* tree();
    };
}