#include "default_animation.h"
#include "env.h"
#include "engine/resources/Items/weapon_default.h"
#include<godot_cpp/classes/particle_process_material.hpp>

DefaultAnimation::DefaultAnimation() {
    can_punch = false;
    z_count = 0;
}

DefaultAnimation::~DefaultAnimation() {}

void DefaultAnimation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_animation_finished", "anim_name"), &DefaultAnimation::_on_animation_finished);
    ClassDB::bind_method(D_METHOD("_on_punch_timer_timeout"), &DefaultAnimation::_on_punch_timer_timeout);
}

void DefaultAnimation::_ready() {
    anim_tree = get_node<AnimationTree>("slash/AnimationTree");
    anim_tree_playback = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/playback"));
    particles = get_node<GPUParticles2D>("slash/Particles_PUNCH");
    press_z_label = get_node<Label>("slash/press_z");
    punch_sound = get_node<AudioStreamPlayer>("PunchW");
    sparkle_sound = get_node<AudioStreamPlayer>("Sparkle");

    Ref<ParticleProcessMaterial> material = particles->get_process_material();
    material->set_param_max(ParticleProcessMaterial::PARAM_SCALE, 0.2);
    material->set_param_min(ParticleProcessMaterial::PARAM_SCALE, 0.4);
    particles->set_process_material(material);
    
    if(get_crit()) {
        set_modulate(Color(1.0, 1.0, 0.54));
    }

    active_tween = create_tween();
    active_tween->stop();
    
    start();
}

void DefaultAnimation::_input(const Ref<InputEvent>& event) {
    if(event->is_action_pressed("ut_confirm") && can_punch) {
        Ref<ParticleProcessMaterial> material = particles->get_process_material();
        float max = material->get_param_max(ParticleProcessMaterial::PARAM_SCALE);
        float min = material->get_param_min(ParticleProcessMaterial::PARAM_SCALE);
        material->set_param_max(ParticleProcessMaterial::PARAM_SCALE, max+0.1);
        material->set_param_min(ParticleProcessMaterial::PARAM_SCALE, min+0.1);
        particles->set_process_material(material);
        particles->set_amount_ratio(particles->get_amount_ratio() + 1.0f / 5.0f);

        z_count++;
        
        punch_sound->play();
        
        String current_node = anim_tree_playback->get_current_node();
        if (current_node != "punch" && current_node != "explosion") {
            anim_tree_playback->start("punch");
        }
    }
}

void DefaultAnimation::start() {
    Dictionary equipment = global->get_equipment();
    Array item_list = global->get_item_list();

    int weapon_index = equipment["weapon"];
    if(weapon_index >= 0 && weapon_index < item_list.size()) {
        Ref<Item> item = item_list[weapon_index];
        int animation_type = 0; // 디폴트를 나이프로
        if(item->is_class("WeaponDefault")) {
            Ref<WeaponDefault> weapon = item_list[weapon_index];
            animation_type = weapon->get_weapon_animation();
        }
        emit_signal("started");
        
        switch(animation_type) {
            case WeaponDefault::KNIFE: {
                anim_tree_playback->start("slash");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.6);
                break;
            }
            case WeaponDefault::PUNCH: {
                if(particles) particles->set_emitting(true);
                can_punch = true;
                press_z_label->show();
                
                punch_timer = memnew(Timer);
                punch_timer->set_one_shot(true);
                punch_timer->set_wait_time(0.8);
                add_child(punch_timer);
                punch_timer->connect("timeout", Callable(this, "_on_punch_timer_timeout"), CONNECT_ONE_SHOT);
                punch_timer->start();
                break;
            }
            case WeaponDefault::SHOE: {
                anim_tree_playback->start("shoe");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.4);
                
                if(get_crit() && sparkle_sound) {
                    sparkle_sound->play();
                }
                break;
            }
            case WeaponDefault::BOOK: {
                anim_tree_playback->start("book");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.7);
                
                if(get_crit() && sparkle_sound) {
                    sparkle_sound->play();
                }
                break;
            }
            case WeaponDefault::PAN: {
                anim_tree_playback->start("pan");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.5);
                
                if(get_crit() && sparkle_sound) {
                    sparkle_sound->play();
                }
                break;
            }
            case WeaponDefault::GUN: {
                anim_tree_playback->start("gun");
                active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.6);
                
                if(get_crit() && sparkle_sound) {
                    sparkle_sound->play();
                }
                break;
            }
            default:
                ERR_PRINT("DefaultAnimation: Unknown weapon animation type");
                break;
        }

        if(animation_type != WeaponDefault::PUNCH) active_tween->play();
    }
}

void DefaultAnimation::_on_punch_timer_timeout() {
    can_punch = false;

    emit_signal("started");

    active_tween->tween_callback(Callable(this, "emit_signal").bind("finished"))->set_delay(0.5);
    active_tween->play();
    
    anim_tree_playback->next();
    
    if(anim_tree && z_count > 2) {
        anim_tree->set("parameters/conditions/pressed_z_times", true);
    }else {
        set_dmg_mult(Math::min(float(z_count) / 4.0f + 0.25f, 1.0f));
        anim_tree_playback->start("punch");
        
        if(anim_tree) {
            anim_tree->set("parameters/conditions/weak", true);
        }
    }
}

void DefaultAnimation::_on_animation_finished(StringName anim_name) {
    String current_node = anim_tree_playback->get_current_node();
    if(current_node == "End") {
        hide();
        emit_signal("finished");
        call_deferred("queue_free");
    }
}