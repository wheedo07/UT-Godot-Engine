#include "attacks_manager.h"
#include "attacks.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

AttackManager::AttackManager() {}

AttackManager::~AttackManager() {}

void AttackManager::_bind_methods() {
    ADD_SIGNAL(MethodInfo("player_turn"));
    
    ClassDB::bind_method(D_METHOD("add_attack", "attack"), &AttackManager::add_attack);
    ClassDB::bind_method(D_METHOD("start_attack", "id"), &AttackManager::start_attack);
    ClassDB::bind_method(D_METHOD("force_end_attacks"), &AttackManager::force_end_attacks);
    ClassDB::bind_method(D_METHOD("end_attack", "id"), &AttackManager::end_attack);
    ClassDB::bind_method(D_METHOD("check_all_attacks_finished"), &AttackManager::check_all_attacks_finished);
}

void AttackManager::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) return;
    mask = Object::cast_to<Node2D>(get_node_internal("Mask"));
    
    top_left = Object::cast_to<Sprite2D>(mask->get_node_internal("TL"));
    bottom_right = Object::cast_to<Sprite2D>(mask->get_node_internal("BR"));
    
    current_attacks = Array();
}

Node* AttackManager::add_attack(const Ref<PackedScene>& attack) {
    if (!attack.is_valid()) {
        ERR_PRINT("PackedScene이 비어있음");
        return nullptr;
    }
    
    Node* attack_node = attack->instantiate();
    if (!attack_node) {
        ERR_PRINT("instantiate 실패");
        return nullptr;
    }

    if(!attack_node->is_class("AttackBase")) {
        ERR_PRINT("AttackBase가 아님");
        return nullptr;
    }
    
    add_child(attack_node, true);
    current_attacks.push_back(attack_node);
    
    attack_node->set("attack_id", current_attacks.size() - 1);
    attack_node->connect("remove_attack", Callable(this, "end_attack"));
    
    return attack_node;
}

void AttackManager::start_attacks() {
    for(int i = 0; i < current_attacks.size(); i++) {
        if(run_attack_id.find(i) != -1) continue;
        start_attack(i);
    }
}

void AttackManager::start_attack(int id) {
    if(id < 0 || id >= current_attacks.size()) {
        ERR_PRINT(vformat("attack ID %d가 없습니다", id));
        return;
    }
    
    AttackBase* attack = Object::cast_to<AttackBase>(current_attacks[id]);
    if(!attack) {
        ERR_PRINT(vformat("attack ID %d가 없습니다", id));
        return;
    }
    
    if(attack->has_method("start_attack")) { // C++ 이랑 GDscript 모두 호환되도록
        attack->call("start_attack");
    }else {
        attack->start_attack();
    }
    run_attack_id.append(id);
}

void AttackManager::force_end_attacks() {
    // 모든 공격 강제 종료
    for (int i = 0; i < current_attacks.size(); i++) {
        Node* attack = Object::cast_to<Node>(current_attacks[i]);
        attack->queue_free();
    }
    
    current_attacks.clear();
    run_attack_id.clear();
    emit_signal("player_turn");
}

void AttackManager::end_attack(int id) {
    if(id < 0 || id >= current_attacks.size()) {
        ERR_PRINT(vformat("attack ID %d가 없습니다", id));
        return;
    }
    
    Node* attack = Object::cast_to<Node>(current_attacks[id]);
    if(attack) {
        attack->queue_free();
        current_attacks[id] = Variant(); 
    }
    
    if(check_all_attacks_finished()) {
        current_attacks.clear();
        run_attack_id.clear();
        emit_signal("player_turn");
    }
}

bool AttackManager::check_all_attacks_finished() {
    bool finished = true;
    
    for(int i = 0; i < current_attacks.size(); i++) {
        Variant attack = current_attacks[i];
        if (attack.get_type() != Variant::Type::NIL) {
            finished = false;
            break;
        }
    }
    return finished;
}