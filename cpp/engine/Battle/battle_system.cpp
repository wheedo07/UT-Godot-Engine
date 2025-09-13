#include "battle_system.h"
#include "env.h"
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
    transparent = false;
    rewards["gold"] = 0;
    rewards["exp"] = 0;
}

BattleMain::~BattleMain() {}

void BattleMain::_bind_methods() {
    ADD_SIGNAL(MethodInfo("damage_info_finished"));
    ADD_SIGNAL(MethodInfo("end_turn"));
    ADD_SIGNAL(MethodInfo("item_used", PropertyInfo(Variant::INT, "id")));
    
    ClassDB::bind_method(D_METHOD("no_enemies_handler"), &BattleMain::no_enemies_handler);
    ClassDB::bind_method(D_METHOD("on_get_turn"), &BattleMain::on_get_turn);
    ClassDB::bind_method(D_METHOD("on_end_turn"), &BattleMain::on_end_turn);

    ClassDB::bind_method(D_METHOD("_on_player_turn_start"), &BattleMain::_on_player_turn_start);
    ClassDB::bind_method(D_METHOD("_on_enemy_turn_start"), &BattleMain::_on_enemy_turn_start);
    ClassDB::bind_method(D_METHOD("_on_damage_info_finished"), &BattleMain::_on_damage_info_finished);
    ClassDB::bind_method(D_METHOD("_initialize"), &BattleMain::initialize);
    
    ClassDB::bind_method(D_METHOD("_fight", "target"), &BattleMain::_fight);
    ClassDB::bind_method(D_METHOD("hit", "damage", "target", "crit"), &BattleMain::hit, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("miss", "target"), &BattleMain::miss);
    ClassDB::bind_method(D_METHOD("_act", "target", "option"), &BattleMain::_act);
    ClassDB::bind_method(D_METHOD("_mercy", "choice"), &BattleMain::_mercy);
    ClassDB::bind_method(D_METHOD("_item", "item_id"), &BattleMain::_item);
    
    ClassDB::bind_method(D_METHOD("kill_enemy", "enemy_id"), &BattleMain::kill_enemy, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("spare_enemy", "enemy_id"), &BattleMain::spare_enemy, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("end_encounter"), &BattleMain::end_encounter);
    ClassDB::bind_method(D_METHOD("check_end_encounter"), &BattleMain::check_end_encounter);
    ClassDB::bind_method(D_METHOD("check_enemy_solo"), &BattleMain::check_enemy_solo);
    ClassDB::bind_method(D_METHOD("pure_int_to_short_representation", "input"), &BattleMain::pure_int_to_short_representation);
    
    ClassDB::bind_method(D_METHOD("_on_slash_finished", "damage", "target", "crit"), &BattleMain::_on_slash_finished);
    ClassDB::bind_method(D_METHOD("_on_damage_info_completed", "target"), &BattleMain::_on_damage_info_completed);
    ClassDB::bind_method(D_METHOD("_on_fight_used_completed", "target"), &BattleMain::_on_fight_used_completed);
    ClassDB::bind_method(D_METHOD("_on_blitter_finished_all_texts"), &BattleMain::_on_blitter_finished_all_texts);
    ClassDB::bind_method(D_METHOD("_on_timer_timeout", "action"), &BattleMain::_on_timer_timeout);
    ClassDB::bind_method(D_METHOD("_on_camera_blind_completed"), &BattleMain::_on_camera_blind_completed);
    ClassDB::bind_method(D_METHOD("_finish_encounter"), &BattleMain::_finish_encounter);
    ClassDB::bind_method(D_METHOD("_on_transparent"), &BattleMain::_on_transparent);
    ClassDB::bind_method(D_METHOD("_on_end"), &BattleMain::_on_end, DEFVAL(false), DEFVAL(-1));
    
    ClassDB::bind_method(D_METHOD("set_encounter", "encounter"), &BattleMain::set_encounter);
    ClassDB::bind_method(D_METHOD("get_encounter"), &BattleMain::get_encounter);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "encounter", PROPERTY_HINT_RESOURCE_TYPE, "Encounter"), "set_encounter", "get_encounter");

    ClassDB::bind_method(D_METHOD("set_property", "value"), &BattleMain::set_property);
    ClassDB::bind_method(D_METHOD("get_turn_number"), &BattleMain::get_turn_number);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "turn_number", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_turn_number");
}

void BattleMain::_ready() {
    if(isEditor) return;
    camera = Object::cast_to<CameraRemoteController>(get_node_internal("Camera"));
    buttons = Object::cast_to<BattleButtons>(get_node_internal("Buttons"));
    box = Object::cast_to<BattleBox>(get_node_internal("BattleBox"));
    enemies_node = Object::cast_to<Node2D>(get_node_internal("Enemies"));
    bg = Object::cast_to<TextureRect>(get_node_internal("Background/Texture"));
    colorRect = Object::cast_to<ColorRect>(get_node_internal("ColorRect"));
    soul_battle = Object::cast_to<SoulBattle>(get_node_internal("SoulLayer/Soul_Battle"));
    attacks = Object::cast_to<AttackManager>(get_node_internal("Attacks/BoxClipper"));
    attacks_parent = Object::cast_to<Node>(get_node_internal("Attacks"));
    hud = Object::cast_to<BattleHUD>(get_node_internal("HUD"));
    music_player = global->get_Music();
    lvlup_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("lvlup"));

    ResourceLoader* loader = ResourceLoader::get_singleton();
    attack_scene = loader->load("res://Battle/AttackMeter/meter.tscn");
    slash_scene = loader->load("res://Battle/Slashes/slashes.tscn");
    damage_info_scene = loader->load("res://Battle/AttackMeter/damage.tscn");
    if(!encounter.is_valid() && global->battle_encounter && global->battle_encounter->is_class("Encounter")) {
        encounter = global->battle_encounter;
    }

    global->set_battle_start(true);
    if(encounter.is_valid()) {
        bg->set_texture(encounter->get_background());
        bg->set_position(encounter->get_offset());
        Array enemy_scenes = encounter->get_enemies().duplicate();

        Ref<AudioStream> music = encounter->get_music();
        if(music_player && music.is_valid()) {
            music_player->set_stream(music);
            music_player->play();
        }else {
            music_player->stop();
        }
        
        if (enemy_scenes.size() == 0) {
            soul_battle->hide();
            get_tree()->connect("process_frame", Callable(this, "no_enemies_handler"), CONNECT_ONE_SHOT);
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

        bool is_first_turn = false;
        
        for (int i = 0; i < enemies_node->get_child_count(); i++) {
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
            enemy->connect("changed_state", Callable(box, "set_targets"));
            
            Dictionary stats = enemy->get_stats();
            enemies_max_hp.append(stats.get("max_hp", 1));
            enemies_def.append(stats.get("def", 0));
            
            Blitter* blitter_text = box->get_blitter_text();
            PackedStringArray flavor_texts = enemy->get_flavour_text();
            if(i == 0) {
                if(flavor_texts.size() > 0) {
                    blitter_text->set_flavour_texts(flavor_texts);
                }else {
                    PackedStringArray default_text = { "* " + enemy->get_enemy_name() + String::utf8("가 나타났다") };
                    blitter_text->set_flavour_texts(default_text);
                }
            }
            
            Dictionary rwrds = enemy->get_rewards();
            rewards["gold"] = int(rewards["gold"]) + int(rwrds.get("gold", 0));
            rewards["exp"] = int(rewards["exp"]) + int(rwrds.get("exp", 0));
            
            connect("item_used", Callable(enemy, "on_item_used"));
            enemy->connect("spared", Callable(this, "spare_enemy"));

            // C++ 이랑 GDscript 모두 호환되도록
            connect("end_turn", Callable(this, "on_get_turn"));
            
            if (enemy->get_is_first_turn()) {
                call_deferred("on_get_turn");
                is_first_turn = true;
            }
        }
        
        if(is_first_turn) {
            box->set_z_index(-1);
            buttons->disable();
            soul_battle->enable();
        } else {
            box->blitter_flavour();
            buttons->enable();
        }
        
        Node* tl = box->get_node_internal("BoxContainer/TL");
        Node* br = box->get_node_internal("BoxContainer/BR");
        tl->call("set_remote_node", tl->call("get_path_to", attacks->top_left));
        br->call("set_remote_node", tl->call("get_path_to", attacks->bottom_right));
        
        initialize();
    }
}

void BattleMain::no_enemies_handler() {
    box->change_state(BattleBox::BattleState::State_BlitteringCasual);
    Blitter* blitter_text = box->get_blitter_text();
    PackedStringArray texts;
    texts.push_back(String("* ")+tr("UT_NOBODY_CAME"));
    blitter_text->type_text(texts);
    blitter_text->connect("finished_all_texts", Callable(this, "_on_blitter_finished_all_texts"), CONNECT_ONE_SHOT);
}

void BattleMain::_on_blitter_finished_all_texts() {
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(1.0);
    timer->connect("timeout", Callable(this, "_on_timer_timeout").bind("no_enemies_exit"), CONNECT_ONE_SHOT);
}

void BattleMain::_on_timer_timeout(const String& action) {
    if (action == "no_enemies_exit") {
        soul_battle->queue_free();
        scene_changer->load_cached_overworld_scene();
    }
}

void BattleMain::initialize() {
    CameraFx* camera_node = global->get_scene_container()->get_camera();
    Color modulate = camera_node->blinder->get_modulate();
    modulate.a = 1;
    camera_node->blinder->set_modulate(modulate);
    camera_node->blind(0.5, 0);
}

void BattleMain::_on_player_turn_start() {
    if(transparent) toggle_transparent();
    soul_battle->menu_enable();
    buttons->enable();
    box->set_z_index(0);
    box->blitter_flavour();
}

void BattleMain::_on_enemy_turn_start() {
    soul_battle->enable();
    box->set_z_index(-1);
    turn_number += 1;
}

void BattleMain::_on_damage_info_finished() {
    // (필요한 경우)
}

void BattleMain::_on_death_player() {
    for(int i=0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy) {
            if(enemy->has_method("on_win")) { // C++ 이랑 GDscript 모두 호환되도록
                enemy->call("on_win");
            } else enemy->on_win();
        }
    }
}

void BattleMain::_fight(int target) {
    if (!attack_scene.is_valid() || !box) return;

    Node* clone = attack_scene->instantiate();
    if (clone) {
        clone->set("target", target);
        clone->connect("damagetarget", Callable(this, "hit"), CONNECT_ONE_SHOT);
        clone->connect("missed", Callable(this, "miss"), CONNECT_ONE_SHOT);
        clone->set("targetdef", enemies_def[target]);
        box->add_child(clone);
    }
}

void BattleMain::hit(int damage, int target, bool crit) {
    if (!slash_scene.is_valid() || !box || target < 0 || target >= enemies.size()) return;
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
    if (!enemy) return;
    
    Slash* slashes = Object::cast_to<Slash>(slash_scene->instantiate());
    if (slashes) {
        if (enemy->get_dodging()) {
            slashes->connect("started", Callable(enemy, "dodge"), CONNECT_ONE_SHOT);
        }
        slashes->connect("finished", Callable(this, "_on_slash_finished").bind(damage, target, crit), CONNECT_ONE_SHOT);

        slashes->set_crit(crit);
        box->add_child(slashes, true);
        slashes->set_global_position(enemy->get_sprites()->call("get_global_position"));
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
                box->enemies_hp[target] = (float)box->enemies_hp[target] - damage;
            }
            
            box->add_child(clone);
            clone->connect("finished", Callable(this, "_on_damage_info_completed").bind(target), CONNECT_ONE_SHOT);
        }
    }
}

void BattleMain::_on_damage_info_completed(int target) {
    emit_signal("damage_info_finished");
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
    if (!enemy) return;
    enemy->connect("on_fight_end", Callable(this, "_on_fight_used_completed").bind(target), CONNECT_ONE_SHOT);
    if(enemy->has_method("on_fight_used")) { // C++ 이랑 GDscript 모두 호환되도록
        enemy->call("on_fight_used");
    }else enemy->on_fight_used();
}

void BattleMain::_on_fight_used_completed(int target) {
    if (box) box->disable();
    
    if (float(box->enemies_hp[target]) < 0) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
        if (enemy) {
            enemy->on_death();
            kill_enemy(target);
        }
    } else {
        emit_signal("end_turn");
    }
}

void BattleMain::miss(int target) {
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
        clone->connect("finished", Callable(box, "disable"), CONNECT_ONE_SHOT);
        clone->connect("finished", Callable(this, "emit_signal").bind("end_turn"), CONNECT_ONE_SHOT);
    }
}

void BattleMain::_act(int target, int option) {
    if (target < 0 || target >= enemies.size()) return;
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[target]);
    if (enemy) {
        enemy->connect("on_act_end", Callable(this, "emit_signal").bind("end_turn"), CONNECT_ONE_SHOT);
        if(enemy->has_method("on_act_used")) { // C++ 이랑 GDscript 모두 호환되도록
            enemy->call("on_act_used", option);
        }else enemy->on_act_used(option);
    }
}

void BattleMain::_mercy(int choice) {
    switch (choice) {
        case -1:
            emit_signal("end_turn");
            break;
        case 0:
            completed_size = enemy_size();
            for (int i = 0; i < enemies.size(); i++) {
                Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
                if (enemy) {
                    Ref<EnemyState> state = enemy->get_enemy_states()[enemy->get_current_state()];
                    enemy->connect("on_mercy_end", Callable(this, "_on_end").bind(state->get_sparable(), i), CONNECT_ONE_SHOT);
                    if(enemy->has_method("on_mercy_used")) { // C++ 이랑 GDscript 모두 호환되도록
                        enemy->call("on_mercy_used");
                    }else enemy->on_mercy_used();
                }
            }
            break;
        case 1: {
            CameraFx* camera_node = global->get_scene_container()->get_camera();
            camera_node->blind(0, 1);
            camera_node->connect("finished_tween", Callable(this, "_on_camera_blind_completed"), CONNECT_ONE_SHOT);
            break;
        }
    }
}

void BattleMain::_on_camera_blind_completed() {
    global->set_temp_atk(0);
    global->set_temp_def(0);
    scene_changer->load_cached_overworld_scene();
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
    completed_size = enemy_size();
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy) {
            enemy->connect("on_item_end", Callable(this, "_on_end"), CONNECT_ONE_SHOT);
            if(enemy->has_method("on_item_used")) { // C++ 이랑 GDscript 모두 호환되도록
                enemy->call("on_item_used", item_id);
            }else enemy->on_item_used(item_id);
        }
    }
}

void BattleMain::kill_enemy(int enemy_id) {
    if (enemy_id < 0 || enemy_id >= enemies.size()) return;
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[enemy_id]);
    if (enemy) {
        enemy->on_defeat(true);
        enemies[enemy_id] = nullptr;
        enemy_names[enemy_id] = Variant();
        disconnect("end_turn", Callable(enemy, "_on_get_turn"));

        Ref<SceneTreeTimer> timer = get_tree()->create_timer(1.8);
        timer->connect("timeout", Callable(enemy, "queue_free"), CONNECT_ONE_SHOT);

        int kills = global->get_player_kills();
        kills++;
        global->set_player_kills(kills);
        
        if (box) {
            box->set_enemies(enemies);
            box->disable();
        }
        
        if (check_end_encounter()) {
            end_encounter();
        } else {
            bool solo = check_enemy_solo();
            for (int i = 0; i < enemies.size(); i++) {
                Enemy* e = Object::cast_to<Enemy>(enemies[i]);
                if (e) {
                    e->set_solo(solo);
                }
            }
            emit_signal("end_turn");
        }
    }
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
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[enemy_id]);
    if (enemy) {
        Dictionary enemy_rewards = enemy->get_rewards();
        int exp_reward = enemy_rewards.get("exp", 0);
        rewards["exp"] = int(rewards["exp"]) - exp_reward;
        
        enemy->on_defeat(false);
        Ref<Tween> tween = create_tween()->set_parallel();
        tween->tween_property(enemy, "modulate:a", 0, 1);
        Vector2 current_pos = enemy->get_position();
        tween->tween_property(enemy, "position", Vector2(current_pos.x, current_pos.y - 20), 1.0);
        tween->chain()->tween_callback(Callable(enemy, "queue_free"));
        disconnect("end_turn", Callable(enemy, "_on_get_turn"));

        enemies[enemy_id] = nullptr;
        enemy_names[enemy_id] = Variant();
        
        if(box) box->set_enemies(enemies);
        
        if(check_end_encounter()) {
            Ref<SceneTreeTimer> timer = get_tree()->create_timer(1.2);
            timer->connect("timeout", Callable(this, "end_encounter"), CONNECT_ONE_SHOT);
        } else {
            bool solo = check_enemy_solo();
            for (int i = 0; i < enemies.size(); i++) {
                Enemy* e = Object::cast_to<Enemy>(enemies[i]);
                if (e) {
                    e->set_solo(solo);
                }
            }
        }
    }
}

String BattleMain::pure_int_to_short_representation(int input) {
    Dictionary magnitudes;
    magnitudes[0.0000000001] = "n";
    magnitudes[0.0000001] = "u";
    magnitudes[0.001] = "m";
    magnitudes[1] = "";
    magnitudes[1000] = "k";
    magnitudes[1000000] = "M";
    magnitudes[1000000000] = "B";
    
    double highest = 1.0;
    Array keys = magnitudes.keys();
    for (int i = 0; i < keys.size(); i++) {
        double key = keys[i];
        if (input >= key) {
            highest = key;
        }
    }
    
    double result = input / highest;
    String value = String::num(result);
    if (value.length() > 4) {
        value = value.substr(0, 4);
    }
    return String::num_real(round(value.to_float())) + String(magnitudes[highest]);
}

void BattleMain::end_encounter() {
    music_player->stop();
    
    // 보상 처리
    global->set_player_gold(global->get_player_gold() + int(rewards["gold"]));
    global->set_player_exp(global->get_player_exp() + int(rewards["exp"]));
    
    String win_text = box->get("wintext");
    win_text = vformat(win_text,
        rewards["exp"],
        rewards["gold"]
    );
    
    // 레벨업 처리
    if(global->check_level_up()) {
        win_text += String(" \n* ")+tr("UT_LOVE_INCREASED");
        if (lvlup_sound) {
            lvlup_sound->play();
        }
    }
    
    box->change_state(BattleBox::State_BlitteringCasual);
    Blitter* blitter_text = box->get_blitter_text();
    if (blitter_text) {
        PackedStringArray texts;
        texts.push_back(win_text);
        blitter_text->type_text(texts);
        blitter_text->connect("finished_all_texts", Callable(this, "_finish_encounter"), CONNECT_ONE_SHOT);
    }
}

void BattleMain::_finish_encounter() {
    global->set_temp_atk(0);
    global->set_temp_def(0);
    
    soul_battle->queue_free();
    scene_changer->load_cached_overworld_scene();
}

int BattleMain::enemy_size() {
    int size = 0;
    for(int i=0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy) size++;
    }
    return size;
}

void BattleMain::modify_stats(int id, Dictionary stats) {
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

void BattleMain::on_get_turn() {
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy) {
            if (enemy->has_method("_on_get_turn")) { // C++ 이랑 GDscript 모두 호환되도록
                enemy->call("_on_get_turn");
            } else {
                enemy->_on_get_turn();
            }
        }
    }
}

void BattleMain::on_end_turn() {
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

void BattleMain::set_property(Variant value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

int BattleMain::get_turn_number() {
    return turn_number;
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
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1);
    timer->connect("timeout", Callable(this, "_on_transparent"), CONNECT_ONE_SHOT);
}

void BattleMain::_on_transparent() {
    DisplayServer* display = DisplayServer::get_singleton();
    SceneContainer* scene = global->get_scene_container();
    soul_battle->toggle_hpText();
    if(transparent) {
        display->window_set_flag(DisplayServer::WINDOW_FLAG_TRANSPARENT, false);
        get_tree()->get_root()->set_transparent_background(false);
        scene->get_node_internal("SettingsContainer")->call("show");
        scene->get_node_internal("SubViewportContainer/MainViewport/CanvasLayer")->call("show");
        attacks->show();
        buttons->show();
        bg->show();
        enemies_node->show();
        box->box_show();
        global->enable_input("ui_close");
        global->enable_input("toggle_fullscreen");
    }else {
        display->window_set_flag(DisplayServer::WINDOW_FLAG_TRANSPARENT, true);
        get_tree()->get_root()->set_transparent_background(true);
        scene->get_node_internal("SettingsContainer")->call("hide");
        scene->get_node_internal("SubViewportContainer/MainViewport/CanvasLayer")->call("hide");
        attacks->hide();
        buttons->hide();
        bg->hide();
        enemies_node->hide();
        box->box_hide();
        box->change_size(Vector2(640, 330));
        global->toggle_fullscreen();
        global->disable_input("ui_close");
        global->disable_input("toggle_fullscreen");
    }
    transparent = !transparent;
}