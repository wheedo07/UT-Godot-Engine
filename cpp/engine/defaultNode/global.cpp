#include "global.h"
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

Global::Global() {
    Music = nullptr;
    heal_sound = nullptr;
    Info = nullptr;
    KrTimer = nullptr;
    scene_container = nullptr;
    savepath = "user://saved.json";
    settingpaths = "user://settings.json";

    // 기본 상태 변수
    first = true;
    fullscreen = false;
    debugmode = false;
    paused = false;

    battle_start = false;
    player_in_menu = false;
    player_set_menu = false;
    player_can_move = true;
    player_move = true;
    player_text_box = false;
    battle_text_box = false;
    
    // 인벤토리 관련
    boxesinmenu = false;
    unlockedboxes = 0;
    equipment["weapon"] = 0;
    equipment["armor"] = 1;
    cells = Array();
    items = Array();
    boxitems = Array(); 
    
    // 설정
    settings["shake"] = true;
    settings["vfx"] = true;
    settings["border"] = true;
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
    overworld_data["room_pos"] = Vector2(0.0, 0.0);
    playtime = 0.0;
    cache_playtime = 0.0;
    start = false;
    isMobile = false;
}

Global::~Global() {}

void Global::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    os = OS::get_singleton();
    display = DisplayServer::get_singleton();
    Music = Object::cast_to<AudioStreamPlayer>(get_node_internal("MusicGlobal"));
    heal_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("heal"));
    Info = Object::cast_to<RichTextLabel>(get_node_internal("Info"));
    KrTimer = Object::cast_to<Timer>(get_node_internal("KrTimer"));
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_HIDDEN);
    String osName = os->get_name();
    isMobile = osName == "Android";
    if(osName == "Web") return;
    if(osName == "Android") {
        call_deferred("toggle_fullscreen");
        savepath = "user://saved.json";
        settingpaths = "user://settings.json";
    }

    if(savepath.find("$home") != -1) {
        if(osName == "Windows") 
            savepaths = savepath.replace("$home", std::getenv("homepath"));
    }else savepaths = savepath;

    if(savepaths.find("user://") == -1) {
        String dir = savepaths.replace("saved.json", "");
        settingpaths = dir + "settings.json";
        if(fs::exists(dir.utf8().get_data())) return;

        if(!fs::create_directories(dir.utf8().get_data()))
            ERR_PRINT("디렉토리 생성 실패!");
    }
}

PackedStringArray Global::item_use_text(int item_id) {
    Ref<Item> item = item_list[item_id];
    PackedStringArray use_text = item->get_use_message();
    
    if (item->get_heal_amount()) {
        heal(item->get_heal_amount());
        if (player_hp == player_max_hp) {
            use_text.push_back(String::utf8("* 당신의 체력은 가득찼다"));
        } else {
            use_text.push_back(vformat(String::utf8("* 당신은 체력을 %s 회복했다"), item->get_heal_amount()));
        }
    }
    if (item->get_defense_amount()) {
        temp_def += item->get_defense_amount();
        use_text.push_back(vformat(String::utf8("*당신의 방어력이 +%s 올라갔다"), item->get_defense_amount()));
    }
    if (item->get_attack_amount()) {
        temp_atk += item->get_attack_amount();
        use_text.push_back(vformat(String::utf8("*당신의 공격력이 +%s 올라갔다"), item->get_attack_amount()));
    }
    return use_text;
}

PackedStringArray Global::equip_item(int item_id) {
    Ref<Item> item = item_list[item_id];
    PackedStringArray equip_text = item->get_use_message();
    
    switch (item->get_item_type()) {
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
    heal_sound->play();
    if(player_hp + amt > player_max_hp) {
        amt = player_max_hp - player_hp;
    }
    player_hp += amt;
}

void Global::_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    if (event->is_action_pressed("toggle_fullscreen") && !isMobile) {
        toggle_fullscreen();
    }
    if (event->is_action_pressed("debug") && (os->has_feature("debug_mode") || os->is_debug_build())) {
        toggle_collision_shape_visibility();
        debugmode = !debugmode;
    }
}

void Global::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    if (debugmode) {
        if (event->is_action_pressed("refresh_scene") && os->is_debug_build()) {
            UtilityFunctions::print(String::utf8("경고: 노드가 손실될 수 있습니다!"));
            player_hp = player_max_hp;
            player_can_move = true;
            player_in_menu = false;
            
            if (Object::cast_to<SceneContainer>(get_tree()->get_current_scene())) {
                scene_container->reload_current_scene();
                return;
            }
            get_tree()->reload_current_scene();
        }
        
        if (event->is_action_pressed("force_save") && (os->is_debug_build() || os->has_feature("debug_op"))) {
            save_game(false);
        }
    }
}

void Global::_process(double delta) {
    if(isEditor) return;
    if (debugmode) {
        Info->set_text(vformat(String::utf8("[rainbow]디버그 모드[/rainbow]\nFPS: %s")
        + (os->is_debug_build() ? String::utf8("\n[R] 현재 장면 다시 로드") : String("")),
            Engine::get_singleton()->get_frames_per_second()));
    } else {
        Info->set_text("");
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
        if(savepaths.find("user://") != -1) {
            Ref<FileAccess> file = FileAccess::open(savepaths, FileAccess::READ_WRITE);
            Dictionary savedata = JSON::parse_string(file->get_as_text());
            savedata["flags"] = flags;
            file->store_line(JSON::stringify(savedata));
            file->close();
        }else {
            std::ifstream file_read(savepaths.utf8().get_data());
            file_read.seekg(3);
            std::stringstream buffer;
            buffer << file_read.rdbuf();
            Dictionary savedata = JSON::parse_string(String::utf8(buffer.str().data()));
            file_read.close();
            savedata["flags"] = flags;

            std::ofstream file_write(savepaths.utf8().get_data());
            file_write.write((char*)bom, sizeof(bom));
            file_write << JSON::stringify(savedata).utf8().get_data() << std::endl;
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
    inv["unlockedboxes"] = unlockedboxes;
    inv["boxinv"] = boxitems;
    inv["boxinmenu"] = boxesinmenu;
    
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

    if(savepaths.find("user://") != -1) {
        Ref<FileAccess> file = FileAccess::open(savepaths, FileAccess::WRITE);
        file->store_line(JSON::stringify(savedata));
        file->close();
    }else {
        std::ofstream file_write(savepaths.utf8().get_data());
        file_write.write((char*)bom, sizeof(bom));
        file_write << JSON::stringify(savedata).utf8().get_data() << std::endl;
        file_write.close();
    }
    save_settings();
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

    settings = backup["settings"];
    refresh_audio_busses();

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
    unlockedboxes = backup["unlockedboxes"];
    boxesinmenu = backup["boxesinmenu"];
    cells = backup["cells"];

    battle_start = false;
    player_in_menu = false;
    player_set_menu = false;
    player_can_move = true;
    player_move = true;
    player_text_box = false;
    battle_text_box = false;
    paused = false;
    
    if(savepaths.find("user://") != -1) {
        String dir = savepaths.replace("saved.json", "");
        Ref<DirAccess> dirAcs = DirAccess::open(dir);
        dirAcs->remove("saved.json");
    }else fs::remove(savepaths.utf8().get_data());
}

void Global::toggle_collision_shape_visibility() {
    SceneTree* tree = get_tree();
    tree->set_debug_collisions_hint(!tree->is_debugging_collisions_hint());
    
    TypedArray<Node> node_stack;
    node_stack.push_back(tree->get_root());

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
    if (player_kr > 0) {
        player_kr -= 1;
        player_hp -= 1;
    }
}

int Global::check_level_up() {
    int lv = player_lv;
    int lvup = 0;
    
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
        lvup = 1;
        player_max_hp = 16 + lv * 4;
        player_attack = 8 + lv * 2;
        player_defense = 9 + Math::ceil(lv / 4.0);
        if (lv == 20) {
            player_max_hp = 99;
            player_attack = 99;
            player_defense = 99;
        }
    } else {
        lvup = 0;
    }
    
    player_lv = lv;
    return lvup;
}

void Global::save_settings() {
    Dictionary setting_data;
    
    if(settingpaths.find("user://") != -1) {
        Ref<FileAccess> file;
        if (FileAccess::file_exists(settingpaths)) {
            file = FileAccess::open(settingpaths, FileAccess::READ);
            if (!file->eof_reached()) {
                String content = file->get_as_text();
                if (!content.is_empty()) {
                    Variant parsed = JSON::parse_string(content);
                    if (parsed.get_type() == Variant::DICTIONARY) {
                        setting_data = parsed;
                    }
                }
            }
            file->close();
        }
        
        file = FileAccess::open(settingpaths, FileAccess::WRITE);
        if (file.is_valid()) {
            setting_data["settings"] = settings;
            setting_data["g_flags"] = g_flags;
            file->store_line(JSON::stringify(setting_data));
            file->close();
        }
    } else {
        std::ifstream file_read(settingpaths.utf8().get_data());
        if(file_read.is_open()) {
            file_read.seekg(3);
            std::stringstream buffer;
            buffer << file_read.rdbuf();
            file_read.close();
            
            String content = String::utf8(buffer.str().data());
            if (!content.is_empty()) {
                Variant parsed = JSON::parse_string(content);
                if (parsed.get_type() == Variant::DICTIONARY) {
                    setting_data = parsed;
                }
            }
        }
        
        std::ofstream file_write(settingpaths.utf8().get_data());
        if(file_write.is_open()) {
            file_write.write((char*)bom, sizeof(bom));
            setting_data["settings"] = settings;
            setting_data["g_flags"] = g_flags;
            file_write << JSON::stringify(setting_data).utf8().get_data() << std::endl;
            file_write.close();
        }
    }
}

void Global::load_game() {
    if(backup.is_empty()) {
        backup["settings"] = settings.duplicate(true);
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
        backup["unlockedboxes"] = unlockedboxes;
        backup["cells"] = cells.duplicate(true);
        
        refresh_audio_busses();
    }

    Dictionary settings_data;
    if(settingpaths.find("user://") != -1) {
        if(FileAccess::file_exists(settingpaths)) {
            Ref<FileAccess> settings_file = FileAccess::open(settingpaths, FileAccess::READ);
            if (settings_file.is_valid()) {
                Variant parsed = JSON::parse_string(settings_file->get_as_text());
                if (parsed.get_type() == Variant::DICTIONARY) {
                    settings_data = parsed;
                }
                settings_file->close();
            }
        }
    } else {
        std::ifstream settings_file(settingpaths.utf8().get_data());
        if(settings_file.is_open()) {
            settings_file.seekg(3); // BOM 건너뛰기
            std::stringstream buffer;
            buffer << settings_file.rdbuf();
            settings_file.close();
            
            Variant parsed = JSON::parse_string(String::utf8(buffer.str().data()));
            if (parsed.get_type() == Variant::DICTIONARY) {
                settings_data = parsed;
            }
        }
    }

    // SETTINGS
    settings.merge(settings_data.get("settings", settings), true);
    g_flags.merge(settings_data.get("g_flags", g_flags), true);

    Dictionary savedata;
    if(savepaths.find("user://") != -1) {
        if(!FileAccess::file_exists(savepaths)) return;
        Ref<FileAccess> file = FileAccess::open(savepaths, FileAccess::READ);
        savedata = JSON::parse_string(file->get_as_text());
        file->close();
    }else {
        std::ifstream file(savepaths.utf8().get_data());
        file.seekg(3);
        if(!file.is_open()) return;
        std::stringstream buffer;
        buffer << file.rdbuf();
        savedata = JSON::parse_string(String::utf8(buffer.str().data()));
        file.close();
    }

    if (savedata.is_empty()) savedata = Dictionary();
    // EQUIPMENT
    Dictionary inv = savedata.get("inv", Dictionary());
    equipment.merge(inv.get("equipment", Dictionary()), true);

    // ITEMS
    items = inv.get("items", Array());

    // DIM BOXES
    boxesinmenu = inv.get("boxinmenu", false);
    boxitems = inv.get("boxinv", Array::make(Array(), Array(), Array()));
    unlockedboxes = inv.get("unlockedboxes", 0);

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
    playtime = savedata.get("playtime", 0.0);
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
}

void Global::save_flag(const String& flag, const Variant& value) {
    Dictionary new_flag;
    new_flag[flag] = value;
    flags.merge(new_flag, true);
    if(!first) save_game(true);
}

void Global::loop_Music() {
    if(Music) Music->play();
}

void Global::alert(String text, String title) {
    display->window_set_flag(DisplayServer::WINDOW_FLAG_ALWAYS_ON_TOP, false);
    os->alert(text, title);
    display->window_set_flag(DisplayServer::WINDOW_FLAG_ALWAYS_ON_TOP, true);
}