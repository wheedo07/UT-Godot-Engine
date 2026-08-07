#pragma once

#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include "undertale-engine/Main/root.h"
namespace godot {
    class UTGESceneTree : public SceneTree {
        GDCLASS(UTGESceneTree, SceneTree);
       
        protected:
            static void _bind_methods();

        private:
            UTGERoot *get_root();

        public:
            Node *change_scene(StringName layer_id, Ref<PackedScene> scene);
            Ref<SceneTreeTimer> create_timer(double p_time_sec, bool p_process_always = true, bool p_process_in_physics = false, bool p_ignore_time_scale = false);
    };
}