#include "battle_system.h"
#include "env.h"
#include "dust_transition.h"
#include "encounter_script.h"
#include "AttackMeter/meter.h"
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/window.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>

BattleMain::BattleMain() {
    turn_number = 0;
    completed_enemies = 0;
    completed_size = 0;
    kr = false;
    is_first_turn = false;
    transparent = false;
    rewards["gold"] = 0;
    rewards["exp"] = 0;
    player_turn = true;
}

BattleMain::~BattleMain() {}

void BattleMain::_bind_methods() {
    ADD_SIGNAL(MethodInfo("end_turn"));

    // 배틀 액션 신호
    ADD_SIGNAL(MethodInfo("fight_used", PropertyInfo(Variant::INT, "target")));
    ADD_SIGNAL(MethodInfo("act_used", PropertyInfo(Variant::INT, "target"), PropertyInfo(Variant::INT, "option")));
    ADD_SIGNAL(MethodInfo("item_used", PropertyInfo(Variant::INT, "id")));
    ADD_SIGNAL(MethodInfo("mercy_used", PropertyInfo(Variant::INT, "choice")));
    ADD_SIGNAL(MethodInfo("enemy_killed", PropertyInfo(Variant::INT, "enemy_id")));
    ADD_SIGNAL(MethodInfo("enemy_spared", PropertyInfo(Variant::INT, "enemy_id")));
    ADD_SIGNAL(MethodInfo("damage_info_finished"));
    
    ClassDB::bind_method(D_METHOD("enemy_size"), &BattleMain::enemy_size);
    ClassDB::bind_method(D_METHOD("is_kr"), &BattleMain::is_kr);
    ClassDB::bind_method(D_METHOD("toggle_transparent"), &BattleMain::toggle_transparent);
    ClassDB::bind_method(D_METHOD("kill_enemy", "enemy_id"), &BattleMain::kill_enemy, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("spare_enemy", "enemy_id"), &BattleMain::spare_enemy, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("check_end_encounter"), &BattleMain::check_end_encounter);
    ClassDB::bind_method(D_METHOD("check_enemy_solo"), &BattleMain::check_enemy_solo);
    ClassDB::bind_method(D_METHOD("add_enemy", "enemy_scene"), &BattleMain::add_enemy);
    ClassDB::bind_method(D_METHOD("reset_pos_camera"), &BattleMain::reset_pos_camera);
   
    ClassDB::bind_method(D_METHOD("_no_enemies_handler"), &BattleMain::_no_enemies_handler);
    ClassDB::bind_method(D_METHOD("_on_get_turn"), &BattleMain::_on_get_turn);
    ClassDB::bind_method(D_METHOD("_on_end_turn"), &BattleMain::_on_end_turn);
    ClassDB::bind_method(D_METHOD("_on_player_turn_start"), &BattleMain::_on_player_turn_start);
    ClassDB::bind_method(D_METHOD("_on_enemy_turn_start"), &BattleMain::_on_enemy_turn_start);
    ClassDB::bind_method(D_METHOD("_fight", "target"), &BattleMain::_fight);
    ClassDB::bind_method(D_METHOD("_hit", "damage", "target", "crit"), &BattleMain::_hit, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("_miss", "target"), &BattleMain::_miss);
    ClassDB::bind_method(D_METHOD("_act", "target", "option"), &BattleMain::_act);
    ClassDB::bind_method(D_METHOD("_mercy", "choice"), &BattleMain::_mercy);
    ClassDB::bind_method(D_METHOD("_item", "item_id"), &BattleMain::_item);
    ClassDB::bind_method(D_METHOD("_on_slash_finished", "damage", "target", "crit"), &BattleMain::_on_slash_finished);
    ClassDB::bind_method(D_METHOD("_on_spare_finished", "enemy"), &BattleMain::_on_spare_finished);
    ClassDB::bind_method(D_METHOD("_on_damage_info_completed", "target", "miss"), &BattleMain::_on_damage_info_completed);
    ClassDB::bind_method(D_METHOD("_on_fight_used_completed", "target"), &BattleMain::_on_fight_used_completed);
    ClassDB::bind_method(D_METHOD("_on_action", "action"), &BattleMain::_on_action);
    ClassDB::bind_method(D_METHOD("_on_transparent"), &BattleMain::_on_transparent);
    ClassDB::bind_method(D_METHOD("_on_end"), &BattleMain::_on_end, DEFVAL(false), DEFVAL(-1));
    ClassDB::bind_method(D_METHOD("_on_kill_enemy", "enemy"), &BattleMain::_on_kill_enemy);
    ClassDB::bind_method(D_METHOD("_encounter_script_add", "on"), &BattleMain::_encounter_script_add);
    
    ClassDB::bind_method(D_METHOD("set_encounter", "encounter"), &BattleMain::set_encounter);
    ClassDB::bind_method(D_METHOD("get_encounter"), &BattleMain::get_encounter);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "encounter", PROPERTY_HINT_RESOURCE_TYPE, "Encounter"), "set_encounter", "get_encounter");

    ClassDB::bind_method(D_METHOD("set_turn_number", "value"), &BattleMain::set_turn_number);
    ClassDB::bind_method(D_METHOD("get_turn_number"), &BattleMain::get_turn_number);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "turn_number", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_turn_number", "get_turn_number");

    ClassDB::bind_method(D_METHOD("set_property", "value"), &BattleMain::set_property);
    ClassDB::bind_method(D_METHOD("get_buttons"), &BattleMain::get_buttons);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "buttons", PROPERTY_HINT_NONE, "BattleButtons", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_buttons");
}

void BattleMain::_ready() {
    camera = Object::cast_to<CameraController>(get_node_internal("Camera"));
    buttons = Object::cast_to<BattleButtons>(get_node_internal("Buttons"));
    box = Object::cast_to<BattleBox>(get_node_internal("BattleBox"));
    enemies_node = Object::cast_to<Node2D>(get_node_internal("Enemies"));
    bg = Object::cast_to<TextureRect>(get_node_internal("Background/Texture"));
    soul_battle = Object::cast_to<SoulBattle>(get_node_internal("Soul_Battle"));
    attacks = Object::cast_to<AttackManager>(get_node_internal("Attacks/BoxClipper"));
    attacks_parent = Object::cast_to<Node>(get_node_internal("Attacks"));
    hud = Object::cast_to<BattleHUD>(get_node_internal("HUD"));
    script_node = Object::cast_to<EncounterScript>(get_node_internal("ScriptNode"));
    lvlup_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("lvlup"));
    music_player = global->get_Music();

    ResourceLoader* loader = ResourceLoader::get_singleton();
    attack_scene = loader->load("res://Engine/Battle/AttackMeter/meter.tscn");
    slash_scene = loader->load("res://Engine/Battle/Slashes/slashes.tscn");
    damage_info_scene = loader->load("res://Engine/Battle/AttackMeter/damage.tscn");

    if(!encounter.is_valid() && global->battle_encounter && global->battle_encounter->is_class("Encounter")) {
        encounter = global->battle_encounter;
    }

    global->_set_battle_start(true);
    if(encounter.is_valid()) {
        initialize();
    }else {
        ERR_PRINT("Encounter가 설정되지 않았습니다.");
    }
}

void BattleMain::_process(double delta) {
    bg->set_position(encounter->get_offset());
}

void BattleMain::initialize() {
    bg->set_texture(encounter->get_background());
    buttons->set_button(encounter->get_button_set());
    box->get_blitter_text()->set_encounter(encounter);

    Ref<Script> encounter_script = encounter->get_encounter_script();
    if(encounter_script.is_valid()) {
        script_node->set_script(encounter_script);
        remove_child(script_node);
        get_tree()->connect("process_frame", Callable(this, "_encounter_script_add").bind(false), CONNECT_ONE_SHOT);
    }

    Array enemy_scenes = encounter->get_enemies().duplicate();
    Ref<AudioStream> music = encounter->get_music();
    if(music_player && music.is_valid()) {
        music_player->set_stream(music);
        music_player->play();
    }else {
        music_player->stop();
    }
    
    if(enemy_scenes.size() == 0) {
        soul_battle->hide();
        get_tree()->connect("process_frame", Callable(this, "_no_enemies_handler"), CONNECT_ONE_SHOT);
        return;
    }
    
    enemy_names = enemy_scenes;
    
    for (int i = 0; i < enemy_scenes.size(); i++) {
        Ref<PackedScene> enemy_scene = enemy_scenes[i];
        if (enemy_scene.is_valid()) {
            Node* enemy_instance = enemy_scene->instantiate();
            if (enemy_instance) {
                enemies_node->add_child(enemy_instance);
                Enemy* enemy = Object::cast_to<Enemy>(enemy_instance);
                if (enemy) {
                    if (enemy_scenes.size() == 2) {
                        enemy->set_position(Vector2(i == 0 ? -100 : 100, 0));
                    } else if (enemy_scenes.size() == 3 && i != 1) {
                        enemy->set_position(Vector2(i == 0 ? -200 : 200, 0));
                    }
                }
            }
        }
    }
    
    for(int i=0; i < enemies_node->get_child_count(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies_node->get_child(i));
        enemies.append(enemy);
    }
    box->set_enemies(enemies);
    
    for(int i=0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        // 복수 적 처리
        if(enemies.size() > 1) {
            enemy->set_solo(false);
        }
        
        if(enemy->get_kr()) {
            kr = true;
            hud->set_kr();
        }
        
        enemy->set_id(i);
        enemy->connect("changed_act", Callable(box, "_set_targets"));
        
        Dictionary stats = enemy->get_stats();
        enemies_max_hp.append(stats.get("max_hp", 1));
        enemies_def.append(stats.get("def", 0));
        
        PackedStringArray flavour_texts = encounter->get_flavour_text();
        if(i == 0 && flavour_texts.size() == 0) {
            PackedStringArray default_text = { "* " + tr(enemy->get_enemy_name()) + String::utf8("!!") };
            encounter->set_flavour_text(default_text);
        }
        
        Dictionary rwrds = enemy->get_rewards();
        rewards["gold"] = int(rewards["gold"]) + int(rwrds.get("gold", 0));
        rewards["exp"] = int(rewards["exp"]) + int(rwrds.get("exp", 0));
        
        if(enemy->get_is_first_turn() && !is_first_turn) {
            call_deferred("_on_get_turn");
            is_first_turn = true;
        }
    }
    
    if(is_first_turn) {
        buttons->disable();
        soul_battle->enable();
        player_turn = false;
    } else {
        box->blitter_flavour();
        buttons->_enable();
    }
    
    Node* tl = box->get_node_internal("BoxContainer/TL");
    Node* br = box->get_node_internal("BoxContainer/BR");
    tl->call("set_remote_node", tl->call("get_path_to", attacks->top_left));
    br->call("set_remote_node", tl->call("get_path_to", attacks->bottom_right));
    
    CameraFx* fx = camera->get_global_camera();
    fx->show_blinder();
    fx->blind(0.5, 0);
}

void BattleMain::_no_enemies_handler() {
    box->blitter_print({ tr("UT_NOBODY_CAME") });
    box->connect("blitter_end", Callable(this, "_on_action").bind("end_battle"), CONNECT_ONE_SHOT);
}

void BattleMain::_on_action(const String& action) {
    if(action == "end_battle") {
        global->set_temp_atk(0);
        global->set_temp_def(0);
        soul_battle->queue_free();
        scene_changer->load_cached_scene();
    }else if(action == "script_off") {
        for(int i=0; i < enemies.size(); i++) {
            Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
            enemy->set_script(enemies_script[i]);
            enemy->request_ready();
            enemies_node->add_child(enemy);
        }
    }else if(action == "kill_enemy") {
        int kills = global->get_player_kills();
        kills++;
        global->set_player_kills(kills);
        box->_disable();
        
        if(check_end_encounter()) {
            end_encounter();
        }else {
            bool solo = check_enemy_solo();
            for (int i = 0; i < enemies.size(); i++) {
                Enemy* e = Object::cast_to<Enemy>(enemies[i]);
                if (e) {
                    e->set_solo(solo);
                }
            }
            emit_signal("end_turn");
        }
    }else if(action == "spare_enemy") {
        if(check_end_encounter()) {
            end_encounter();
        }else {
            bool solo = check_enemy_solo();
            for(int i=0; i < enemies.size(); i++) {
                Enemy* e = Object::cast_to<Enemy>(enemies[i]);
                if (e) {
                    e->set_solo(solo);
                }
            }
        }
    }
}

void BattleMain::_on_player_turn_start() {
    if(transparent) toggle_transparent();
    buttons->_enable();
    box->blitter_flavour();
    player_turn = true;
    soul_battle->menu_enable();

    // 디버그 관련
    global->isDebugTurn = true;
}

void BattleMain::_on_enemy_turn_start() {
    turn_number += 1;
    attacks->force_end_attacks();
    player_turn = false;
    soul_battle->enable();

    // 디버그 관련
    global->isDebugTurn = true;
}

void BattleMain::_enemy_script_off() {
    enemies_script.clear();
    for(int i=0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        enemies_script.append(enemy->get_script());
        enemy->set_script(Variant());
        enemies_node->remove_child(enemy);
    }
    get_tree()->connect("process_frame", Callable(this, "_on_action").bind("script_off"), CONNECT_ONE_SHOT);
}

PackedStringArray BattleMain::_on_death_player() {
    for(int i=0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy->has_method("handle_victory")) { // C++ 이랑 GDscript 모두 호환되도록
            return enemy->call("handle_victory");
        }else {
            return enemy->handle_victory();
        }
    }
    return {};
}

void BattleMain::_fight(int target) {
    if(!attack_scene.is_valid() || !box) return;

    AttackMeter* meter = Object::cast_to<AttackMeter>(attack_scene->instantiate());
    emit_signal("fight_used", target);
    meter->set("target", target);
    meter->connect("damagetarget", Callable(this, "_hit"), CONNECT_ONE_SHOT);
    meter->connect("missed", Callable(this, "_miss"), CONNECT_ONE_SHOT);
    meter->set("targetdef", enemies_def[target]);
    box->add_child(meter);

    Ref<BoxSet> box_set = encounter->get_box_set();
    if(box_set.is_valid()) {
        Ref<Texture> meter_texture = box_set->get_meter_texture();
        if(meter_texture.is_valid()) {
            meter->set_meter_texture(meter_texture);
        }
        meter->set_scale(box_set->get_meter_scale());
    }
}

void BattleMain::_hit(int damage, int target, bool crit) {
    if(!slash_scene.is_valid() || !box || target < 0 || target >= enemies.size()) return;
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
    if(!enemy) return;
    
    Slash* slashes = Object::cast_to<Slash>(slash_scene->instantiate());
    if(slashes) {
        if(enemy->get_dodging()) {
            int dodge_sign = (UtilityFunctions::randi_range(0, 1) * 2) - 1;
            slashes->connect("started", Callable(enemy, "emit_signal").bind("dodged", dodge_sign == 1), CONNECT_ONE_SHOT);
            slashes->connect("started", Callable(enemy, "_dodge").bind(dodge_sign), CONNECT_ONE_SHOT);
        }else slashes->connect("started", Callable(enemy, "emit_signal").bind("hit", damage), CONNECT_ONE_SHOT);
        
        slashes->connect("finished", Callable(this, "_on_slash_finished").bind(damage, target, crit), CONNECT_ONE_SHOT);

        slashes->set_crit(crit);
        box->add_child(slashes, true);
        Node* enemy_sprites = enemy->get_sprites();
        if(!enemy_sprites) {
            ERR_PRINT("Enemy의 sprites 노드가 없습니다");
            return;
        }
        Vector2 offset = enemy_sprites->call("get_global_position");
        if(enemy_sprites->has_method("get_pivot_offset")) {
            offset += enemy_sprites->call("get_pivot_offset");
        }

        slashes->set_global_position(offset);
    }
}

void BattleMain::_on_slash_finished(int damage, int target, bool crit) {
    Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
    if (!enemy) return;
    
    Slash* slashes = nullptr;
    for (int i = 0; i < box->get_child_count(); i++) {
        slashes = Object::cast_to<Slash>(box->get_child(i));
        if (slashes) break;
    }
    
    if (!slashes) return;
    damage = floor(damage * slashes->get_dmg_mult());
    
    if (damage_info_scene.is_valid()) {
        Node* clone = damage_info_scene->instantiate();
        if (clone) {
            clone->connect("damagetarget", Callable(enemy, "_hurt"), CONNECT_ONE_SHOT);
            clone->call("set_global_position", slashes->get_global_position());
            clone->set("hp", box->enemies_hp[target]);
            clone->set("max_hp", enemies_max_hp[target]);
            
            if (enemy->get_dodging()) {
                clone->set("miss", true);
            } else {
                clone->set("damage", damage);
                String info;
                if(enemy->has_method("damage_info")) { // C++ 이랑 GDscript 모두 호환되도록
                    info = enemy->call("damage_info", damage > 0 ? damage : 0);
                }else {
                    info = enemy->damage_info(damage > 0 ? damage : 0);
                }
                clone->set("info", info);
                if(damage > 0) box->enemies_hp[target] = (float)box->enemies_hp[target] - damage;
            }
            
            box->add_child(clone);
            clone->connect("finished", Callable(this, "_on_damage_info_completed").bind(target, false), CONNECT_ONE_SHOT);
        }
    }
}

void BattleMain::_on_damage_info_completed(int target, bool miss) {
    emit_signal("damage_info_finished");
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
    if (!enemy) return;
    enemy->connect("on_fight_end", Callable(this, "_on_fight_used_completed").bind(target), CONNECT_ONE_SHOT);
    if(enemy->has_method("on_fight")) { // C++ 이랑 GDscript 모두 호환되도록
        enemy->call("on_fight", miss);
    }else enemy->on_fight(miss);
}

void BattleMain::_on_fight_used_completed(int target) {
    if(box) 
    box->_disable();
    
    if (float(box->enemies_hp[target]) < 0) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
        if (enemy) {
            kill_enemy(target);
        }
    } else {
        emit_signal("end_turn");
    }
}

void BattleMain::_miss(int target) {
    if (!damage_info_scene.is_valid() || !box || target < 0 || target >= enemies.size()) return;
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
    if (!enemy) return;
    
    Node* clone = damage_info_scene->instantiate();
    if (clone) {
        clone->call("set_global_position", enemy->get_sprites()->call("get_global_position"));
        clone->set("hp", box->enemies_hp[target]);
        clone->set("max_hp", enemies_max_hp[target]);
        clone->set("miss", true);
        box->add_child(clone);
        clone->connect("finished", Callable(this, "_on_damage_info_completed").bind(target, true), CONNECT_ONE_SHOT);
    }
}

void BattleMain::_act(int target, int option) {
    if(target < 0 || target >= enemies.size()) return;
    emit_signal("act_used", target, option);
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
    if (enemy) {
        enemy->connect("on_act_end", Callable(this, "emit_signal").bind("end_turn"), CONNECT_ONE_SHOT);
        if(enemy->has_method("on_act")) { // C++ 이랑 GDscript 모두 호환되도록
            enemy->call("on_act", option);
        }else enemy->on_act(option);
    }
}

void BattleMain::_mercy(int choice) {
    emit_signal("mercy_used", choice);
    switch (choice) {
        case -1:
            emit_signal("end_turn");
            break;
        case 0:
            completed_size = enemy_size();
            for (int i = 0; i < enemies.size(); i++) {
                Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
                if (enemy) {
                    Ref<EnemyAct> state = enemy->get_enemy_acts()[enemy->get_current_act()];
                    enemy->connect("on_mercy_end", Callable(this, "_on_end").bind(state->get_sparable(), i), CONNECT_ONE_SHOT);
                    if(enemy->has_method("on_mercy")) { // C++ 이랑 GDscript 모두 호환되도록
                        enemy->call("on_mercy");
                    }else enemy->on_mercy();
                }
            }
            break;
        case 1: {
            CameraFx* camera_node = global->get_scene_container()->get_camera();
            camera_node->blind(0, 1);
            camera_node->connect("finished_tween", Callable(this, "_on_action").bind("end_battle"), CONNECT_ONE_SHOT);
            break;
        }
    }
}

void BattleMain::_on_end(bool mercy, int id) {
    if(mercy) spare_enemy(id);

    completed_enemies++;
    if(completed_enemies >= completed_size && !check_end_encounter()) {
        completed_enemies = 0;
        emit_signal("end_turn");
    }
}

void BattleMain::_item(int item_id) {
    emit_signal("item_used", item_id);
    completed_size = enemy_size();
    for(int i = 0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy) {
            enemy->connect("on_item_end", Callable(this, "_on_end"), CONNECT_ONE_SHOT);
            if(enemy->has_method("on_item")) { // C++ 이랑 GDscript 모두 호환되도록
                enemy->call("on_item", item_id);
            }else enemy->on_item(item_id);
        }
    }
}

void BattleMain::kill_enemy(int enemy_id) {
    if(enemy_id < 0 || enemy_id >= enemies.size()) return;
    emit_signal("enemy_killed", enemy_id);
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[enemy_id]);
    if(enemy) {
        enemies[enemy_id] = nullptr;
        enemy_names[enemy_id] = Variant();
        box->set_enemies(enemies);

        enemy->connect("on_defeat_end", Callable(this, "_on_kill_enemy").bind(enemy), CONNECT_ONE_SHOT);
        if(enemy->has_method("on_defeat")) { // C++ 이랑 GDscript 모두 호환되도록
            enemy->call("on_defeat", true);
        }else enemy->on_defeat(true);
    }
}

void BattleMain::_on_kill_enemy(Enemy* enemy) {
    DustTransition* dust = enemy->get_dust();
    if(!dust) {
        ERR_PRINT("Enemy 노드에 필요한 dust 노드가 없습니다");
        return;
    }
    dust->start_transition();
    dust->connect("finished", Callable(enemy, "queue_free"), CONNECT_ONE_SHOT);
    dust->connect("finished", Callable(this, "_on_action").bind("kill_enemy"), CONNECT_ONE_SHOT);
}

bool BattleMain::check_enemy_solo() {
    int enemy_count = 0;
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if (enemy) {
            enemy_count++;
        }
    }
    return enemy_count == 1;
}

bool BattleMain::check_end_encounter() {
    bool empty = true;
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if (enemy) {
            empty = false;
            break;
        }
    }
    return empty;
}

void BattleMain::spare_enemy(int enemy_id) {
    if (enemy_id < 0 || enemy_id >= enemies.size()) return;
    emit_signal("enemy_spared", enemy_id);
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[enemy_id]);
    if(!enemy) return;

    enemies[enemy_id] = nullptr;
    enemy_names[enemy_id] = Variant();
    box->set_enemies(enemies);

    enemy->connect("on_defeat_end", Callable(this, "_on_spare_finished").bind(enemy), CONNECT_ONE_SHOT);
    if(enemy->has_method("on_defeat")) { // C++ 이랑 GDscript 모두 호환되도록
        enemy->call("on_defeat", false);
    }else enemy->on_defeat(false);
}

void BattleMain::_on_spare_finished(Enemy* enemy) {
    GPUParticles2D* spare = enemy->get_spare();
    if(!spare) {
        ERR_PRINT("Enemy 노드에 필요한 spare 노드가 없습니다");
        return;
    }

    // Dictionary enemy_rewards = enemy->get_rewards();
    // int exp_reward = enemy_rewards.get("exp", 0);
    // rewards["exp"] = int(rewards["exp"]) - exp_reward;

    Ref<Tween> tween = create_tween()->set_parallel();
    tween->tween_property(enemy, "modulate:a", 0, 1);
    tween->tween_callback(Callable(spare, "set_emitting").bind(true));
    tween->chain()->tween_callback(Callable(enemy, "queue_free"));
    spare->connect("finished", Callable(this, "_on_action").bind("spare_enemy"), CONNECT_ONE_SHOT);
}

void BattleMain::end_encounter() {
    music_player->stop();
    
    // 보상 처리
    global->set_player_gold(global->get_player_gold() + int(rewards["gold"]));
    global->set_player_exp(global->get_player_exp() + int(rewards["exp"]));
    
    String win_text = tr(box->get("wintext"));
    win_text = vformat(win_text,
        rewards["exp"],
        rewards["gold"]
    );
    
    // 레벨업 처리
    if(global->check_level_up()) {
        win_text += String(" \n")+tr("UT_LOVE_INCREASED");
        if (lvlup_sound) {
            lvlup_sound->play();
        }
    }
    
    box->change_state(BattleBox::State_BlitteringCasual);
    Blitter* blitter_text = box->get_blitter_text();
    if (blitter_text) {
        PackedStringArray texts = { win_text };
        blitter_text->type_text(texts);
        blitter_text->connect("finished_all_texts", Callable(this, "_on_action").bind("end_battle"), CONNECT_ONE_SHOT);
    }
}

int BattleMain::enemy_size() {
    int size = 0;
    for(int i=0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy) size++;
    }
    return size;
}

void BattleMain::_modify_stats(int id, Dictionary stats) {
    box->enemies_hp[id] = stats.get("hp", box->enemies_hp[id]);
    enemies_max_hp[id] = stats.get("max_hp", enemies_max_hp[id]);
    enemies_def[id] = stats.get("def", enemies_def[id]);
}

void BattleMain::set_encounter(const Ref<Encounter>& p_encounter) {
    encounter = p_encounter;
}

Ref<Encounter> BattleMain::get_encounter() const {
    return encounter;
}

bool BattleMain::is_kr() {
    return kr;
}

void BattleMain::_on_get_turn() {
    for(int i=0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if(is_first_turn && !enemy->get_is_first_turn()) continue;
        if(enemy->has_method("_on_get_turn")) { // C++ 이랑 GDscript 모두 호환되도록
            enemy->call("_on_get_turn");
        } else {
            enemy->_on_get_turn();
        }
    }
    is_first_turn = false;
}

void BattleMain::_on_end_turn() {
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if (enemy) {
            if (enemy->has_method("_on_end_turn")) { // C++ 이랑 GDscript 모두 호환되도록
                enemy->call("_on_end_turn");
            } else {
                enemy->_on_end_turn();
            }
        }
    }
}

void BattleMain::add_enemy(Ref<PackedScene> enemy_scene) {
    if(enemy_size() >= 3) {
        ERR_PRINT("적은 최대 3마리까지 추가할수 있습니다");
        return;
    }

    if(!enemy_scene.is_valid()) {
        ERR_PRINT("유효하지 않은 적 씬입니다");
        return;
    }

    Node* enemy_instance = enemy_scene->instantiate();
    if(!enemy_instance || !enemy_instance->is_class("Enemy")) {
        ERR_PRINT("씬이 Enemy 클래스로 캐스팅되지 않습니다");
        enemy_instance->queue_free();
        return;
    }

    Enemy* enemy = Object::cast_to<Enemy>(enemy_instance);
    if(!enemy) {
        ERR_PRINT("Enemy로 캐스팅할 수 없습니다");
        enemy_instance->queue_free();
        return;
    }

    int current_count = enemy_size();
    Vector2 position = Vector2(0, 0);
    if(current_count == 1) {
        Enemy* existing_enemy = nullptr;
        for(int i = 0; i < enemies.size(); i++) {
            existing_enemy = Object::cast_to<Enemy>(enemies[i]);
            if(existing_enemy) {
                existing_enemy->set_position(Vector2(-100, 0));
                break;
            }
        }
        position = Vector2(100, 0);
    } else if(current_count == 2) {
        position = Vector2(200, 0);
        
        int placed = 0;
        for(int i = 0; i < enemies.size(); i++) {
            Enemy* existing_enemy = Object::cast_to<Enemy>(enemies[i]);
            if(existing_enemy) {
                if(placed == 0) {
                    existing_enemy->set_position(Vector2(-200, 0));
                } else if(placed == 1) {
                    existing_enemy->set_position(Vector2(0, 0));
                }
                placed++;
            }
        }
    }

    enemy->set_position(position);
    enemies_node->add_child(enemy_instance);
    
    int enemy_id = -1;
    for(int i = 0; i < enemies.size(); i++) {
        if(enemies[i] == Variant()) {
            enemies[i] = enemy;
            enemy_names[i] = enemy_scene;
            enemy_id = i;
            break;
        }
    }
    
    if(enemy_id == -1) {
        enemies.append(enemy);
        enemy_names.append(enemy_scene);
        enemy_id = enemies.size() - 1;
        
        Dictionary stats = enemy->get_stats();
        enemies_max_hp.append(stats.get("max_hp", 1));
        enemies_def.append(stats.get("def", 0));
        
        box->enemies_hp.append(stats.get("hp", stats.get("max_hp", 1)));
    } else {
        Dictionary stats = enemy->get_stats();
        enemies_max_hp[enemy_id] = stats.get("max_hp", 1);
        enemies_def[enemy_id] = stats.get("def", 0);
        
        box->enemies_hp[enemy_id] = stats.get("hp", stats.get("max_hp", 1));
    }

    enemy->set_id(enemy_id);
    enemy->set_solo(enemy_size() == 1);
    
    if(enemy->get_kr()) {
        kr = true;
        hud->set_kr();
    }

    enemy->connect("changed_act", Callable(box, "_set_targets"));

    Dictionary rewards_data = enemy->get_rewards();
    rewards["gold"] = int(rewards["gold"]) + int(rewards_data.get("gold", 0));
    rewards["exp"] = int(rewards["exp"]) + int(rewards_data.get("exp", 0));

    for(int i = 0; i < enemies.size(); i++) {
        Enemy* e = Object::cast_to<Enemy>(enemies[i]);
        if(e) {
            e->set_solo(enemy_size() == 1);
        }
    }

    box->set_enemies(enemies);
}

void BattleMain::toggle_transparent() {
    if(global->isMobile()) {
        ERR_PRINT("투명 모드는 모바일에서 지원되지 않습니다");
        return;
    }
    SceneContainer* scene = global->get_scene_container();
    if(transparent) 
        scene->_on_settings_setting_changed("border", global->get_settings()["border"]);
    else scene->_on_settings_setting_changed("border", false);

    if(global->get_fullscreen()) global->toggle_fullscreen();
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1, false);
    timer->connect("timeout", Callable(this, "_on_transparent"), CONNECT_ONE_SHOT);
}

void BattleMain::_on_transparent() {
    DisplayServer* display = DisplayServer::get_singleton();
    SceneContainer* scene = global->get_scene_container();
    soul_battle->show_hpText(transparent);
    if(transparent) {
        display->window_set_flag(DisplayServer::WINDOW_FLAG_TRANSPARENT, false);
        get_tree()->get_root()->set_transparent_background(false);
        scene->get_node_internal("SettingsContainer")->call("show");
        scene->get_node_internal("SubViewportContainer/MainViewport/CanvasLayer")->call("show");
        attacks->show();
        buttons->show();
        hud->show();
        bg->show();
        enemies_node->show();
        box->box_show();
        global->enable_input("toggle_fullscreen");
    }else {
        display->window_set_flag(DisplayServer::WINDOW_FLAG_TRANSPARENT, true);
        get_tree()->get_root()->set_transparent_background(true);
        scene->get_node_internal("SettingsContainer")->call("hide");
        scene->get_node_internal("SubViewportContainer/MainViewport/CanvasLayer")->call("hide");
        attacks->hide();
        buttons->hide();
        hud->hide();
        bg->hide();
        enemies_node->hide();
        box->box_hide();
        box->change_size(Vector2(640, 330));
        global->toggle_fullscreen();
        global->disable_input("toggle_fullscreen");
    }
    transparent = !transparent;
}

void BattleMain::reset_pos_camera() {
    camera->set_position(Vector2(320, 240));
}

void BattleMain::_encounter_script_add(bool on) {
    if(!on) {
        get_tree()->connect("process_frame", Callable(this, "_encounter_script_add").bind(true), CONNECT_ONE_SHOT);
        return;
    }
    script_node->request_ready();
    add_child(script_node);
}

void BattleMain::set_property(Variant value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

void BattleMain::set_turn_number(int value) {
    turn_number = value;
}

int BattleMain::get_turn_number() {
    return turn_number;
}

BattleButtons* BattleMain::get_buttons() {
    return buttons;
}