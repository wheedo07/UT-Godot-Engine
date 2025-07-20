#include "enemy.h"
#include "env.h"
#include "engine/Battle/battle_system.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/shader_material.hpp>

Enemy::Enemy() {
    kr = false;
    id = 0;
    solo = true;
    enemy_name = "Enemy";
    is_first_turn = false;
    dodging = false;
    new_states_override = true;
    one_by_one_overrdie = false;
    current_state = 0;
    
    // 기본 통계 설정
    stats["def"] = 10;
    stats["hp"] = 100;
    stats["max_hp"] = 100;
    
    // 기본 보상 설정
    rewards["exp"] = 10;
    rewards["gold"] = 10;

    Ref<EnemyState> state = memnew(EnemyState);
    enemy_states.append(state);

    e_head = nullptr;
    e_body = nullptr;
    hurt_sound = nullptr;
    sprites = nullptr;
}

Enemy::~Enemy() {}

void Enemy::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished_dialogue"));
    ADD_SIGNAL(MethodInfo("changed_state"));
    ADD_SIGNAL(MethodInfo("spared", PropertyInfo(Variant::INT, "id_number")));
    ADD_SIGNAL(MethodInfo("on_fight_end"));
    ADD_SIGNAL(MethodInfo("on_act_end"));
    ADD_SIGNAL(MethodInfo("on_item_end"));
    ADD_SIGNAL(MethodInfo("on_mercy_end"));

    // 스크립트 메소드
    BIND_VIRTUAL_METHOD(Enemy, ready, hash_djb2("Enemy_ready"));
    BIND_VIRTUAL_METHOD(Enemy, _on_get_turn, hash_djb2("Enemy__on_get_turn"));
    BIND_VIRTUAL_METHOD(Enemy, _on_end_turn, hash_djb2("Enemy__on_end_turn"));
    BIND_VIRTUAL_METHOD(Enemy, on_fight_used, hash_djb2("Enemy_on_fight_used"));
    BIND_VIRTUAL_METHOD(Enemy, on_act_used, hash_djb2("Enemy_on_act_used"));
    BIND_VIRTUAL_METHOD(Enemy, on_item_used, hash_djb2("Enemy_on_item_used"));
    BIND_VIRTUAL_METHOD(Enemy, on_mercy_used, hash_djb2("Enemy_on_mercy_used"));
    BIND_VIRTUAL_METHOD(Enemy, on_win, hash_djb2("Enemy_on_win"));

    ClassDB::bind_method(D_METHOD("set_kr", "p_kr"), &Enemy::set_kr);
    ClassDB::bind_method(D_METHOD("get_kr"), &Enemy::get_kr);
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &Enemy::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &Enemy::get_id);
    ClassDB::bind_method(D_METHOD("set_solo", "p_solo"), &Enemy::set_solo);
    ClassDB::bind_method(D_METHOD("get_solo"), &Enemy::get_solo);
    ClassDB::bind_method(D_METHOD("set_enemy_name", "p_enemy_name"), &Enemy::set_enemy_name);
    ClassDB::bind_method(D_METHOD("get_enemy_name"), &Enemy::get_enemy_name);
    ClassDB::bind_method(D_METHOD("set_is_first_turn", "p_is_first_turn"), &Enemy::set_is_first_turn);
    ClassDB::bind_method(D_METHOD("get_is_first_turn"), &Enemy::get_is_first_turn);
    ClassDB::bind_method(D_METHOD("set_dodging", "p_dodging"), &Enemy::set_dodging);
    ClassDB::bind_method(D_METHOD("get_dodging"), &Enemy::get_dodging);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "enemy_name"), "set_enemy_name", "get_enemy_name");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "kr"), "set_kr", "get_kr");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "isFirstTurn"), "set_is_first_turn", "get_is_first_turn");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dodging"), "set_dodging", "get_dodging");

    ClassDB::bind_method(D_METHOD("set_stats", "p_stats"), &Enemy::set_stats);
    ClassDB::bind_method(D_METHOD("get_stats"), &Enemy::get_stats);
    ClassDB::bind_method(D_METHOD("set_rewards", "p_rewards"), &Enemy::set_rewards);
    ClassDB::bind_method(D_METHOD("get_rewards"), &Enemy::get_rewards);
    ClassDB::bind_method(D_METHOD("set_new_states_override", "p_new_states_override"), &Enemy::set_new_states_override);
    ClassDB::bind_method(D_METHOD("get_new_states_override"), &Enemy::get_new_states_override);
    ClassDB::bind_method(D_METHOD("set_one_by_one_overrdie", "p_one_by_one_overrdie"), &Enemy::set_one_by_one_overrdie);
    ClassDB::bind_method(D_METHOD("get_one_by_one_overrdie"), &Enemy::get_one_by_one_overrdie);

    ADD_GROUP("data && status", "");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "stats"), "set_stats", "get_stats");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "rewards"), "set_rewards", "get_rewards");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "new_states_override"), "set_new_states_override", "get_new_states_override");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "one_by_one_overrdie"), "set_one_by_one_overrdie", "get_one_by_one_overrdie");

    ClassDB::bind_method(D_METHOD("set_dialogues", "p_dialogues"), &Enemy::set_dialogues);
    ClassDB::bind_method(D_METHOD("get_dialogues"), &Enemy::get_dialogues);
    ClassDB::bind_method(D_METHOD("set_flavour_text", "p_flavour_text"), &Enemy::set_flavour_text);
    ClassDB::bind_method(D_METHOD("get_flavour_text"), &Enemy::get_flavour_text);
    
    ADD_GROUP("textbox && dialogues", "");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dialogues", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":Dialogues"
    ), "set_dialogues", "get_dialogues");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "flavour_text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT)),
    "set_flavour_text", "get_flavour_text");

    ClassDB::bind_method(D_METHOD("set_current_state", "p_current_state"), &Enemy::set_current_state);
    ClassDB::bind_method(D_METHOD("get_current_state"), &Enemy::get_current_state);
    ClassDB::bind_method(D_METHOD("set_enemy_states", "p_enemy_states"), &Enemy::set_enemy_states);
    ClassDB::bind_method(D_METHOD("get_enemy_states"), &Enemy::get_enemy_states);
    
    ADD_GROUP("Act", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_state"), "set_current_state", "get_current_state");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "enemy_states", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":EnemyState"
    ), "set_enemy_states", "get_enemy_states");

    ClassDB::bind_method(D_METHOD("set_hurt_sound_path", "p_path"), &Enemy::set_hurt_sound_path);
    ClassDB::bind_method(D_METHOD("get_hurt_sound_path"), &Enemy::get_hurt_sound_path);
    ClassDB::bind_method(D_METHOD("set_dust_sound_path", "p_path"), &Enemy::set_dust_sound_path);
    ClassDB::bind_method(D_METHOD("get_dust_sound_path"), &Enemy::get_dust_sound_path);
    ClassDB::bind_method(D_METHOD("set_dust_path", "p_path"), &Enemy::set_dust_path);
    ClassDB::bind_method(D_METHOD("get_dust_path"), &Enemy::get_dust_path);
    ClassDB::bind_method(D_METHOD("set_e_head_path", "p_path"), &Enemy::set_e_head_path);
    ClassDB::bind_method(D_METHOD("get_e_head_path"), &Enemy::get_e_head_path);
    ClassDB::bind_method(D_METHOD("set_e_body_path", "p_path"), &Enemy::set_e_body_path);
    ClassDB::bind_method(D_METHOD("get_e_body_path"), &Enemy::get_e_body_path);
    ClassDB::bind_method(D_METHOD("set_sprites_path", "p_path"), &Enemy::set_sprites_path);
    ClassDB::bind_method(D_METHOD("get_sprites_path"), &Enemy::get_sprites_path);
    ClassDB::bind_method(D_METHOD("set_dialogue_path", "p_path"), &Enemy::set_dialogue_path);
    ClassDB::bind_method(D_METHOD("get_dialogue_path"), &Enemy::get_dialogue_path);
    
    ADD_GROUP("NodePath", "");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "hurt_sound_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AudioStreamPlayer"), "set_hurt_sound_path", "get_hurt_sound_path");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "dust_sound_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AudioStreamPlayer"), "set_dust_sound_path", "get_dust_sound_path");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "dust_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "GPUParticles2D"), "set_dust_path", "get_dust_path");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "e_head_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AnimatedSprite2D"), "set_e_head_path", "get_e_head_path");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "e_body_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AnimatedSprite2D"), "set_e_body_path", "get_e_body_path");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "sprites_path"), "set_sprites_path", "get_sprites_path");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "dialogue_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "DialogueControl"), "set_dialogue_path", "get_dialogue_path");

    ClassDB::bind_method(D_METHOD("handle_typing", "text_index", "dialogue_ref", "duration", "skip"), &Enemy::handle_typing);
    
    ClassDB::bind_method(D_METHOD("get_sprites"), &Enemy::get_sprites);
    ClassDB::bind_method(D_METHOD("get_act_info", "act_choice"), &Enemy::get_act_info);
    ClassDB::bind_method(D_METHOD("_get_act", "state", "option"), &Enemy::_get_act);
    ClassDB::bind_method(D_METHOD("change_state", "new_state"), &Enemy::change_state);
    ClassDB::bind_method(D_METHOD("play_dialogue", "index", "duration", "skip"), &Enemy::play_dialogue, DEFVAL(0), DEFVAL(true));
    ClassDB::bind_method(D_METHOD("play_set_dialogue", "dialogue_ref", "duration", "skip"), &Enemy::play_set_dialogue, DEFVAL(0), DEFVAL(true));
    ClassDB::bind_method(D_METHOD("dodge"), &Enemy::dodge);
    ClassDB::bind_method(D_METHOD("_hurt", "amount"), &Enemy::_hurt);
    ClassDB::bind_method(D_METHOD("on_death"), &Enemy::on_death);
    ClassDB::bind_method(D_METHOD("_on_spared", "id_number"), &Enemy::_on_spared);
    ClassDB::bind_method(D_METHOD("_on_finished_all_texts_dialogue", "head", "body"), &Enemy::_on_finished_all_texts_dialogue);
    ClassDB::bind_method(D_METHOD("change_stats", "value"), &Enemy::change_stats);

    ClassDB::bind_method(D_METHOD("set_property", "value"), &Enemy::set_property);
    ClassDB::bind_method(D_METHOD("get_main"), &Enemy::get_main);
    ClassDB::bind_method(D_METHOD("get_attacks"), &Enemy::get_attacks);
    ClassDB::bind_method(D_METHOD("get_box"), &Enemy::get_box);
    ClassDB::bind_method(D_METHOD("get_soul"), &Enemy::get_soul);
    ClassDB::bind_method(D_METHOD("get_dialogue"), &Enemy::get_dialogue);
    ClassDB::bind_method(D_METHOD("get_bg"), &Enemy::get_bg);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "main", PROPERTY_HINT_NONE, "BattleMain", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_main");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attacks", PROPERTY_HINT_NONE, "AttackManager", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_attacks");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "box", PROPERTY_HINT_NONE, "BattleBox", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_box");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "soul", PROPERTY_HINT_NONE, "SoulBattle", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_soul");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "dialogue", PROPERTY_HINT_NONE, "DialogueControl", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_dialogue");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bg", PROPERTY_HINT_NONE, "BattleBackground", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_bg");
}

void Enemy::_ready() {
    if(isEditor) return;
    hurt_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal(hurt_sound_path));
    dust_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal(dust_sound_path));
    dust = Object::cast_to<GPUParticles2D>(get_node_internal(dust_path));
    
    main = Object::cast_to<BattleMain>(global->get_scene_container()->get_current_scene());
    attacks = main->attacks;
    non_mask = main->attacks_parent;
    camera = main->camera;
    box = main->box;
    soul = main->soul_battle;
    bg = main->bg;
    
    dialogue = Object::cast_to<DialogueControl>(get_node_internal(dialogue_path));
    sprites = get_node_internal(sprites_path);
    if(!e_head_path.is_empty()) e_head = Object::cast_to<AnimatedSprite2D>(get_node_internal(e_head_path));
    if(!e_body_path.is_empty()) e_body = Object::cast_to<AnimatedSprite2D>(get_node_internal(e_body_path));
    connect("spared", Callable(this, "_on_spared"));
    
    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    }else {
        ready();
    }
}

void Enemy::change_stats(Dictionary value) {
    main->change_stats(id, value);
}

Ref<ActInfo> Enemy::get_act_info(int act_choice) {
    if (new_states_override) {
        if (one_by_one_overrdie) {
            _get_act(0, act_choice);
            for (int i = 0; i < current_state + 1; i++) {
                _get_act(i, act_choice);
            }
        } else {
            _get_act(current_state, act_choice);
        }
    } else {
        _get_act(current_state, act_choice);
    }
    
    Ref<ActInfo> info = _info;
    _info.unref();
    return info;
}

void Enemy::_get_act(int state, int option) {
    if (state >= 0 && state < enemy_states.size()) {
        Ref<EnemyState> enemy_state = enemy_states[state];
        if (enemy_state.is_valid()) {
            Array acts = enemy_state->get_acts();
            if (option >= 0 && option < acts.size()) {
                _info = acts[option];
            }
        }
    }
}

void Enemy::change_state(int new_state) {
    current_state = new_state;
    emit_signal("changed_state");
}

void Enemy::play_dialogue(int index, float duration, bool skip) {
    if(!is_node_ready() || !dialogue) return;
    global->set_battle_text_box(true);
    
    int head_frame = -1;
    int body_frame = -1;

    if(e_head) head_frame = e_head->get_frame();
    if(e_body) body_frame = e_body->get_frame();
    
    if(index >= 0 && index < dialogues.size()) {
        Ref<Dialogues> dialogue_ref = dialogues[index];
        if (dialogue_ref.is_valid()) {
            EnemySpeech* text_typer = Object::cast_to<EnemySpeech>(dialogue->get_node_internal("TextContainer/Text"));
            Callable call = Callable(this, "handle_typing").bind(dialogue_ref, duration, skip);
            if(text_typer->is_connected("started_typing", call)) text_typer->disconnect("started_typing", call);
            text_typer->connect("started_typing", call);
            dialogue->DialogueText(dialogue_ref);
        }else ERR_PRINT("index의 맞는 Dialogues가 유효 하지 않습니다");
    }
    dialogue->set_key(skip);
    dialogue->connect("finished_all_texts_dialogue", Callable(this, "_on_finished_all_texts_dialogue").bind(head_frame, body_frame), CONNECT_ONE_SHOT);
}

void Enemy::play_set_dialogue(Ref<Dialogues> dialogue_ref, float duration, bool skip) {
    if(!is_node_ready() || !dialogue) return;
    global->set_battle_text_box(true);
    
    int head_frame = -1;
    int body_frame = -1;

    if(e_head) head_frame = e_head->get_frame();
    if(e_body) body_frame = e_body->get_frame();
    
    if (dialogue_ref.is_valid()) {
        EnemySpeech* text_typer = Object::cast_to<EnemySpeech>(dialogue->get_node_internal("TextContainer/Text"));
        Callable call = Callable(this, "handle_typing").bind(dialogue_ref, duration, skip);
        if(text_typer->is_connected("started_typing", call)) text_typer->disconnect("started_typing", call);
        text_typer->connect("started_typing", call);
        dialogue->DialogueText(dialogue_ref);
    }else ERR_PRINT("Dialogues가 유효 하지 않습니다");
    dialogue->set_key(skip);
    dialogue->connect("finished_all_texts_dialogue", Callable(this, "_on_finished_all_texts_dialogue").bind(head_frame, body_frame), CONNECT_ONE_SHOT);
}

void Enemy::_on_finished_all_texts_dialogue(int head, int body) {
    if(head != -1) e_head->set_frame(head);
    if(body != -1) e_body->set_frame(body);
    emit_signal("finished_dialogue");
}

void Enemy::handle_typing(int text_index, Ref<Dialogues> dialogue_ref, float duration, bool skip) {
    if(e_head) {
        PackedInt32Array expressions = dialogue_ref->get_dialogues_single(Dialogues::DIALOGUE_EXPRESSION_HEAD);
        if (text_index < expressions.size() && expressions[text_index] != -1) {
            e_head->set_frame(expressions[text_index]);
        }
    }
    if(e_body) {
        PackedInt32Array body_frames = dialogue_ref->get_dialogues_single(Dialogues::DIALOGUE_EXPRESSION_BODY);
        if (text_index < body_frames.size() && body_frames[text_index] != -1) {
            e_body->set_frame(body_frames[text_index]);
        }
    }

    if(duration != 0) dialogue->on_text_click_played(skip, duration);
}

void Enemy::dodge() {
    if (!sprites) return;
    
    Ref<Tween> tw = create_tween()->set_ease(Tween::EASE_OUT)->set_trans(Tween::TRANS_QUAD);
    RandomNumberGenerator* rng = memnew(RandomNumberGenerator);
    rng->randomize();
    int rand_sign = (rng->randi_range(0, 1) * 2) - 1;
    
    tw->tween_property(sprites, "position:x", rand_sign * 120.0f, 0.4)->as_relative();
    tw->tween_property(sprites, "position:x", 0, 0.4)->as_relative()->set_delay(0.8);
}

void Enemy::_hurt(int amount) {
    if (!sprites) return;

    Vector2 vec = sprites->call("get_position");
    
    float defaultpos = vec.x;
    
    Ref<Tween> hurtsoundtween = create_tween();
    if(hurt_sound) hurtsoundtween->tween_callback(Callable(hurt_sound, "play"))->set_delay(0.4);
    
    Ref<Tween> tw = create_tween();
    tw->set_loops(6);
    
    tw->tween_property(sprites, "position:x", -4.0f, 0.02)->as_relative();
    tw->tween_interval(0.02);
    tw->tween_property(sprites, "position:x", 4.0f, 0.02)->as_relative();
    tw->tween_interval(0.02);
    
    Ref<Tween> chain_tw = tw->chain();
    chain_tw->tween_property(sprites, "position:x", defaultpos, 0.03);
}

void Enemy::ready() {
    // 파생 클래스에서 구현
}

void Enemy::on_fight_used() {
    // 파생 클래스에서 구현
    emit_signal("on_fight_end");
}

void Enemy::on_act_used(int option) {
    // 파생 클래스에서 구현
    emit_signal("on_act_end");
}

void Enemy::on_item_used(int option) {
    // 파생 클래스에서 구현
    emit_signal("on_item_end");
}

void Enemy::on_mercy_used() {
    // 파생 클래스에서 구현
    emit_signal("on_mercy_end");
}

void Enemy::_on_get_turn() {
    // 파생 클래스에서 구현
}

void Enemy::_on_end_turn() {
    // 파생 클래스에서 구현
}

void Enemy::on_win() {
    // 파생 클래스에서 구현
}

void Enemy::on_death() {
    if (!sprites || !dust || !dust_sound) {
        return;
    }
    
    Ref<Tween> tween = get_tree()->create_tween();
    tween->set_parallel(true);
    
    tween->tween_property(sprites, "modulate:a", 0, dust->get_lifetime() / 4.0f);
    
    Ref<ShaderMaterial> material = dust->get_process_material();
    tween->tween_property(material.ptr(), "shader_parameter/progress", 1.0f, dust->get_lifetime())->from(0);
    
    dust->restart();
    dust->set_emitting(true);
    dust_sound->play();
}

void Enemy::_on_spared(int id_number) {
    GPUParticles2D* spare = Object::cast_to<GPUParticles2D>(get_node_internal("Spare"));
    if (spare) {
        spare->restart();
        spare->set_emitting(true);
    }
    
    AudioStreamPlayer* sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("Sounds/Dust"));
    if (sound) {
        sound->play();
    }
}

void Enemy::on_defeat(bool death) {}

void Enemy::set_kr(bool p_kr) {
    kr = p_kr;
}

bool Enemy::get_kr() const {
    return kr;
}

void Enemy::set_id(int p_id) {
    id = p_id;
}

int Enemy::get_id() const {
    return id;
}

void Enemy::set_solo(bool p_solo) {
    solo = p_solo;
}

bool Enemy::get_solo() const {
    return solo;
}

void Enemy::set_enemy_name(const String& p_enemy_name) {
    enemy_name = p_enemy_name;
}

String Enemy::get_enemy_name() const {
    return enemy_name;
}

void Enemy::set_is_first_turn(bool p_is_first_turn) {
    is_first_turn = p_is_first_turn;
}

bool Enemy::get_is_first_turn() const {
    return is_first_turn;
}

void Enemy::set_dodging(bool p_dodging) {
    dodging = p_dodging;
}

bool Enemy::get_dodging() const {
    return dodging;
}

void Enemy::set_stats(const Dictionary& p_stats) {
    stats = p_stats;
}

Dictionary Enemy::get_stats() const {
    return stats;
}

void Enemy::set_dialogues(const Array& p_dialogues) {
    dialogues = p_dialogues;
}

Array Enemy::get_dialogues() const {
    return dialogues;
}

void Enemy::set_flavour_text(const PackedStringArray& p_flavour_text) {
    flavour_text = p_flavour_text;
}

PackedStringArray Enemy::get_flavour_text() const {
    return flavour_text;
}

void Enemy::set_rewards(const Dictionary& p_rewards) {
    rewards = p_rewards;
}

Dictionary Enemy::get_rewards() const {
    return rewards;
}

void Enemy::set_new_states_override(bool p_new_states_override) {
    new_states_override = p_new_states_override;
}

bool Enemy::get_new_states_override() const {
    return new_states_override;
}

void Enemy::set_one_by_one_overrdie(bool p_one_by_one_overrdie) {
    one_by_one_overrdie = p_one_by_one_overrdie;
}

bool Enemy::get_one_by_one_overrdie() const {
    return one_by_one_overrdie;
}

void Enemy::set_enemy_states(const Array& p_enemy_states) {
    enemy_states = p_enemy_states;
}

Array Enemy::get_enemy_states() const {
    return enemy_states;
}

void Enemy::set_current_state(int p_current_state) {
    current_state = p_current_state;
}

int Enemy::get_current_state() const {
    return current_state;
}

Node* Enemy::get_sprites() const {
    return sprites;
}

void Enemy::set_hurt_sound_path(const NodePath& p_path) {
    hurt_sound_path = p_path;
}

NodePath Enemy::get_hurt_sound_path() const {
    return hurt_sound_path;
}

void Enemy::set_dust_sound_path(const NodePath& p_path) {
    dust_sound_path = p_path;
}

NodePath Enemy::get_dust_sound_path() const {
    return dust_sound_path;
}

void Enemy::set_dust_path(const NodePath& p_path) {
    dust_path = p_path;
}

NodePath Enemy::get_dust_path() const {
    return dust_path;
}

void Enemy::set_e_head_path(const NodePath& p_path) {
    e_head_path = p_path;
}

NodePath Enemy::get_e_head_path() const {
    return e_head_path;
}

void Enemy::set_e_body_path(const NodePath& p_path) {
    e_body_path = p_path;
}

NodePath Enemy::get_e_body_path() const {
    return e_body_path;
}

void Enemy::set_sprites_path(const NodePath& p_path) {
    sprites_path = p_path;
}

NodePath Enemy::get_sprites_path() const {
    return sprites_path;
}

void Enemy::set_dialogue_path(const NodePath& p_path) {
    dialogue_path = p_path;
}

NodePath Enemy::get_dialogue_path() const {
    return dialogue_path;
}

void Enemy::set_property(Object* value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

BattleMain* Enemy::get_main() {
    return main;
}

AttackManager* Enemy::get_attacks() {
    return attacks;
}

BattleBox* Enemy::get_box() {
    return box;
}

SoulBattle* Enemy::get_soul() {
    return soul;
}

DialogueControl* Enemy::get_dialogue() {
    return dialogue;
}

TextureRect* Enemy::get_bg() {
    return bg;
}