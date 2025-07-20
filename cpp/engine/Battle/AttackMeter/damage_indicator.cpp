#include "damage_indicator.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/callback_tweener.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

DamageIndicator::DamageIndicator() {
    time = 0.6f;
    hp = 20;
    max_hp = 20;
    damage_amount = 5;
    miss = false;
}

DamageIndicator::~DamageIndicator() {}

void DamageIndicator::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished"));
    ADD_SIGNAL(MethodInfo("damagetarget", PropertyInfo(Variant::INT, "damage")));
    
    ClassDB::bind_method(D_METHOD("_on_frame_processed"), &DamageIndicator::_on_frame_processed);
    
    ClassDB::bind_method(D_METHOD("set_hp", "hp"), &DamageIndicator::set_hp);
    ClassDB::bind_method(D_METHOD("get_hp"), &DamageIndicator::get_hp);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "hp"), "set_hp", "get_hp");
    
    ClassDB::bind_method(D_METHOD("set_max_hp", "max_hp"), &DamageIndicator::set_max_hp);
    ClassDB::bind_method(D_METHOD("get_max_hp"), &DamageIndicator::get_max_hp);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_hp"), "set_max_hp", "get_max_hp");
    
    ClassDB::bind_method(D_METHOD("set_damage", "damage"), &DamageIndicator::set_damage);
    ClassDB::bind_method(D_METHOD("get_damage"), &DamageIndicator::get_damage);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "damage"), "set_damage", "get_damage");
    
    ClassDB::bind_method(D_METHOD("set_miss", "miss"), &DamageIndicator::set_miss);
    ClassDB::bind_method(D_METHOD("get_miss"), &DamageIndicator::get_miss);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "miss"), "set_miss", "get_miss");
    
    ClassDB::bind_method(D_METHOD("set_time", "time"), &DamageIndicator::set_time);
    ClassDB::bind_method(D_METHOD("get_time"), &DamageIndicator::get_time);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
}

void DamageIndicator::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) return;
    text_label = Object::cast_to<RichTextLabel>(get_node_internal("Hp"));
    health_bar = Object::cast_to<ProgressBar>(get_node_internal("Bar"));
    hit_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("Hit"));
    
    Color modulate_color = get_modulate();
    modulate_color.a = 0;
    set_modulate(modulate_color);
    
    if (health_bar) {
        health_bar->set_max(max_hp);
        health_bar->set_value(hp);
    }
    
    call_deferred("_on_frame_processed");
}

void DamageIndicator::_on_frame_processed() {
    Vector2 bar_size = health_bar->get_size();
    bar_size.x = Math::min(100.0f + max_hp / 5.0f, 560.0f);
    health_bar->set_size(bar_size);
    
    Vector2 bar_position = health_bar->get_position();
    bar_position.x = -bar_size.x / 2.0f;
    health_bar->set_position(bar_position);
    
    if (miss) {
        if (text_label) {
            text_label->set_text("[center][color=gray]MISS");
        }
        if (health_bar) {
            health_bar->hide();
        }
    } else if (damage_amount > 0) {
        if (hit_sound) {
            hit_sound->play();
        }
        if (text_label) {
            text_label->set_text("[center][color=red]" + String::num_int64(damage_amount));
        }
        emit_signal("damagetarget", damage_amount);
    } else {
        if (hit_sound) {
            hit_sound->play();
        }
        emit_signal("damagetarget", damage_amount);
        if (text_label) {
            text_label->set_text("[center][color=gray]0");
        }
        if (health_bar) {
            health_bar->hide();
        }
    }
    
    flash_tween = get_tree()->create_tween();
    flash_tween->set_loops(3);
    flash_tween->pause();
    flash_tween->tween_interval(0.1);
    flash_tween->tween_callback(Callable(this, "show"));
    flash_tween->tween_interval(0.1);
    flash_tween->tween_callback(Callable(this, "hide"));
    
    flash_tween->connect("finished", Callable(this, "emit_signal").bind("finished"));
    
    animation_tween = get_tree()->create_tween();
    animation_tween->set_parallel(true);
    animation_tween->set_trans(Tween::TRANS_CIRC);
    animation_tween->set_ease(Tween::EASE_OUT);
    
    value_tween = get_tree()->create_tween();
    value_tween->set_parallel(true);
    value_tween->set_trans(Tween::TRANS_CIRC);
    value_tween->set_ease(Tween::EASE_OUT);
    
    if (text_label) {
        Vector2 text_pos = text_label->get_position();
        animation_tween->tween_property(text_label, "position:y", text_pos.y - 10.0f, time / 2.0f);
    }
    
    animation_tween->tween_property(this, "modulate:a", 1.0f, time / 10.0f);
    
    if (health_bar) {
        value_tween->tween_property(health_bar, "value", health_bar->get_value() - damage_amount, time);
    }
    
    Ref<Tween> chain_tween = animation_tween->chain();
    chain_tween->tween_callback(Callable(flash_tween.ptr(), "play"))->set_delay(0.3);
    
    if (text_label) {
        Vector2 original_pos = text_label->get_position();
        chain_tween->tween_property(text_label, "position:y", original_pos.y, time)
                  ->set_ease(Tween::EASE_OUT)
                  ->set_trans(Tween::TRANS_BOUNCE);
    }
}

void DamageIndicator::set_hp(int p_hp) {
    hp = p_hp;
}

int DamageIndicator::get_hp() const {
    return hp;
}

void DamageIndicator::set_max_hp(int p_max_hp) {
    max_hp = p_max_hp;
}

int DamageIndicator::get_max_hp() const {
    return max_hp;
}

void DamageIndicator::set_damage(int p_damage) {
    damage_amount = p_damage;
}

int DamageIndicator::get_damage() const {
    return damage_amount;
}

void DamageIndicator::set_miss(bool p_miss) {
    miss = p_miss;
}

bool DamageIndicator::get_miss() const {
    return miss;
}

void DamageIndicator::set_time(float p_time) {
    time = p_time;
}

float DamageIndicator::get_time() const {
    return time;
}