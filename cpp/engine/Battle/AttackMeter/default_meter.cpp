#include "default_meter.h"
#include "env.h"
#include "bar.h"
#include "engine/Battle/encounter_script.h"
#include "engine/resources/Items/item.h"
#include "engine/resources/Items/weapon.h"
#include<godot_cpp/classes/resource_loader.hpp>

#define TIME 0.5f
DefaultMeter::DefaultMeter() {
    speed_mult = 0;
    total_bars = 0;
    can_crit = false;
    waiting_calculations = 0;
    hits = 0;
    misses = 0;
    distance = 0;
    score = 0;
    crits = 0;
    meter_scale = Vector2(1, 1);
}

DefaultMeter::~DefaultMeter() {}

void DefaultMeter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_timeout", "position", "direction"), &DefaultMeter::_on_timeout);
    ClassDB::bind_method(D_METHOD("_on_bar_about_to_fade_out"), &DefaultMeter::_on_bar_about_to_fade_out);
    ClassDB::bind_method(D_METHOD("calculate", "posx", "crit", "hspeed"), &DefaultMeter::calculate);
    ClassDB::bind_method(D_METHOD("miss"), &DefaultMeter::miss);
}

void DefaultMeter::_ready() {
    meter = Object::cast_to<Sprite2D>(get_node_internal("Meter"));
    bar_scene = ResourceLoader::get_singleton()->load("res://Engine/Battle/AttackMeter/bar.tscn");

    Ref<Item> item = encounter_script->get_player_weapon();
    if(item.is_valid() && item->is_class("Weapon")) {
        Ref<Weapon> weapon = item;
        can_crit = weapon->get_critical_hits();
        total_bars = weapon->get_weapon_bars();
        speed_mult = weapon->get_weapon_speed();
    }else {
        total_bars = WEAPON_DEFAULT_BARS;
        speed_mult = WEAPON_DEFAULT_SPEED;
        delay_offset = WEAPON_DEFAULT_DELAY;
        can_crit = WEAPON_DEFAULT_CRIT;
    }

    Color modulate = meter->get_modulate();
    modulate.a = 0;
    meter->set_modulate(modulate);
    
    meter->set_scale(Vector2(0.33f * meter_scale.x, meter_scale.y));
    
    Ref<Tween> tw = get_tree()->create_tween();
    tw->set_trans(transtype);
    tw->set_parallel(true);
    tw->set_ease(Tween::EASE_OUT);
    
    tw->tween_interval(0.1);
    
    Ref<Tween> chain_tw = tw->chain();
    chain_tw->tween_property(meter, "modulate:a", 1.0f, TIME / 2.0f);
    chain_tw->tween_property(meter, "scale:x", meter_scale.x, TIME);
    
    int randir = (UtilityFunctions::randi_range(0, 1) * 2) - 1;
    Vector2 summonpos = Vector2(320, 320);
    
    if(randir == 1) {
        summonpos.x = 40;
    }else if (randir == -1) {
        summonpos.x = 600;
    }

    waiting_calculations = total_bars;
    for(int i=0; i < total_bars; i++) {
        float base_delay = (0.25f * i * (1.0f + delay_offset * 0.3f)) / speed_mult;
        float random_delay = UtilityFunctions::randi_range(0, 2) * 0.05f;

        float delay = base_delay + random_delay;
        summonbar(summonpos, randir, delay);
    }
}

void DefaultMeter::remove_meter() {
    Ref<Tween> tw = get_tree()->create_tween();
    tw->set_trans(transtype);
    tw->set_parallel(true);
    tw->set_ease(Tween::EASE_IN);
    
    tw->tween_property(meter, "scale:x", 0, TIME);
    tw->tween_property(meter, "modulate:a", 0, TIME);
    
    Ref<Tween> chain_tw = tw->chain();
    chain_tw->tween_callback(Callable(this, "queue_free"))->set_delay(0.2);
}

void DefaultMeter::summonbar(Vector2 position, int direction, float delay) {
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(delay, false);
    timer->connect("timeout", Callable(this, "_on_timeout").bind(position, direction), CONNECT_ONE_SHOT);
}

void DefaultMeter::_on_timeout(Vector2 position, int direction) {
    AttackBar *clonebar = Object::cast_to<AttackBar>(bar_scene->instantiate());
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

void DefaultMeter::_on_bar_about_to_fade_out() {
    hits++;
    if ((hits + misses) >= total_bars) {
        remove_meter();
    }
}

void DefaultMeter::miss() {
    misses++;
    _on_calculated();
}

void DefaultMeter::calculate(int posx, bool crit, float hspeed) {
    crits += crit ? 1 : 0;
    
    if(meter) {
        distance = Math::abs(posx - meter->get_position().x);
    }else {
        distance = 0;
    }
    
    if(distance <= 12.0f) {
        distance = 12.0f;
    }
    
    distance /= 275.0f;
    
    if(total_bars <= 1) {
        distance = 2.0f * (1.0f - distance);
    }else {
        distance = distance * hspeed / 7.0f - 0.8f;
        
        if(distance >= 28.0f) score += 1;
        if(distance >= 22.0f && distance < 28.0f) score += 10;
        if(distance >= 16.0f && distance < 22.0f) score += 15;
        if(distance >= 10.0f && distance < 16.0f) score += 20;
        if(distance >= 5.0f && distance < 10.0f) score += 40;
        if(distance >= 4.0f && distance < 5.0f) score += 50;
        if(distance >= 3.0f && distance < 4.0f) score += 70;
        if(distance >= 2.0f && distance < 3.0f) score += 80;
        if(distance >= 1.0f && distance < 2.0f) score += 90;
        if(distance < 1.0f) score += 110;
    }
    
    _on_calculated();
}

void DefaultMeter::_on_calculated() {
    waiting_calculations--;
    
    if (waiting_calculations <= 0) {
        int damage = finalcalculation();
        
        if(misses < total_bars) {
            emit_signal("damagetarget", damage, enemy_id, crits == total_bars);
        }else {
            emit_signal("missed", enemy_id);
        }
    }
}

int DefaultMeter::finalcalculation() {
    int damage = global->get_player_attack() + 10;
    
    // 무기 및 방어구에서 추가 공격력 가져오기
    Ref<Item> weapon = encounter_script->get_player_weapon();
    int weapon_attack_amount = weapon->get("attack_amount");
    if(weapon_attack_amount) damage += weapon_attack_amount;
    
    Ref<Item> armor = encounter_script->get_player_armor();
    int armor_attack_amount = armor->get("attack_amount");
    if(armor_attack_amount) damage += armor_attack_amount;
    
    // 임시 공격력 추가
    damage += global->get_temp_atk();
    
    // 적 방어력 적용
    damage -= enemy_def;
    
    if(total_bars <= 1) {
        return Math::round((damage + UtilityFunctions::randf_range(-2.0f, 2.0f)) * distance);
    }
    
    if(can_crit) {
        if (score > 440) score = static_cast<int>(score * 1.4f);
        if (score > 380) score = static_cast<int>(score * 1.2f);
    }
    
    return Math::round(damage * (score / 160.0f) * (4.0f / total_bars)) + Math::round(UtilityFunctions::randf_range(-2.0f, 2.0f));
}

void DefaultMeter::set_meter_texture(Ref<Texture> texture) {
    meter->set_texture(texture);
}

void DefaultMeter::set_meter_scale(Vector2 scale) {
    meter_scale = scale;
}