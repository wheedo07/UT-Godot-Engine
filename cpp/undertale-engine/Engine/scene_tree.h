#pragma once

#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/templates/vector.hpp>
#include "undertale-engine/Main/root.h"
#include "timer.h"
namespace godot {
    class UTGESceneTree : public SceneTree {
        GDCLASS(UTGESceneTree, SceneTree);
       
        protected:
            static void _bind_methods();

        private:
            Vector<Ref<UTGETimer>> timers;
            UTGERoot *get_root();
            void _process_timers(double p_delta, bool p_process_in_physics);

        public:
            bool _physics_process(double p_delta) override;
            bool _process(double p_delta) override;

            /* API 함수 */

            Node *change_scene(StringName layer_id, Ref<PackedScene> scene);
            Ref<UTGETimer> create_timer(double p_time_sec, bool p_process_always = true, bool p_process_in_physics = false, bool p_ignore_time_scale = false);
    };
}