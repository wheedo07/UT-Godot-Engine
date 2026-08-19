#include "pool.h"
using namespace godot;

void UTGEPool::_bind_methods() {
    /* API 메서드 */
    ClassDB::bind_method(D_METHOD("release", "node"), &UTGEPool::release);
    ClassDB::bind_method(D_METHOD("obtain", "scene"), &UTGEPool::obtain);
    ClassDB::bind_method(D_METHOD("has", "node"), &UTGEPool::has);
    ClassDB::bind_method(D_METHOD("clear"), &UTGEPool::clear);
    ClassDB::bind_method(D_METHOD("get_active_count"), &UTGEPool::get_active_count);
    ClassDB::bind_method(D_METHOD("get_released_count"), &UTGEPool::get_released_count);
}

Node *UTGEPool::obtain(Ref<PackedScene> scene) {
    ERR_FAIL_COND_V(scene.is_null(), nullptr);

    PoolEntry *entry = pop_released_entry(scene);
    if(!entry) return create_entry(scene);

    PoolEntry active_entry = *entry;
    Node *node = active_entry.node;
    ERR_FAIL_NULL_V(node, nullptr);

    reset_node(node, active_entry.state);
    restore_scripts(node, active_entry.scripts);

    Node *parent = active_entry.parent;
    if(parent) {
        parent->add_child(node);
        int target_index = active_entry.index;
        int last_index = parent->get_child_count() - 1;
        if(target_index < 0) {
            target_index = last_index;
        }
        if(target_index > last_index) {
            target_index = last_index;
        }
        parent->move_child(node, target_index);
    }

    request_ready_recursive(node);

    released_entries.erase(ObjectID(node->get_instance_id()));
    active_entries.insert(ObjectID(node->get_instance_id()), active_entry);
    return node;
}

void UTGEPool::release(Node *node) {
    ERR_FAIL_NULL(node);

    PoolEntry *entry = find_active_entry(node);
    ERR_FAIL_NULL(entry);

    PoolEntry released_entry = *entry;
    ObjectID id = ObjectID(node->get_instance_id());
    Node *parent = node->get_parent();
    if(parent) {
        released_entry.parent = parent;
        released_entry.index = node->get_index();
        parent->remove_child(node);
    }
    released_entry.scripts.clear();
    detach_scripts(node, released_entry.scripts);

    active_entries.erase(id);
    released_entries.insert(id, released_entry);
    released_by_scene[released_entry.scene].push_back(id);
}

bool UTGEPool::has(Node *node) const {
    if(!node) return false;
    ObjectID id = ObjectID(node->get_instance_id());
    return active_entries.has(id) || released_entries.has(id);
}

void UTGEPool::clear() {
    for(HashMap<ObjectID, PoolEntry>::Iterator it = released_entries.begin(); it != released_entries.end(); ++it) {
        if(it->value.node) {
            it->value.node->queue_free();
        }
    }
    for(HashMap<ObjectID, PoolEntry>::Iterator it = active_entries.begin(); it != active_entries.end(); ++it) {
        if(it->value.node) {
            it->value.node->queue_free();
        }
    }
    released_entries.clear();
    released_by_scene.clear();
    active_entries.clear();
}

int UTGEPool::get_active_count() const {
    return active_entries.size();
}

int UTGEPool::get_released_count() const {
    return released_entries.size();
}

Node *UTGEPool::create_entry(const Ref<PackedScene> &scene) {
    Node *instance = instantiate_scene(scene);
    ERR_FAIL_NULL_V(instance, nullptr);

    Node *parent = get_parent();
    if(parent) {
        parent->add_child(instance);
    }

    PoolEntry entry;
    entry.node = instance;
    entry.scene = scene;
    entry.state = scene->get_state();
    entry.parent = parent;
    entry.index = parent ? instance->get_index() : -1;
    active_entries.insert(ObjectID(instance->get_instance_id()), entry);
    return instance;
}

UTGEPool::PoolEntry *UTGEPool::find_active_entry(Node *node) {
    return active_entries.getptr(ObjectID(node->get_instance_id()));
}

const UTGEPool::PoolEntry *UTGEPool::find_active_entry(Node *node) const {
    return active_entries.getptr(ObjectID(node->get_instance_id()));
}

UTGEPool::PoolEntry *UTGEPool::pop_released_entry(const Ref<PackedScene> &scene) {
    std::vector<ObjectID> *bucket = released_by_scene.getptr(scene);
    if(!bucket) return nullptr;
    while(!bucket->empty()) {
        ObjectID id = bucket->back();
        bucket->pop_back();

        PoolEntry *entry = released_entries.getptr(id);
        if(entry) return entry;
    }

    released_by_scene.erase(scene);
    return nullptr;
}

Node *UTGEPool::instantiate_scene(const Ref<PackedScene> &scene) const {
    ERR_FAIL_COND_V(scene.is_null(), nullptr);
    ERR_FAIL_COND_V(!scene->can_instantiate(), nullptr);

    Node *instance = scene->instantiate();
    ERR_FAIL_NULL_V(instance, nullptr);
    return instance;
}

void UTGEPool::reset_node(Node *target, const Ref<SceneState> &state) const {
    ERR_FAIL_NULL(target);
    ERR_FAIL_COND(state.is_null());

    for(int node_idx = 0; node_idx < state->get_node_count(); node_idx++) {
        Node *current = resolve_state_path(target, node_idx, state);
        if(!current) continue;
        int property_count = state->get_node_property_count(node_idx);
        for(int prop_idx = 0; prop_idx < property_count; prop_idx++) {
            StringName property_name = state->get_node_property_name(node_idx, prop_idx);
            if(property_name == StringName("script")) continue;
            current->set(property_name, state->get_node_property_value(node_idx, prop_idx));
        }
    }
}

void UTGEPool::detach_scripts(Node *node, HashMap<NodePath, Variant> &scripts) const {
    ERR_FAIL_NULL(node);
    detach_scripts_recursive(node, node, scripts);
}

void UTGEPool::detach_scripts_recursive(Node *root, Node *current, HashMap<NodePath, Variant> &scripts) const {
    ERR_FAIL_NULL(root);
    ERR_FAIL_NULL(current);

    NodePath path = root->get_path_to(current);
    scripts.insert(path, current->get_script());
    current->set_script(Variant());

    for(int i=0; i < current->get_child_count(); i++) {
        Node *child = current->get_child(i);
        ERR_CONTINUE(!child);
        detach_scripts_recursive(root, child, scripts);
    }
}

void UTGEPool::restore_scripts(Node *node, const HashMap<NodePath, Variant> &scripts) const {
    ERR_FAIL_NULL(node);

    NodePath root_path;
    const Variant *root_script = scripts.getptr(root_path);
    if(root_script) node->set_script(*root_script);

    for(HashMap<NodePath, Variant>::ConstIterator it = scripts.begin(); it != scripts.end(); ++it) {
        if(it->key.is_empty()) continue;
        Node *target = node->get_node_or_null(it->key);
        if(target) {
            target->set_script(it->value);
        }
    }
}

void UTGEPool::request_ready_recursive(Node *node) const {
    ERR_FAIL_NULL(node);

    node->request_ready();
    for(int i=0; i < node->get_child_count(); i++) {
        request_ready_recursive(node->get_child(i));
    }
}

Node *UTGEPool::resolve_state_path(Node *target, int node_index, const Ref<SceneState> &state) const {
    ERR_FAIL_NULL_V(target, nullptr);
    ERR_FAIL_COND_V(state.is_null(), nullptr);

    if(node_index == 0) return target;
    NodePath path = state->get_node_path(node_index);
    if(path.is_empty()) return target;
    return target->get_node_or_null(path);
}