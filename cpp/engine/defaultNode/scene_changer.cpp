#include "scene_changer.h"
#include "env.h"
#include "engine/Battle/battle_system.h"
#include "engine/Overworld/overworld.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/classes/scene_state.hpp>
#include<godot_cpp/classes/window.hpp>

SceneChanger::SceneChanger() {
    blind_time = 0.4f;
    
    data = Dictionary();
    data["entrance"] = Variant();
    data["direction"] = Vector2i(0, 1);
    
    data_default = data.duplicate();
    
    waiting_for_transition = false;
    default_scene = "";
    battle_scene_path = "res://Main/battle.tscn";
    current_node = nullptr;
}

SceneChanger::~SceneChanger() {}

void SceneChanger::_bind_methods() {
    ClassDB::bind_method(D_METHOD("enter_room_default"), &SceneChanger::enter_room_default);
    ClassDB::bind_method(D_METHOD("enter_room_path", "room_path", "extra_data"), &SceneChanger::enter_room_path, DEFVAL(Dictionary()));
    ClassDB::bind_method(D_METHOD("load_cached_scene", "transition"), &SceneChanger::load_cached_scene, DEFVAL(true));
    ClassDB::bind_method(D_METHOD("load_battle", "battle_resource", "transition", "to_position"), &SceneChanger::load_battle, DEFVAL(true), DEFVAL(Vector2(48, 452)));
    ClassDB::bind_method(D_METHOD("load_general_scene", "scene_path"), &SceneChanger::load_general_scene);
    ClassDB::bind_method(D_METHOD("is_cached_overworld_scene"), &SceneChanger::is_cached_overworld_scene);
    
    ClassDB::bind_method(D_METHOD("_load_and_set_scene", "path"), &SceneChanger::_load_and_set_scene);
    ClassDB::bind_method(D_METHOD("_set_player_data", "current_scene"), &SceneChanger::_set_player_data);
    ClassDB::bind_method(D_METHOD("_on_battle_transition_finished"), &SceneChanger::_on_battle_transition_finished);
    ClassDB::bind_method(D_METHOD("_on_scene_setup_finished", "transition"), &SceneChanger::_on_scene_setup_finished);
    
    ClassDB::bind_method(D_METHOD("set_default_scene", "scene"), &SceneChanger::set_default_scene);
    ClassDB::bind_method(D_METHOD("get_default_scene"), &SceneChanger::get_default_scene);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_scene", PROPERTY_HINT_FILE, "*.tscn"), "set_default_scene", "get_default_scene");

    ClassDB::bind_method(D_METHOD("set_battle_scene_path", "path"), &SceneChanger::set_battle_scene_path);
    ClassDB::bind_method(D_METHOD("get_battle_scene_path"), &SceneChanger::get_battle_scene_path);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "battle_scene_path", PROPERTY_HINT_FILE, "*.tscn"), "set_battle_scene_path", "get_battle_scene_path");
}

void SceneChanger::_ready() {
    loader = ResourceLoader::get_singleton();
    if(data_default.is_empty()) {
        data_default = data.duplicate();
    }
    if(default_scene.is_empty()) {
        ERR_PRINT("default_scene 이 비어있습니다. 기본 씬을 설정해주세요.");
    }
    if(battle_scene_path.is_empty()) {
        ERR_PRINT("battle_scene_path 이 비어있습니다. 전투 씬 경로를 설정해주세요.");
    }
}

void SceneChanger::enter_room_default() {
    CameraFx* camera = global->get_scene_container()->get_camera();
    camera->blind(0.1, 1);
    camera->connect("finished_tween", Callable(this, "_load_and_set_scene").bind(default_scene), CONNECT_ONE_SHOT);
}

void SceneChanger::enter_room_path(const String& room_path, const Dictionary& extra_data) {
    data = extra_data.duplicate();
    
    Dictionary overworld_data = global->get_overworld_data();
    overworld_data["room"] = room_path;
    global->set_overworld_data(overworld_data);
    
    CameraFx* camera = global->get_scene_container()->get_camera();
    camera->blind(0.1, 1);
    camera->connect("finished_tween", Callable(this, "_load_and_set_scene").bind(room_path), CONNECT_ONE_SHOT);
}

void SceneChanger::_load_and_set_scene(const String& path) {
    Ref<PackedScene> resource = loader->load(path);
    
    Dictionary overworld_data = global->get_overworld_data();
    overworld_data["room"] = path;
    global->set_overworld_data(overworld_data);
    
    global->get_scene_container()->change_scene_to_packed(resource);
    global->get_scene_container()->get_camera()->blind(blind_time, 1, 0.35);
    global->get_scene_container()->connect("change_scene", Callable(this, "_set_player_data").bind(global->get_scene_container()->get_current_scene()), CONNECT_ONE_SHOT);
}

void SceneChanger::_set_player_data(Node* current_scene) {
    if (!global) {
        ERR_PRINT("global 로드 문제");
        return;
    }
    
    if(current_scene->has_method("_room_init")) {
        current_scene->call("_room_init", data);
    }

    Ref<SceneTreeTimer> timer = get_tree()->create_timer(blind_time * 2, false);
    timer->connect("timeout", Callable(global, "set_player_can_move").bind(true), CONNECT_ONE_SHOT);
    timer->connect("timeout", Callable(global, "_set_player_in_menu").bind(false), CONNECT_ONE_SHOT);
    
    data = data_default.duplicate();
}

void SceneChanger::load_cached_scene(bool transition) {
    if (!global) {
        ERR_PRINT("global 로드 문제");
        _on_scene_setup_finished(transition);
        return;
    }
    CameraFx* camera = global->get_scene_container()->get_camera();
    
    global->_set_player_in_menu(false);
    global->set_player_can_move(true);
    
    if (transition) {
        camera->blind(0.1, 1);
        camera->connect("finished_tween", Callable(this, "_on_scene_setup_finished").bind(transition), CONNECT_ONE_SHOT);
    } else {
        _on_scene_setup_finished(transition);
    }
}

void SceneChanger::_on_scene_setup_finished(bool transition) {
    if (!global) {
        ERR_PRINT("global 로드 문제");
        return;
    }
    
    CameraFx* camera = global->get_scene_container()->get_camera();
    SceneContainer* tree = global->get_scene_container();
    tree->unload_current_scene();

    Ref<PackedScene> scene = loader->load(default_scene);
    Overworld* sc = nullptr;
    if(is_cached_overworld_scene()) {
        sc = Object::cast_to<Overworld>(current_node);
    }else if(scene.is_valid() && !current_node) {
        sc = Object::cast_to<Overworld>(scene->instantiate());
    }
    
    if(sc) {
        tree->set_current_scene(sc);
        tree->get_main_viewport()->add_child(sc);
        
        if(sc->has_signal("initialized")) sc->emit_signal("initialized");
        if(sc->has_method("ready")) sc->call("ready"); // // C++ 이랑 GDscript 모두 호환되도록
        else sc->ready();
    }else if(current_node) {
        tree->set_current_scene(current_node);
        current_node->request_ready();
        tree->get_main_viewport()->add_child(current_node);
    }else {
        ERR_PRINT("캐시된 씬을 불러오지 못했습니다.");
    }
    global->call_deferred("_set_battle_start", false);

    if (transition) {
        camera->blind(blind_time, 1, 0.35);
    }
}

void SceneChanger::load_battle(const Ref<Encounter>& battle_resource, bool transition, const Vector2& to_position) {
    if (!global) {
        ERR_PRINT("global 로드 문제");
        return;
    }
    SceneContainer* tree = global->get_scene_container();
    
    if (transition) {
        Ref<PackedScene> transition_scene = loader->load("res://Overworld/battle_transition.tscn");
        BattleTransition* screen = Object::cast_to<BattleTransition>(transition_scene->instantiate());
        set_meta("scene_path", battle_scene_path);
        set_meta("encounter", battle_resource);
        set_meta("to_position", to_position);
        
        screen->set_target(to_position);
        tree->get_current_scene()->add_child(screen);
        
        waiting_for_transition = true;
        screen->connect("completed", Callable(this, "_on_battle_transition_finished"), CONNECT_ONE_SHOT);
        screen->transition();
        
    } else {
        _load_battle_scene(battle_scene_path, battle_resource);
    }
}

bool SceneChanger::is_cached_overworld_scene() {
    return current_node && current_node->is_class("Overworld");
}

void SceneChanger::_on_battle_transition_finished() {
    if (!waiting_for_transition) return;
    waiting_for_transition = false;
    
    String scene_path = get_meta("scene_path");
    Ref<Encounter> encounter = get_meta("encounter");
    
    _load_battle_scene(scene_path, encounter);
}

void SceneChanger::_load_battle_scene(const String& scene_path, const Ref<Encounter>& encounter) {
    if (!global) {
        ERR_PRINT("global 로드 문제");
        return;
    }
    
    global->_set_player_in_menu(false);
    global->set_player_can_move(true);
    
    Ref<PackedScene> battle_scene = loader->load(scene_path);
    BattleMain* battle = Object::cast_to<BattleMain>(battle_scene->instantiate());
    battle->set_encounter(encounter);
    
    SceneContainer* tree = global->get_scene_container();
    current_node = tree->get_current_scene();
    tree->get_main_viewport()->remove_child(current_node);
    
    tree->set_current_scene(battle);
    tree->reload_camera();
    tree->get_main_viewport()->add_child(battle);
}

void SceneChanger::load_general_scene(const String& scene_path) {
    global->get_scene_container()->change_scene_to_file(scene_path);
}

void SceneChanger::set_default_scene(const String& p_scene) {
    default_scene = p_scene;
}

String SceneChanger::get_default_scene() const {
    return default_scene;
}

void SceneChanger::set_battle_scene_path(const String& p_path) {
    battle_scene_path = p_path;
}

String SceneChanger::get_battle_scene_path() const {
    return battle_scene_path;
}