#include "death_soul.h"
#include "env.h"
#include<godot_cpp/classes/scene_tree.hpp>

DeathSoul::DeathSoul() {
    soul_type = HUMAN;
    human_color = Color(1, 0, 0, 1);  // RED
    monster_color = Color(1, 1, 1, 1); // WHITE
    die_in_progress = false;
}

DeathSoul::~DeathSoul() {}

void DeathSoul::_bind_methods() {
    BIND_ENUM_CONSTANT(HUMAN);
    BIND_ENUM_CONSTANT(MONSTER);
    
    ADD_SIGNAL(MethodInfo("died"));
    ClassDB::bind_method(D_METHOD("die"), &DeathSoul::die);
    ClassDB::bind_method(D_METHOD("_on_first_timer_timeout"), &DeathSoul::_on_first_timer_timeout);
    ClassDB::bind_method(D_METHOD("_on_second_timer_timeout"), &DeathSoul::_on_second_timer_timeout);
    
    ClassDB::bind_method(D_METHOD("set_soul_type", "soul_type"), &DeathSoul::set_soul_type);
    ClassDB::bind_method(D_METHOD("get_soul_type"), &DeathSoul::get_soul_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "soul_type", PROPERTY_HINT_ENUM, "HUMAN,MONSTER"), "set_soul_type", "get_soul_type");
}

void DeathSoul::_ready() {
    if(isEditor) return;
    shards = Object::cast_to<GPUParticles2D>(get_node_internal("shards"));
    snap_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("snap"));
    shatter_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("shatter"));
    
    // 소울 타입에 따른 외관 설정
    if (soul_type == MONSTER) {
        set_modulate(monster_color);
        Vector2 scale_val = get_scale();
        scale_val.y = -1.0f;
        set_scale(scale_val);
    } else {
        set_modulate(human_color);
    }
}

void DeathSoul::die() {
    if(die_in_progress) return;
    
    die_in_progress = true;
    
    global->get_scene_container()->get_camera()->add_shake(0.15f);
    
    set_animation("death");
    snap_sound->play();
    
    first_timer = get_tree()->create_timer(0.5f);
    first_timer->connect("timeout", Callable(this, "_on_first_timer_timeout"));
}

void DeathSoul::_on_first_timer_timeout() {
    shards->set_emitting(true);
    shatter_sound->play();
    
    Color self_modulate_color = get_self_modulate();
    self_modulate_color.a = 0;
    set_self_modulate(self_modulate_color);
    global->get_scene_container()->get_camera()->add_shake(0.17f);
    
    second_timer = get_tree()->create_timer(2.0f);
    second_timer->connect("timeout", Callable(this, "_on_second_timer_timeout"));
}

void DeathSoul::_on_second_timer_timeout() {
    emit_signal("died");
    die_in_progress = false;
}

void DeathSoul::set_soul_type(SoulType p_soul_type) {
    soul_type = p_soul_type;
}

DeathSoul::SoulType DeathSoul::get_soul_type() const {
    return soul_type;
}