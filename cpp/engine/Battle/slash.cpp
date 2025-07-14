#include "slash.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/material.hpp>
#include<godot_cpp/classes/particle_process_material.hpp>

Slash::Slash() {
    can_punch = false;
    z_count = 0;
    crit = false; 
    dmg_mult = 1.0;
    punch_finished = false;
    anim_finished = false;
}

Slash::~Slash() {}

void Slash::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished"));
    ADD_SIGNAL(MethodInfo("started"));
    ADD_SIGNAL(MethodInfo("punchfinish"));
    
    ClassDB::bind_method(D_METHOD("_on_punch_timer_timeout"), &Slash::_on_punch_timer_timeout);
    ClassDB::bind_method(D_METHOD("_on_animation_finished", "anim_name"), &Slash::_on_animation_finished);
    
    ClassDB::bind_method(D_METHOD("set_crit", "value"), &Slash::set_crit);
    ClassDB::bind_method(D_METHOD("get_crit"), &Slash::get_crit);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "crit"), "set_crit", "get_crit");
    
    ClassDB::bind_method(D_METHOD("set_dmg_mult", "value"), &Slash::set_dmg_mult);
    ClassDB::bind_method(D_METHOD("get_dmg_mult"), &Slash::get_dmg_mult);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dmg_mult"), "set_dmg_mult", "get_dmg_mult");
}

void Slash::_ready() {
    if(isEditor) return;
    anim_tree = Object::cast_to<AnimationTree>(get_node_internal("AnimationTree"));
    anim_tree_playback = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/playback"));
    
    particles = Object::cast_to<GPUParticles2D>(get_node_internal("GPUParticles2D3"));
    punch_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("PunchW"));
    sparkle_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("Sparkle"));
    press_z_label = Object::cast_to<Label>(get_node_internal("press_z"));
    anim_tree->connect("animation_finished", Callable(this, "_on_animation_finished"));

    Ref<ParticleProcessMaterial> material = particles->get_process_material();
    material->set_param_max(ParticleProcessMaterial::PARAM_SCALE, 0.2);
    material->set_param_min(ParticleProcessMaterial::PARAM_SCALE, 0.4);
    particles->set_process_material(material);
    
    // 크리티컬 히트 효과
    if (crit) {
        set_modulate(Color(1.0, 1.0, 0.54)); // Color("fffd8a")
    }
    
    active_tween = create_tween();
    active_tween->stop();
    
    process_weapon_animation();
}

void Slash::_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    if (event->is_action_pressed("ui_accept") && can_punch) {
        Ref<ParticleProcessMaterial> material = particles->get_process_material();
        float max = material->get_param_max(ParticleProcessMaterial::PARAM_SCALE);
        float min = material->get_param_min(ParticleProcessMaterial::PARAM_SCALE);
        material->set_param_max(ParticleProcessMaterial::PARAM_SCALE, max+0.1);
        material->set_param_min(ParticleProcessMaterial::PARAM_SCALE, min+0.1);
        particles->set_process_material(material);
        particles->set_amount_ratio(particles->get_amount_ratio() + 1.0f / 5.0f);
        
        z_count++;
        
        punch_sound->play();
        
        if (z_count > 2) {
            emit_signal("punchfinish");
            punch_finished = true;
        }
        
        String current_node = anim_tree_playback->get_current_node();
        if (current_node != "punch" && current_node != "explosion") {
            anim_tree_playback->start("punch");
        }
    }
}

void Slash::process_weapon_animation() {
    Dictionary equipment = global->get_equipment();
    TypedArray<Item> item_list = global->get_item_list();
    
    int weapon_index = equipment["weapon"];
    if (weapon_index >= 0 && weapon_index < item_list.size()) {
        Ref<Item> weapon = item_list[weapon_index];
        int weapon_type = weapon->get_weapon_type();
        
        emit_signal("started");
        
        switch (weapon_type) {
            case Item::KNIFE: {
                anim_tree_playback->start("slash");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.6);
                break;
            }
            case Item::PUNCH: {
                if (particles) {
                    particles->set_emitting(true);
                }
                can_punch = true;
                
                if (press_z_label) {
                    press_z_label->show();
                }
                
                punch_timer = memnew(Timer);
                punch_timer->set_one_shot(true);
                punch_timer->set_wait_time(0.8);
                add_child(punch_timer);
                punch_timer->connect("timeout", Callable(this, "_on_punch_timer_timeout"), CONNECT_ONE_SHOT);
                punch_timer->start();
                
                break;
            }
            case Item::SHOE: {
                anim_tree_playback->start("shoe");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.4);
                
                if (crit && sparkle_sound) {
                    sparkle_sound->play();
                }
                break;
            }
            case Item::BOOK: {
                anim_tree_playback->start("book");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.7);
                
                if (crit && sparkle_sound) {
                    sparkle_sound->play();
                }
                break;
            }
            case Item::PAN: {
                anim_tree_playback->start("pan");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.5);
                
                if (crit && sparkle_sound) {
                    sparkle_sound->play();
                }
                break;
            }
            case Item::GUN: {
                anim_tree_playback->start("gun");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.6);
                
                if (crit && sparkle_sound) {
                    sparkle_sound->play();
                }
                break;
            }
            default:
                UtilityFunctions::push_warning("Slash: Unknown weapon type");
                break;
        }

        if(weapon_type != Item::PUNCH) active_tween->play();
    }
}

void Slash::_on_punch_timer_timeout() {
    if (!punch_finished) {
        emit_signal("punchfinish");
        punch_finished = true;
    }
    
    can_punch = false;
    emit_signal("started");

    active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.5);
    active_tween->play();
    
    anim_tree_playback->next();
    
    if (anim_tree && z_count > 2) {
        anim_tree->set("parameters/conditions/pressed_z_times", true);
    } else {
        dmg_mult = Math::min(float(z_count) / 4.0f + 0.25f, 1.0f);
        
        anim_tree_playback->start("punch");
        
        if (anim_tree) {
            anim_tree->set("parameters/conditions/weak", true);
        }
    }
}

void Slash::_on_animation_finished(const StringName& anim_name) {
    String current_node = anim_tree_playback->get_current_node();
    if (current_node == "End") {
        anim_finished = true;
        hide();
        cleanup_and_free();
    }
}


void Slash::cleanup_and_free() {
    if (!active_tween.is_valid()) {
        emit_signal("finished");
    }
    
    call_deferred("queue_free");
}

void Slash::set_crit(bool p_crit) {
    crit = p_crit;
    if (is_inside_tree() && crit) {
        set_modulate(Color(1.0, 1.0, 0.54)); // Color("fffd8a")
    }
}

bool Slash::get_crit() const {
    return crit;
}

void Slash::set_dmg_mult(float p_dmg_mult) {
    dmg_mult = p_dmg_mult;
}

float Slash::get_dmg_mult() const {
    return dmg_mult;
}