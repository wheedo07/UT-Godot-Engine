#include "meter.h"
#include "bar.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/variant/callable.hpp>

AttackMeter::AttackMeter() {
    targetdef = 0;
    total_bars = 0;
    speed_mult = 1.0f;
    targetid = 0;
    target = 0;
    distance = 0;
    score = 0;
    misses = 0;
    crits = 0;
    hits = 0;
    can_crit = false;
    waiting_calculations = 0;
    initial_setup_complete = false;
}

AttackMeter::~AttackMeter() {}

void AttackMeter::_bind_methods() {
    ADD_SIGNAL(MethodInfo("calculated"));
    ADD_SIGNAL(MethodInfo("damagetarget", 
                         PropertyInfo(Variant::INT, "damage"), 
                         PropertyInfo(Variant::INT, "target"), 
                         PropertyInfo(Variant::BOOL, "crit")));
    ADD_SIGNAL(MethodInfo("missed", PropertyInfo(Variant::INT, "target")));
    
    ClassDB::bind_method(D_METHOD("_on_timeout", "position", "direction"), &AttackMeter::_on_timeout);

    ClassDB::bind_method(D_METHOD("remove_meter"), &AttackMeter::remove_meter);
    ClassDB::bind_method(D_METHOD("summonbar", "position", "direction", "delay"), &AttackMeter::summonbar);
    ClassDB::bind_method(D_METHOD("miss"), &AttackMeter::miss);
    ClassDB::bind_method(D_METHOD("calculate", "posx", "crit", "hspeed"), &AttackMeter::calculate);
    ClassDB::bind_method(D_METHOD("finalcalculation"), &AttackMeter::finalcalculation);
    ClassDB::bind_method(D_METHOD("_on_calculated"), &AttackMeter::_on_calculated);
    ClassDB::bind_method(D_METHOD("_on_bar_about_to_fade_out"), &AttackMeter::_on_bar_about_to_fade_out);
    
    ClassDB::bind_method(D_METHOD("set_targetdef", "targetdef"), &AttackMeter::set_targetdef);
    ClassDB::bind_method(D_METHOD("get_targetdef"), &AttackMeter::get_targetdef);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "targetdef"), "set_targetdef", "get_targetdef");
    
    ClassDB::bind_method(D_METHOD("set_target", "target"), &AttackMeter::set_target);
    ClassDB::bind_method(D_METHOD("get_target"), &AttackMeter::get_target);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "target"), "set_target", "get_target");
    
    ClassDB::bind_method(D_METHOD("set_targetid", "targetid"), &AttackMeter::set_targetid);
    ClassDB::bind_method(D_METHOD("get_targetid"), &AttackMeter::get_targetid);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "targetid"), "set_targetid", "get_targetid");
}

void AttackMeter::_ready() {
    if(isEditor) return;
    meter = Object::cast_to<Sprite2D>(get_node_internal("Meter"));

    bar_scene = ResourceLoader::get_singleton()->load("res://Battle/AttackMeter/bar.tscn");
    Ref<Item> weapon = global->get_item_list()[global->get_equipment()["weapon"]];
    can_crit = weapon->get_critical_hits();
    
    Color modulate = meter->get_modulate();
    modulate.a = 0;
    meter->set_modulate(modulate);
    
    Vector2 scale = meter->get_scale();
    scale.x = 0.33f;
    meter->set_scale(scale);
    
    Ref<Tween> tw = get_tree()->create_tween();
    tw->set_trans(transtype);
    tw->set_parallel(true);
    tw->set_ease(Tween::EASE_OUT);
    
    tw->tween_interval(0.1);
    
    Ref<Tween> chain_tw = tw->chain();
    chain_tw->tween_property(meter, "modulate:a", 1.0f, TIME / 2.0f);
    chain_tw->tween_property(meter, "scale:x", 1.0f, TIME);
    
    int randir = (UtilityFunctions::randi_range(0, 1) * 2) - 1;
    Vector2 summonpos = Vector2(320, 320);
    
    if (randir == 1) {
        summonpos.x = 40;
    } else if (randir == -1) {
        summonpos.x = 600;
    }
    
    total_bars = weapon->get_weapon_bars();
    speed_mult = weapon->get_weapon_speed();
    
    waiting_calculations = total_bars;
    
    for (int i = 0; i < total_bars; i++) {
        float delay = (UtilityFunctions::randi_range(0, 2) * 0.05f + 0.25f * i) / speed_mult;
        summonbar(summonpos, randir, delay);
    }
    
    initial_setup_complete = true;
}

void AttackMeter::remove_meter() {
    Ref<Tween> tw = get_tree()->create_tween();
    tw->set_trans(transtype);
    tw->set_parallel(true);
    tw->set_ease(Tween::EASE_IN);
    
    tw->tween_property(meter, "scale:x", 0, TIME);
    tw->tween_property(meter, "modulate:a", 0, TIME);
    
    Ref<Tween> chain_tw = tw->chain();
    chain_tw->tween_callback(Callable(this, "queue_free"))->set_delay(0.2);
}

void AttackMeter::summonbar(const Vector2& position, int direction, float delay) {
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(delay);
    
    timer->connect("timeout", Callable(this, "_on_timeout").bind(position, direction), CONNECT_ONE_SHOT);
}

void AttackMeter::_on_timeout(const Vector2& position, int direction) {
    AttackBar* clonebar = Object::cast_to<AttackBar>(bar_scene->instantiate());
    clonebar->connect("hit", Callable(this, "calculate"));
    clonebar->connect("miss", Callable(this, "miss"));
    
    clonebar->set_speed_mult(speed_mult);
    clonebar->set_position(position);
    clonebar->set_direction(direction);
    
    add_child(clonebar);
    move_child(clonebar, 1);
    
    clonebar->connect("about_to_fade_out", Callable(this, "_on_bar_about_to_fade_out"), CONNECT_ONE_SHOT);
    clonebar->connect("miss", Callable(this, "_on_bar_about_to_fade_out"), CONNECT_ONE_SHOT);
}

void AttackMeter::_on_bar_about_to_fade_out() {
    hits++;
    if ((hits + misses) >= total_bars) {
        remove_meter();
    }
}

void AttackMeter::miss() {
    misses++;
    emit_signal("calculated");
    _on_calculated();
}

void AttackMeter::calculate(int posx, bool crit, float hspeed) {
    crits += static_cast<int>(crit);
    
    if (meter) {
        distance = Math::abs(posx - meter->get_position().x);
    } else {
        distance = 0;
    }
    
    if (distance <= 12.0f) {
        distance = 12.0f;
    }
    
    distance /= 275.0f;
    
    if (total_bars <= 1) {
        distance = 2.0f * (1.0f - distance);
    } else {
        distance = distance * hspeed / 7.0f - 0.8f;
        
        if (distance >= 28.0f) score += 1;
        if (distance >= 22.0f && distance < 28.0f) score += 10;
        if (distance >= 16.0f && distance < 22.0f) score += 15;
        if (distance >= 10.0f && distance < 16.0f) score += 20;
        if (distance >= 5.0f && distance < 10.0f) score += 40;
        if (distance >= 4.0f && distance < 5.0f) score += 50;
        if (distance >= 3.0f && distance < 4.0f) score += 70;
        if (distance >= 2.0f && distance < 3.0f) score += 80;
        if (distance >= 1.0f && distance < 2.0f) score += 90;
        if (distance < 1.0f) score += 110;
    }
    
    emit_signal("calculated");
    _on_calculated();
}

void AttackMeter::_on_calculated() {
    // 초기 설정이 완료되지 않았으면 무시
    if (!initial_setup_complete) {
        return;
    }
    
    waiting_calculations--;
    
    if (waiting_calculations <= 0) {
        int damage = finalcalculation();
        
        if (misses < total_bars) {
            emit_signal("damagetarget", damage, target, crits == total_bars);
        } else {
            emit_signal("missed", target);
        }
    }
}

int AttackMeter::finalcalculation() {
    Dictionary equipment = global->get_equipment();
    TypedArray<Item> item_list = global->get_item_list();
    
    int player_attack = global->get_player_attack();
    int temp_atk = global->get_temp_atk();
    
    int damage = player_attack + 10;
    
    // 무기 및 방어구에서 추가 공격력 가져오기
    Ref<Item> weapon = item_list[equipment["weapon"]];
    if (weapon->get_attack_amount()) damage += weapon->get_attack_amount();
    
    Ref<Item> armor = item_list[equipment["armor"]];
    if (armor->get_attack_amount()) damage += armor->get_attack_amount();
    
    // 임시 공격력 추가
    damage += temp_atk;
    
    // 방어력 감소
    damage -= targetdef;
    
    if (total_bars <= 1) {
        return Math::round((damage + UtilityFunctions::randf_range(-2.0f, 2.0f)) * distance);
    }
    
    if (can_crit) {
        if (score > 440) score = static_cast<int>(score * 1.4f);
        if (score > 380) score = static_cast<int>(score * 1.2f);
    }
    
    return Math::round(damage * (score / 160.0f) * (4.0f / total_bars)) + 
           Math::round(UtilityFunctions::randf_range(-2.0f, 2.0f));
}

void AttackMeter::set_targetdef(int p_targetdef) {
    targetdef = p_targetdef;
}

int AttackMeter::get_targetdef() const {
    return targetdef;
}

void AttackMeter::set_target(int p_target) {
    target = p_target;
}

int AttackMeter::get_target() const {
    return target;
}

void AttackMeter::set_targetid(int p_targetid) {
    targetid = p_targetid;
}

int AttackMeter::get_targetid() const {
    return targetid;
}