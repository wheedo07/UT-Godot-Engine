#pragma once

#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/scene_state.hpp>
#include<godot_cpp/core/object_id.hpp>
#include<godot_cpp/templates/hash_map.hpp>
#include<godot_cpp/variant/node_path.hpp>
#include<godot_cpp/variant/variant.hpp>
#include<vector>
namespace godot {
    class UTGEPool : public Node {
        GDCLASS(UTGEPool, Node);

        protected:
            static void _bind_methods();

        private:
            struct PoolEntry {
                Node *node = nullptr;
                Ref<PackedScene> scene;
                Ref<SceneState> state;
                Node *parent = nullptr;
                int index = -1;
                HashMap<NodePath, Variant> scripts;
            };

            HashMap<ObjectID, PoolEntry> active_entries;
            HashMap<ObjectID, PoolEntry> released_entries;
            HashMap<Ref<PackedScene>, std::vector<ObjectID>> released_by_scene;

            PoolEntry *find_active_entry(Node *node);
            const PoolEntry *find_active_entry(Node *node) const;
            PoolEntry *pop_released_entry(const Ref<PackedScene> &scene);
            Node *create_entry(const Ref<PackedScene> &scene);
            Node *instantiate_scene(const Ref<PackedScene> &scene) const;
            void reset_node(Node *target, const Ref<SceneState> &state) const;
            void detach_scripts(Node *node, HashMap<NodePath, Variant> &scripts) const;
            void detach_scripts_recursive(Node *root, Node *current, HashMap<NodePath, Variant> &scripts) const;
            void restore_scripts(Node *node, const HashMap<NodePath, Variant> &scripts) const;
            void request_ready_recursive(Node *node) const;
            Node *resolve_state_path(Node *target, int node_index, const Ref<SceneState> &state) const;

        public:
            Node *obtain(Ref<PackedScene> scene);
            void release(Node *node);
            bool has(Node *node) const;
            void clear();

            int get_active_count() const;
            int get_released_count() const;
    };
}
