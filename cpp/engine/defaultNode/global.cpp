#include "global.h"
#include "engine/Battle/battle_system.h"
#include<sstream>
#include<fstream>
#include<filesystem>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/json.hpp>
#include<godot_cpp/classes/collision_polygon2d.hpp>
#include<godot_cpp/classes/collision_shape2d.hpp>
#include<godot_cpp/classes/ray_cast2d.hpp>
#include<godot_cpp/classes/tile_map.hpp>
#include<godot_cpp/classes/file_access.hpp>
#include<godot_cpp/classes/dir_access.hpp>
#include<godot_cpp/classes/window.hpp>
#include<godot_cpp/classes/input_map.hpp>
#include<godot_cpp/core/math.hpp>
using namespace godot;
namespace fs = std::filesystem;
unsigned char bom[] = {0xEF, 0xBB, 0xBF};

#define HASH_KEY "undertale_engine_by_wheedo07"
Global::Global() {
    Music = nullptr;
    speedup_sound = nullptr;
    Info = nullptr;
    KrTimer = nullptr;
    scene_container = nullptr;
    savepath = "user://file0";
    settingpath = "user://file9";

    // 기본 상태 변수
    first = true;
    fullscreen = false;
    debugmode = false;
    collision_visible = false;

    battle_start = false;
    player_in_menu = false;
    player_set_menu = false;
    player_can_move = true;
    player_move = true;
    player_text_box = false;
    battle_text_box = false;
    
    // 인벤토리 관련
    equipment["weapon"] = 0;
    equipment["armor"] = 1;
    cells = Array();
    items = Array();
    boxitems = Array(); 
    
    // 설정
    settings["shake"] = true;
    settings["vfx"] = true;
    settings["border"] = false;
    settings["SFX"] = 100;
    settings["Music"] = 100;
    settings["Master"] = 100;
    
    // 플레이어 스탯
    player_name = "ERROR";
    player_gold = 0;
    player_lv = 1;
    player_exp = 0;
    player_hp = 20;
    player_max_hp = 20;
    player_defense = 0;
    player_attack = 10;
    player_kr = 0;
    player_kills = 0;
    
    // KR 관련
    krtime = 0.5;
    
    // 임시 변수들
    temp_atk = 0;
    temp_def = 0;
    player_position = Vector2();

    overworld_temp_data["global_position"] = Vector2();
    just_died = true;
    overworld_data["room"] = "";
    overworld_data["room_name"] = "";
    overworld_data["room_pos"] = Vector2(0, 0);
    playtime = 0;
    cache_playtime = 0;
    quit_time = 0;
    speed_time = 0;
    start = false;
    isSetting = false;
    is_Mobile = false;
    battle_encounter = nullptr;

    Item* it0 = memnew(Item);
    it0->set_item_type(Item::WEAPON);
    it0->set_item_name(String::utf8("막대기"));
    it0->set_use_message({
        String::utf8("* 당신은 막대기를 장착했다")
    });
    it0->set_item_information({
        String::utf8("* 막대기 - 1\n* 겉보기와 달리 위협적이지 않다")
    });
    it0->set_throw_message({
        String::utf8("* 당신은 막대기를 던졌다"),
        String::utf8("* 하지만 아무 일도 일어나지 않았다")
    });
    item_list.append(it0);

    Item* it1 = memnew(Item);
    it1->set_item_name(String::utf8("반창고"));
    it1->set_use_message({
        String::utf8("* 당신은 반창고를 사용했다")
    });
    it1->set_item_information({
        String::utf8("* 반창고 - 10 hp 회복\n* 이미 여러 번 붙였던 것이다")
    });
    it1->set_throw_message({
        String::utf8("* 당신은 반창고를 버렸다")
    });
    it1->set_heal_amount(10);
    item_list.append(it1);

    Item* it2 = memnew(Item);
    it2->set_item_name(String::utf8("파이"));
    it2->set_use_message({
        String::utf8("* 당신은 파이를 사용했다")
    });
    it2->set_item_information({
        String::utf8("* 버터스카치 시나몬 파이 한 조각이다")
    });
    it2->set_throw_message({
        String::utf8("* 당신은 파이를 버렸다")
    });
    it2->set_heal_amount(999);
    item_list.append(it2);
}

Global::~Global() {}

void Global::_ready() {
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_HIDDEN);

    os = OS::get_singleton();
    display = DisplayServer::get_singleton();
    marshalls = Marshalls::get_singleton();
    Music = Object::cast_to<AudioStreamPlayer>(get_node_internal("MusicGlobal"));
    speedup_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("speed_up"));
    Info = Object::cast_to<RichTextLabel>(get_node_internal("Info"));
    KrTimer = Object::cast_to<Timer>(get_node_internal("KrTimer"));
    init_paths();
}

void Global::init_paths() {
    String osName = os->get_name();
    is_Mobile = osName == "Android";

    if(osName == "Web") {
        savepath = "user://file0";
        settingpath = "user://file9";
    }
    if(is_Mobile) {
        call_deferred("toggle_fullscreen");
        savepath = "user://file0";
        settingpath = "user://file9";
    }

    if(savepath.find("$home") != -1) {
        if(osName == "Windows") {
            const char* homepath = std::getenv("HOMEPATH");
            if(homepath == nullptr) {
                const char* userprofile = std::getenv("USERPROFILE");
                if(userprofile != nullptr) savepath = savepath.replace("$home", userprofile);
                else savepath = "user://file0";
            }else savepath = savepath.replace("$home", homepath);
        }
    }

    if(savepath.find("user://") == -1) {
        if(osName == "Windows") {
            const char* drive = std::getenv("HOMEDRIVE");
            if(drive != nullptr) savepath = String(drive) + savepath;
            else {
                savepath = "user://file0";
                return;
            }
        }

        String dir = savepath.replace("file0", "");
        settingpath = dir + "file9";
        if(fs::exists(dir.utf8().get_data())) return;

        if(!fs::create_directories(dir.utf8().get_data()))
            ERR_PRINT("디렉토리 생성 실패!");
    }
}

PackedStringArray Global::item_use_text(int item_id) {
    Ref<Item> item = item_list[item_id];
    PackedStringArray use_text = item->get_use_message();
    
    if(item->get_heal_amount()) {
        heal(item->get_heal_amount());
        if (player_hp == player_max_hp) {
            use_text.push_back(tr("UT_HP_FULL"));
        }else {
            use_text.push_back(vformat(tr("UT_HP_HEAL"), item->get_heal_amount()));
        }
    }
    if(item->get_defense_amount()) {
        temp_def += item->get_defense_amount();
        use_text.push_back(vformat(tr("UT_DEF_UP"), item->get_defense_amount()));
    }
    if(item->get_attack_amount()) {
        temp_atk += item->get_attack_amount();
        use_text.push_back(vformat(tr("UT_ATK_UP"), item->get_attack_amount()));
    }
    return use_text;
}

PackedStringArray Global::equip_item(int item_id) {
    Ref<Item> item = item_list[item_id];
    PackedStringArray equip_text = item->get_use_message();
    
    switch(item->get_item_type()) {
        case Item::WEAPON:
            items.push_back(equipment["weapon"]);
            equipment["weapon"] = item_id;
            break;
        case Item::ARMOR:
            items.push_back(equipment["armor"]);
            equipment["armor"] = item_id;
            break;
        default:
            break;
    }
    
    return equip_text;
}

void Global::heal(int amt) {
    if(player_hp + amt > player_max_hp) {
        amt = player_max_hp - player_hp;
    }
    player_hp += amt;
}

void Global::_input(const Ref<InputEvent>& event) {
    if(event->is_action_pressed("toggle_fullscreen") && !is_Mobile) toggle_fullscreen();

    if(event->is_action_pressed("debug") && (os->has_feature("debug_mode") || os->is_debug_build())) {
        debugmode = !debugmode;
        if(!debugmode && collision_visible) toggle_collision_shape_visibility();
    }
}

void Global::_unhandled_input(const Ref<InputEvent>& event) {
    if(debugmode) {
        if(event->is_action_pressed("refresh_scene") && os->is_debug_build() && !isSetting) {
            print_line(tr("UT_WARN_NODE_LOSS"));
            player_hp = player_max_hp;
            player_kr = 0;
            player_can_move = true;
            player_in_menu = false;
            Engine::get_singleton()->set_time_scale(1);
            Node* current_scene = get_scene_container()->get_current_scene();
            if(get_tree()->get_current_scene()->is_class("SceneContainer")) {
                if(current_scene->is_class("BattleMain")) {
                    battle_encounter = current_scene->get("encounter");
                }
                scene_container->reload_current_scene();
                return;
            }
            get_tree()->reload_current_scene();
        }else if(event->is_action_pressed("stop_game") && os->is_debug_build()) {
            if(paused_time != 0) return;
            paused_time = 0.1;
            SceneTree* tree = get_tree();
            tree->set_pause(!tree->is_paused());
            if(tree->is_paused()) print_line(tr("UT_GAME_PAUSED"));
            else print_line(tr("UT_GAME_RESUMED"));
            get_viewport()->set_input_as_handled();
        }else if(event->is_action_pressed("force_save") && (os->is_debug_build() || os->has_feature("debug_op"))) {
            String path = get_scene_container()->get_current_scene()->get_scene_file_path();
            if(path.find("res://Game") == -1) {
                print_line(tr("UT_CANT_HERE"));
                return;
            }
            print_line(tr("UT_SAVING_GAME"));
            save();
            get_viewport()->set_input_as_handled();
        }else if(event->is_action_pressed("debug_turn") && (os->is_debug_build() || os->has_feature("debug_op"))) {
            if(!battle_start) return;
            Node* current_scene = get_scene_container()->get_current_scene();
            if(current_scene->is_class("BattleMain")) {
                BattleMain* battle = Object::cast_to<BattleMain>(current_scene);
                if(battle->player_turn) {
                    battle->buttons->disable();
                    battle->buttons->reset();
                    battle->box->_disable();
                    battle->emit_signal("end_turn");
                }else {
                    battle->attacks->force_end_attacks();
                }
            }
            get_viewport()->set_input_as_handled();
        }else if(event->is_action_pressed("toggle_collision") && os->is_debug_build()) {
            toggle_collision_shape_visibility();
            get_viewport()->set_input_as_handled();
        }
    }
}

void Global::_process(double delta) {
    Input* input = Input::get_singleton();
    Engine* engine = Engine::get_singleton();

    if(input->is_action_pressed("ui_quit")) {
        quit_time += delta;
    }else if(input->is_action_just_released("speed_up") && debugmode && !isSetting && 
            (os->is_debug_build() || os->has_feature("debug_op"))) { 
        engine->set_time_scale(Math::min(engine->get_time_scale() + 0.1, 5.0));
        speed_time = 0.6 * engine->get_time_scale();
        speedup_sound->play();
    }else if(input->is_action_just_released("speed_down") && debugmode && !isSetting && 
            (os->is_debug_build() || os->has_feature("debug_op"))) {
        engine->set_time_scale(Math::max(engine->get_time_scale() - 0.1, 1.0));
        speed_time = 0.6 * engine->get_time_scale();
        speedup_sound->play();
    }else if(input->is_action_just_released("speed_reset") && debugmode && !isSetting && 
            (os->is_debug_build() || os->has_feature("debug_op"))) {
        engine->set_time_scale(1);
        speed_time = 0.6 * engine->get_time_scale();
        speedup_sound->play();
    }else {
        if(tw_label.is_valid() && tw_label->is_valid()) tw_label->kill();
        Info->set_modulate(Color(1,1,1,1));
        quit_time = 0;
    }

    if(quit_time != 0) {
        if(!tw_label.is_valid() || !tw_label->is_valid()) {
            tw_label = create_tween()->set_loops();
            tw_label->tween_property(Info, "modulate:a", 0.35, 0.6)->set_trans(Tween::TRANS_SINE)->set_ease(Tween::EASE_IN_OUT);
            tw_label->tween_property(Info, "modulate:a", 1, 0.6)->set_trans(Tween::TRANS_SINE)->set_ease(Tween::EASE_IN_OUT);
        }

        Info->set_text(String::utf8("[color=red]")+tr("UT_EXITING"));
        if(quit_time >= 2) {
            save_settings();
            get_tree()->quit();
        }
    }else if(speed_time != 0 && debugmode) {
        Info->set_text(vformat(String::utf8("[color=yellow]")+tr("UT_FRAMES")+String("[/color]: %sx")
            + (os->is_debug_build() ? String("\n[R] ")+ tr("UT_RELOAD_SCENE") : String("")),
            engine->get_time_scale()));
        speed_time -= delta;
        if(speed_time <= 0) speed_time = 0;
    }else if(debugmode && !isSetting) {
        Info->set_text(vformat(String("[rainbow]")+tr("UT_DEBUG_MODE")+String("[/rainbow]\nFPS: %s")
        + (os->is_debug_build() ? String("\n[R] ")+ tr("UT_RELOAD_SCENE") : String("")),
            engine->get_frames_per_second()));
    } else {
        Info->set_text("");
    }

    if(paused_time != 0) {
        paused_time -= delta;
        if(paused_time <= 0) paused_time = 0;
    }

    if(start) playtime += delta;

    // KR THING:
    KrTimer->set_wait_time(player_kr > 30 ? krtime / 3.0 : krtime);
}

void Global::_notification(int what) {
    if(what == NOTIFICATION_WM_CLOSE_REQUEST || what == NOTIFICATION_WM_GO_BACK_REQUEST) {
        save_settings();
        get_tree()->quit();
    }
}

void Global::toggle_fullscreen() {
    if (!fullscreen) {
        display->window_set_mode(DisplayServer::WINDOW_MODE_FULLSCREEN);
    } else {
        display->window_set_mode(DisplayServer::WINDOW_MODE_WINDOWED);
    }
    fullscreen = !fullscreen;
    emit_signal("fullscreen_toggled", fullscreen);
}

void Global::save_game(bool is_sys) {
    if (is_sys) {
        if(savepath.find("user://") != -1) {
            Ref<FileAccess> file = FileAccess::open(savepath, FileAccess::READ_WRITE);
            String data = xor_decrypt(file->get_as_text(), HASH_KEY);
            Dictionary savedata = JSON::parse_string(data);
            savedata["flags"] = flags;
            String newdata = xor_encrypt(JSON::stringify(savedata), HASH_KEY);
            file->store_string(newdata);
            file->close();
        }else {
            std::ifstream file_read(savepath.utf8().get_data());
            file_read.seekg(3);
            std::stringstream buffer;
            buffer << file_read.rdbuf();
            String data = xor_decrypt(String::utf8(buffer.str().data()), HASH_KEY);
            Dictionary savedata = JSON::parse_string(data);
            file_read.close();
            savedata["flags"] = flags;

            std::ofstream file_write(savepath.utf8().get_data());
            file_write.write((char*)bom, sizeof(bom));
            file_write << xor_encrypt(JSON::stringify(savedata), HASH_KEY).utf8().get_data() << std::endl;
            file_write.close();
        }
        return;
    }
    first = false;
    
    Dictionary stats;
    stats["gold"] = player_gold;
    stats["exp"] = player_exp;
    stats["name"] = player_name;
    stats["lv"] = player_lv;
    stats["hp"] = player_hp;
    stats["max_hp"] = player_max_hp;
    stats["def"] = player_defense;
    stats["atk"] = player_attack;
    stats["kills"] = player_kills;
    
    Dictionary inv;
    inv["equipment"] = equipment;
    inv["items"] = items;
    inv["cells"] = cells;
    inv["boxinv"] = boxitems;
    
    Dictionary savedata;
    flags.merge(flags_tmp, true);
    flags_tmp = Dictionary();
    savedata["stats"] = stats;
    savedata["inv"] = inv;
    savedata["overworld"] = overworld_data;
    savedata["flags"] = flags;
    savedata["playtime"] = playtime;
    savedata["first"] = first;

    cache_playtime = playtime;

    if(savepath.find("user://") != -1) {
        Ref<FileAccess> file = FileAccess::open(savepath, FileAccess::WRITE);
        String newdata = xor_encrypt(JSON::stringify(savedata), HASH_KEY);
        file->store_string(newdata);
        file->close();
    }else {
        String newdata = xor_encrypt(JSON::stringify(savedata), HASH_KEY);
        std::ofstream file_write(savepath.utf8().get_data());
        file_write.write((char*)bom, sizeof(bom));
        file_write << newdata.utf8().get_data() << std::endl;
        file_write.close();
    }
    save_settings();
    emit_signal("saved");
}

String Global::xor_encrypt(String data, String key) {
    PackedByteArray data_bytes = data.to_utf8_buffer();
    PackedByteArray key_bytes = key.to_utf8_buffer();
    
    for(int i = 0; i < data_bytes.size(); i++) {
        data_bytes[i] ^= key_bytes[i % key_bytes.size()];
    }
    
    return marshalls->raw_to_base64(data_bytes);
}

String Global::xor_decrypt(String data, String key) {
    PackedByteArray encrypted_bytes = marshalls->base64_to_raw(data.strip_edges());
    PackedByteArray key_bytes = key.to_utf8_buffer();
    
    for(int i = 0; i < encrypted_bytes.size(); i++) {
        encrypted_bytes[i] ^= key_bytes[i % key_bytes.size()];
    }
    
    return String::utf8((char*)encrypted_bytes.ptr(), encrypted_bytes.size());
}

void Global::true_resetgame() {
    resetgame();
    g_flags = Dictionary();
    save_settings();
}

void Global::resetgame() {
    playtime = 0;
    cache_playtime = 0;
    
    overworld_data.clear();
    
    flags = backup["flags"];
    first = true;

    player_name = backup["player_name"];
    equipment = backup["equipment"];

    Dictionary player_stats = backup["player_stats"];
    player_lv = player_stats["player_lv"];
    player_exp = player_stats["player_exp"];
    player_gold = player_stats["player_gold"];
    player_hp = player_stats["player_hp"];
    player_max_hp = player_stats["player_max_hp"];
    player_defense = player_stats["player_defense"];
    player_attack = player_stats["player_attack"];
    player_kills = player_stats["player_kills"];

    items = backup["items"];

    boxitems = backup["boxitems"];
    cells = backup["cells"];

    battle_start = false;
    player_in_menu = false;
    player_set_menu = false;
    player_can_move = true;
    player_move = true;
    player_text_box = false;
    battle_text_box = false;
    start = false;
    
    if(savepath.find("user://") != -1) {
        String dir = savepath.replace("file0", "");
        Ref<DirAccess> dirAcs = DirAccess::open(dir);
        dirAcs->remove("file0");
    }else fs::remove(savepath.utf8().get_data());
}

void Global::toggle_collision_shape_visibility() {
    SceneTree* tree = get_tree();
    tree->set_debug_collisions_hint(!tree->is_debugging_collisions_hint());
    collision_visible = tree->is_debugging_collisions_hint();
    call_deferred("_update_collision_visibility");
}

void Global::_update_collision_visibility() {
    TypedArray<Node> node_stack;
    node_stack.push_back(get_tree()->get_root());

    while(!node_stack.is_empty()) {
        Node* node = Object::cast_to<Node>(node_stack.pop_back());
        if(!ObjectDB::get_instance(node->get_instance_id())) continue;
        
        if (node->is_class("TileMap")) {
            TileMap* tilemap = Object::cast_to<TileMap>(node);
            tilemap->set_collision_visibility_mode(TileMap::VISIBILITY_MODE_FORCE_HIDE);
            tilemap->set_collision_visibility_mode(TileMap::VISIBILITY_MODE_DEFAULT);
        }
        if (node->is_class("CollisionShape2D") || 
            node->is_class("CollisionPolygon2D") || 
            node->is_class("RayCast2D")) {
            node->call("queue_redraw");
        }
        
        TypedArray<Node> children = node->get_children();
        for (int i = 0; i < children.size(); i++) {
            node_stack.push_back(Object::cast_to<Node>(children[i]));
        }
    }
}

void Global::_on_kr_tick() {
    if(player_kr > 0) {
        player_kr -= 1;
        player_hp -= 1;
    }
}

bool Global::check_level_up() {
    int lv = player_lv;
    bool lvup = false;
    
    if (player_exp >= 10) lv = 2;
    if (player_exp >= 30) lv = 3;
    if (player_exp >= 70) lv = 4;
    if (player_exp >= 120) lv = 5;
    if (player_exp >= 200) lv = 6;
    if (player_exp >= 300) lv = 7;
    if (player_exp >= 500) lv = 8;
    if (player_exp >= 800) lv = 9;
    if (player_exp >= 1200) lv = 10;
    if (player_exp >= 1700) lv = 11;
    if (player_exp >= 2500) lv = 12;
    if (player_exp >= 3500) lv = 13;
    if (player_exp >= 5000) lv = 14;
    if (player_exp >= 7000) lv = 15;
    if (player_exp >= 10000) lv = 16;
    if (player_exp >= 15000) lv = 17;
    if (player_exp >= 25000) lv = 18;
    if (player_exp >= 50000) lv = 19;
    if (player_exp >= 99999) lv = 20;
    
    if (lv != player_lv) {
        lvup = true;
        player_max_hp = 16 + lv * 4;
        player_attack = 8 + lv * 2;
        player_defense = 9 + Math::ceil(lv / 4.0);
        if (lv == 20) {
            player_max_hp = 99;
            player_attack = 99;
            player_defense = 99;
        }
    } else {
        lvup = false;
    }
    
    player_lv = lv;
    return lvup;
}

void Global::save_settings() {
    Dictionary setting_data;
    
    if(settingpath.find("user://") != -1) {
        Ref<FileAccess> file;
        if (FileAccess::file_exists(settingpath)) {
            file = FileAccess::open(settingpath, FileAccess::READ);
            if (!file->eof_reached()) {
                String content = xor_decrypt(file->get_as_text(), HASH_KEY);
                if(!content.is_empty()) {
                    Variant parsed = JSON::parse_string(content);
                    if (parsed.get_type() == Variant::DICTIONARY) {
                        setting_data = parsed;
                    }
                }
            }
            file->close();
        }
        
        file = FileAccess::open(settingpath, FileAccess::WRITE);
        if(file.is_valid()) {
            setting_data["settings"] = settings;
            setting_data["g_flags"] = g_flags;
            String newdata = xor_encrypt(JSON::stringify(setting_data), HASH_KEY);
            file->store_string(newdata);
            file->close();
        }
    } else {
        std::ifstream file_read(settingpath.utf8().get_data());
        if(file_read.is_open()) {
            file_read.seekg(3);
            std::stringstream buffer;
            buffer << file_read.rdbuf();
            file_read.close();
           
            String content = xor_decrypt(String::utf8(buffer.str().data()), HASH_KEY);
            if (!content.is_empty()) {
                Variant parsed = JSON::parse_string(content);
                if (parsed.get_type() == Variant::DICTIONARY) {
                    setting_data = parsed;
                }
            }
        }
        
        std::ofstream file_write(settingpath.utf8().get_data());
        if(file_write.is_open()) {
            file_write.write((char*)bom, sizeof(bom));
            setting_data["settings"] = settings;
            setting_data["g_flags"] = g_flags;
            String newdata = xor_encrypt(JSON::stringify(setting_data), HASH_KEY);
            file_write << newdata.utf8().get_data() << std::endl;
            file_write.close();
        }
    }
}

void Global::load_game() {
    if(backup.is_empty()) {
        backup["player_name"] = player_name;
        
        backup["flags"] = flags.duplicate(true);
        backup["equipment"] = equipment.duplicate(true);
        
        Dictionary player_stats;
        player_stats["player_lv"] = player_lv;
        player_stats["player_exp"] = player_exp;
        player_stats["player_gold"] = player_gold;
        player_stats["player_hp"] = player_hp;
        player_stats["player_max_hp"] = player_max_hp;
        player_stats["player_defense"] = player_defense;
        player_stats["player_attack"] = player_attack;
        player_stats["player_kills"] = player_kills;
        backup["player_stats"] = player_stats;
        
        backup["items"] = items.duplicate(true);
        backup["boxitems"] = boxitems.duplicate(true);
        backup["cells"] = cells.duplicate();
        
        refresh_audio_busses();
    }

    Dictionary settings_data;
    if(settingpath.find("user://") != -1) {
        if(FileAccess::file_exists(settingpath)) {
            Ref<FileAccess> settings_file = FileAccess::open(settingpath, FileAccess::READ);
            if(settings_file.is_valid()) {
                String data = xor_decrypt(settings_file->get_as_text(), HASH_KEY);
                Variant parsed = JSON::parse_string(data);
                if(parsed.get_type() == Variant::DICTIONARY) {
                    settings_data = parsed;
                }
                settings_file->close();
            }
        }
    } else {
        std::ifstream settings_file(settingpath.utf8().get_data());
        if(settings_file.is_open()) {
            settings_file.seekg(3); // BOM 건너뛰기
            std::stringstream buffer;
            buffer << settings_file.rdbuf();
            settings_file.close();

            String data = xor_decrypt(String::utf8(buffer.str().data()), HASH_KEY);
            Variant parsed = JSON::parse_string(data);
            if(parsed.get_type() == Variant::DICTIONARY) {
                settings_data = parsed;
            }
        }
    }

    // SETTINGS
    settings.merge(settings_data.get("settings", settings), true);
    g_flags.merge(settings_data.get("g_flags", g_flags), true);

    Dictionary savedata;
    if(savepath.find("user://") != -1) {
        if(!FileAccess::file_exists(savepath)) return;
        Ref<FileAccess> file = FileAccess::open(savepath, FileAccess::READ);
        String data = xor_decrypt(file->get_as_text(), HASH_KEY);
        savedata = JSON::parse_string(data);
        file->close();
    }else {
        std::ifstream file(savepath.utf8().get_data());
        file.seekg(3);
        if(!file.is_open()) return;
        std::stringstream buffer;
        buffer << file.rdbuf();
        String data = xor_decrypt(String::utf8(buffer.str().data()), HASH_KEY);
        file.close();
        savedata = JSON::parse_string(data);
    }

    if(savedata.is_empty()) savedata = Dictionary();
    // EQUIPMENT
    Dictionary inv = savedata.get("inv", Dictionary());
    equipment.merge(inv.get("equipment", Dictionary()), true);

    // ITEMS
    items = inv.get("items", Array());

    // DIM BOXES
    boxitems = inv.get("boxinv", Array::make(Array(), Array(), Array()));

    // CELLS
    cells = inv.get("cells", Array());

    // PLAYER STATS
    Dictionary stats = savedata.get("stats", Dictionary());
    player_name = stats.get("name", player_name);
    player_attack = stats.get("atk", player_attack);
    player_defense = stats.get("def", player_defense);
    player_hp = stats.get("hp", player_hp);
    player_max_hp = stats.get("max_hp", player_max_hp);
    player_lv = stats.get("lv", player_lv);
    player_exp = stats.get("exp", player_exp);
    player_gold = stats.get("gold", player_gold);
    player_kills = stats.get("kills", player_kills);
    playtime = savedata.get("playtime", 0);
    cache_playtime = playtime;

    // OVERWORLD
    overworld_data.merge(savedata.get("overworld", Dictionary()), true);

    // FLAGS
    flags = savedata.get("flags", Dictionary());
    first = savedata.get("first", true);

    battle_start = false;
    player_in_menu = false;
    player_set_menu = false;
    player_can_move = true;
    player_move = true;
    player_text_box = false;
    battle_text_box = false;
    
    refresh_audio_busses();
}

void Global::refresh_audio_busses() {
    AudioServer::get_singleton()->set_bus_volume_db(
        AudioServer::get_singleton()->get_bus_index("SFX"), 
        UtilityFunctions::linear_to_db(float(settings.get("SFX", 100)) / 100.0f)
    );
    AudioServer::get_singleton()->set_bus_volume_db(
        AudioServer::get_singleton()->get_bus_index("Music"), 
        UtilityFunctions::linear_to_db(float(settings.get("Music", 100)) / 100.0f)
    );
    AudioServer::get_singleton()->set_bus_volume_db(
        AudioServer::get_singleton()->get_bus_index("Master"), 
        UtilityFunctions::linear_to_db(float(settings.get("Master", 100)) / 100.0f)
    );
}

void Global::disable_input(String key) {
    InputMap* map = InputMap::get_singleton();
    input_event[key] = map->action_get_events(key).duplicate();
    map->action_erase_events(key);
}

void Global::enable_input(String key) {
    if(!input_event.has(key)) return;
    InputMap* map = InputMap::get_singleton();
    TypedArray<InputEvent> events = input_event[key];
    for(int i=0; i < events.size(); i++) {
        Ref<InputEvent> event = events[i];
        map->action_add_event(key, event);
    }
    input_event.erase(key);
}

bool Global::has_input_disabled(String key) {
    return input_event.has(key);
}

void Global::save(String room_name) {
    overworld_data["room_pos"] = player_position;
    if(room_name.is_empty()) {
        overworld_data["room_name"] = scene_container->get_current_scene()->get_name();
    } else {
        overworld_data["room_name"] = room_name;
    }
    save_game(false);
}

void Global::save_flag(String flag, Variant value) {
    Dictionary new_flag;
    new_flag[flag] = value;
    flags.merge(new_flag, true);
    if(!first) save_game(true);
}

void Global::_loop_Music() {
    if(Music) Music->play();
}

void Global::alert(String text, String title) {
    display->window_set_flag(DisplayServer::WINDOW_FLAG_ALWAYS_ON_TOP, false);
    os->alert(text, title);
    display->window_set_flag(DisplayServer::WINDOW_FLAG_ALWAYS_ON_TOP, true);
}

void Global::change_setting(String key, Variant value) {
    settings[key] = value;
    if(key == "SFX" || key == "Music" || key == "Master") {
        refresh_audio_busses();
    }
    get_scene_container()->_on_settings_setting_changed(key, value);
}