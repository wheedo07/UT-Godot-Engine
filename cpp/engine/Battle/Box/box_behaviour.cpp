#include "box_behaviour.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/engine.hpp>

BattleBoxBehaviour::BattleBoxBehaviour() {
    enabled = false;
    box_path = NodePath("../..");
}

BattleBoxBehaviour::~BattleBoxBehaviour() {}

void BattleBoxBehaviour::_bind_methods() {
    ADD_SIGNAL(MethodInfo("change_state", PropertyInfo(Variant::INT, "new_state")));
    
    ClassDB::bind_method(D_METHOD("gain_control"), &BattleBoxBehaviour::gain_control);
    ClassDB::bind_method(D_METHOD("lose_control"), &BattleBoxBehaviour::lose_control);
    ClassDB::bind_method(D_METHOD("_on_gain_control"), &BattleBoxBehaviour::_on_gain_control);
    ClassDB::bind_method(D_METHOD("_on_lose_control"), &BattleBoxBehaviour::_on_lose_control);
    ClassDB::bind_method(D_METHOD("input", "event"), &BattleBoxBehaviour::input);
    ClassDB::bind_method(D_METHOD("_on_changed_state", "new_state"), &BattleBoxBehaviour::_on_changed_state);
    
    ClassDB::bind_method(D_METHOD("set_box_path", "box_path"), &BattleBoxBehaviour::set_box_path);
    ClassDB::bind_method(D_METHOD("get_box_path"), &BattleBoxBehaviour::get_box_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "box_path"), "set_box_path", "get_box_path");
}

void BattleBoxBehaviour::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    box = Object::cast_to<BattleBox>(get_node_internal(box_path));
}

void BattleBoxBehaviour::_input(const Ref<InputEvent>& event) {
    if(!enabled || isEditor) return;
    input(event);
}

void BattleBoxBehaviour::gain_control() {
    enabled = true;
    _on_gain_control();
}

void BattleBoxBehaviour::lose_control() {
    enabled = false;
    _on_lose_control();
}

void BattleBoxBehaviour::_on_gain_control() {}

void BattleBoxBehaviour::_on_lose_control() {}

void BattleBoxBehaviour::input(const Ref<InputEvent>& event) {}

void BattleBoxBehaviour::_on_changed_state(int new_state) {}

void BattleBoxBehaviour::set_box_path(const NodePath& p_box_path) {
    box_path = p_box_path;
}

NodePath BattleBoxBehaviour::get_box_path() const {
    return box_path;
}