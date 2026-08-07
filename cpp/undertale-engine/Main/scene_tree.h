#pragma once

#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
namespace godot {
    class UTGESceneTree : public SceneTree {
        GDCLASS(UTGESceneTree, SceneTree);
       
        protected:
            static void _bind_methods();

        public:
            Ref<SceneTreeTimer> create_timer(double p_time_sec, bool p_process_always = true, bool p_process_in_physics = false, bool p_ignore_time_scale = false);
    };
}