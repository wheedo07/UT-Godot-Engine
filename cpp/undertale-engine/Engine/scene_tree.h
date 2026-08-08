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

            Error change_scene_to_file(StringName layer_id, String path);
            Error change_scene_to_packed(StringName layer_id, Ref<PackedScene> scene);
            Error change_scene_to_node(StringName layer_id, Node *node);

            Ref<UTGETimer> create_timer(double p_time_sec, bool p_process_always = true, bool p_process_in_physics = false, bool p_ignore_time_scale = false);
            Ref<UTGETimer> layer_timer(StringName layer_id, double p_time_sec, bool p_process_always = true, bool p_process_in_physics = false, bool p_ignore_time_scale = false);

            void pause_layer_timers(StringName layer_id, bool force = false);
            void resume_layer_timers(StringName layer_id, bool force = false);
    };
}