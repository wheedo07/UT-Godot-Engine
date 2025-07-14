#include "bar.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/classes/scene_tree.hpp>

AttackBar::AttackBar() {
    speed_mult = 1.0f;
    single_bar = false;
    direction = 0;
    critzone = Vector2(310, 330);
    hityet = false;
    can_crit = false;
}

AttackBar::~AttackBar() {}

void AttackBar::_bind_methods() {
    ADD_SIGNAL(MethodInfo("hit", PropertyInfo(Variant::VECTOR2, "pos"), 
                         PropertyInfo(Variant::BOOL, "crit"), 
                         PropertyInfo(Variant::FLOAT, "speed")));
    ADD_SIGNAL(MethodInfo("miss"));
    ADD_SIGNAL(MethodInfo("about_to_fade_out"));
    
    ClassDB::bind_method(D_METHOD("_on_tween_finished"), &AttackBar::_on_tween_finished);
    ClassDB::bind_method(D_METHOD("_on_tween_finished2"), &AttackBar::_on_tween_finished2);
    
    ClassDB::bind_method(D_METHOD("set_direction", "direction"), &AttackBar::set_direction);
    ClassDB::bind_method(D_METHOD("get_direction"), &AttackBar::get_direction);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "direction"), "set_direction", "get_direction");
    
    ClassDB::bind_method(D_METHOD("set_speed_mult", "speed_mult"), &AttackBar::set_speed_mult);
    ClassDB::bind_method(D_METHOD("get_speed_mult"), &AttackBar::get_speed_mult);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed_mult"), "set_speed_mult", "get_speed_mult");
}

void AttackBar::_ready() {
    if(isEditor) return;
    overlay = Object::cast_to<ColorRect>(get_node_internal("Overlay"));
    animation_player = Object::cast_to<AnimationPlayer>(get_node_internal("AnimationPlayer"));
    hit_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("hit"));
    critical_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("critical"));
    
    Ref<Item> weapon = global->get_item_list()[global->get_equipment()["weapon"]];
    single_bar = weapon->get_weapon_bars() ? weapon->get_weapon_bars() == 1 : false;
    can_crit = weapon->get_critical_hits() ? weapon->get_critical_hits() : false;
    
    tw = get_tree()->create_tween();
    tw->set_ease(Tween::EASE_OUT);
    tw->set_trans(TRANSTYPE);
    tw->set_parallel(true);
    
    tw->tween_interval(Time);
    tw->set_ease(Tween::EASE_IN_OUT);
    
    float _dist = Math::abs(get_position().x - 320.0f) * 2.0f;
    tw->tween_property(this, "position:x", _dist * Math::sign(direction), _dist / (MOVE_SPEED * speed_mult))
      ->as_relative()
      ->set_trans(movetype);
    
    tw->connect("finished", Callable(this, "_on_tween_finished"), CONNECT_ONE_SHOT);
}

void AttackBar::_on_tween_finished() {
    // 화면 밖으로 나갔을 때 처리
    tw = get_tree()->create_tween();
    tw->set_ease(Tween::EASE_OUT);
    tw->set_trans(TRANSTYPE);
    tw->set_parallel(true);
    
    tw->tween_callback(Callable(this, "hide"));
    tw->tween_callback(Callable(this, "emit_signal").bind("miss"));
    tw->tween_callback(Callable(this, "queue_free"));
}

void AttackBar::_on_tween_finished2() {
    Ref<Tween> fade_tween = create_tween();
    fade_tween->set_trans(TRANSTYPE);
    fade_tween->tween_property(this, "modulate:a", 0.0, Time)->set_delay(Time);
    fade_tween->tween_callback(Callable(this, "queue_free"));
    
    emit_signal("about_to_fade_out");
}

void AttackBar::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor || hityet) return;
    if(event->is_action_pressed("ui_accept")) {
        if (tw.is_valid()) tw->kill();
        
        Vector2 pos = get_position();
        pos.x = Math::round(pos.x / 2.0f) * 2.0f;
        set_position(pos);
        hityet = true;
        get_viewport()->set_input_as_handled();
        bool is_crit = pos.x > critzone.x && pos.x < critzone.y && can_crit;
        emit_signal("hit", pos.x, is_crit, MOVE_SPEED * speed_mult);
        
        Ref<Tween> hit_tween = create_tween();
        hit_tween->set_ease(Tween::EASE_OUT);
        hit_tween->set_trans(TRANSTYPE);
        hit_tween->set_parallel(true);
        
        if (!single_bar) {
            // 다중 바 효과
            hit_tween->tween_property(this, "scale", Vector2(1.2, 1.5), Time);
            
            hit_tween->tween_property(overlay, "color:a", 0.0, Time);
            Color color = overlay->get_color();
            color.a = 1.0;
            overlay->set_color(color);

            Color color2 = overlay->get_modulate();
            color2.a = 1.0;
            overlay->set_modulate(color2);
            
            set_frame(2);
            
            hit_sound->play();
        } else {
            // 단일 바 효과
            animation_player->play("glow");
            hit_tween->tween_interval(1.4);
        }
        
        // 크리티컬 히트 처리
        if (can_crit) {
            if (pos.x > critzone.x && pos.x < critzone.y) {
                // 크리티컬 히트
                critical_sound->play();
                hit_tween->tween_property(this, "modulate:b", 0.0, Time / 2.0);
            } else if (pos.x > 70.0 && pos.x < 570.0) {
                // 일반 히트
                hit_sound->play();
                
                overlay->set("color:a", 0.7);
                
                hit_tween->tween_property(this, "modulate:r", 0.0, Time / 2.0);
            } else {
                // 빗나감
                hit_sound->play();
                
                hit_tween->tween_property(this, "modulate:g", 0.0, Time / 2.0);
                hit_tween->tween_property(this, "modulate:b", 0.0, Time / 2.0);
            }
        }
        
        hit_tween->connect("finished", Callable(this, "_on_tween_finished2"), CONNECT_ONE_SHOT);
    }
}

void AttackBar::set_direction(int p_direction) {
    direction = p_direction;
}

int AttackBar::get_direction() const {
    return direction;
}

void AttackBar::set_speed_mult(float p_speed_mult) {
    speed_mult = p_speed_mult;
}

float AttackBar::get_speed_mult() const {
    return speed_mult;
}