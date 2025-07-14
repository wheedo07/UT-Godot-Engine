#include "battle_transistion.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/particle_process_material.hpp>
using namespace godot;

BattleTransition::BattleTransition() {
    current_state = IDLE;
    state_processing = false;
}

BattleTransition::~BattleTransition() {}

void BattleTransition::_bind_methods() {
    ADD_SIGNAL(MethodInfo("completed"));
    ClassDB::bind_method(D_METHOD("transition"), &BattleTransition::transition);
    ClassDB::bind_method(D_METHOD("set_target", "target"), &BattleTransition::set_target);
    ClassDB::bind_method(D_METHOD("get_target"), &BattleTransition::get_target);
    
    ClassDB::bind_method(D_METHOD("on_timer_completed"), &BattleTransition::on_timer_completed);
    ClassDB::bind_method(D_METHOD("advance_transition_state"), &BattleTransition::advance_transition_state);
    ClassDB::bind_method(D_METHOD("process_blink_start"), &BattleTransition::process_blink_start);
    ClassDB::bind_method(D_METHOD("process_blink_hide_1"), &BattleTransition::process_blink_hide_1);
    ClassDB::bind_method(D_METHOD("process_blink_show_1"), &BattleTransition::process_blink_show_1);
    ClassDB::bind_method(D_METHOD("process_blink_hide_2"), &BattleTransition::process_blink_hide_2);
    ClassDB::bind_method(D_METHOD("process_blink_show_2"), &BattleTransition::process_blink_show_2);
    ClassDB::bind_method(D_METHOD("process_soul_drop"), &BattleTransition::process_soul_drop);
    ClassDB::bind_method(D_METHOD("on_tween_finished"), &BattleTransition::on_tween_finished);
    
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "target"), "set_target", "get_target");
}

void BattleTransition::_ready() {
    if(isEditor) return;
    black_rect = Object::cast_to<ColorRect>(get_node_internal("Black"));
    soul = Object::cast_to<Node2D>(get_node_internal("Black/Soul"));
    ghost = Object::cast_to<GPUParticles2D>(get_node_internal("Black/Soul/Ghost"));
    noise_player = Object::cast_to<AudioStreamPlayer>(get_node_internal("noise"));
    drop_player = Object::cast_to<AudioStreamPlayer>(get_node_internal("drop"));
}

void BattleTransition::transition() {
    if (!global || !soul || !black_rect || !noise_player || !drop_player) {
        ERR_PRINT("필요한 노드를 찾을 수 없습니다");
        queue_free();
        return;
    }
    
    soul->set_position(global->get_player_position());
    
    noise_player->play();
    current_state = BLINK_START;
    
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.08);
    timer->connect("timeout", Callable(this, "on_timer_completed"));
    state_processing = true;
}

void BattleTransition::on_timer_completed() {
    if(!state_processing) return;
    advance_transition_state();
}

void BattleTransition::advance_transition_state() {
    switch (current_state) {
        case BLINK_START:
            process_blink_hide_1();
            break;
        case BLINK_HIDE_1:
            process_blink_show_1();
            break;
        case BLINK_SHOW_1:
            process_blink_hide_2();
            break;
        case BLINK_HIDE_2:
            process_blink_show_2();
            break;
        case BLINK_SHOW_2:
            process_soul_drop();
            break;
        case SOUL_DROP:
            // 트윈에서 처리됨
            break;
        case COMPLETED:
            queue_free();
            break;
        default:
            break;
    }
}

void BattleTransition::process_blink_start() {
    current_state = BLINK_HIDE_1;
}

void BattleTransition::process_blink_hide_1() {
    if (!black_rect) return;
    
    black_rect->set_visible(false);
    current_state = BLINK_HIDE_1;
    
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.08);
    timer->connect("timeout", Callable(this, "on_timer_completed"));
}

void BattleTransition::process_blink_show_1() {
    if (!black_rect || !noise_player) return;
    
    black_rect->set_visible(true);
    noise_player->play();
    current_state = BLINK_SHOW_1;
    
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.08);
    timer->connect("timeout", Callable(this, "on_timer_completed"));
}

void BattleTransition::process_blink_hide_2() {
    if (!black_rect) return;
    
    black_rect->set_visible(false);
    current_state = BLINK_HIDE_2;
    
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.08);
    timer->connect("timeout", Callable(this, "on_timer_completed"));
}

void BattleTransition::process_blink_show_2() {
    if (!black_rect || !noise_player) return;
    
    black_rect->set_visible(true);
    noise_player->play();
    current_state = BLINK_SHOW_2;
    
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.08);
    timer->connect("timeout", Callable(this, "on_timer_completed"));
}

void BattleTransition::process_soul_drop() {
    if (!drop_player || !soul || !ghost) return;
    
    drop_player->play();
    
    Ref<Tween> tween = create_tween();
    tween->set_ease(Tween::EASE_OUT);
    tween->set_trans(Tween::TRANS_EXPO);
    
    ghost->set_emitting(true);
    
    tween->tween_property(soul, "position", target, 0.8)->set_delay(0.07);
    tween->tween_property(soul, "modulate:a", 0.0, 0.3);
    
    tween->connect("finished", Callable(this, "on_tween_finished"));
    
    current_state = SOUL_DROP;
}

void BattleTransition::on_tween_finished() {
    current_state = COMPLETED;
    state_processing = false;
    emit_signal("completed");
    queue_free();
}

void BattleTransition::set_target(const Vector2& p_target) {
    target = p_target;
}

Vector2 BattleTransition::get_target() const {
    return target;
}