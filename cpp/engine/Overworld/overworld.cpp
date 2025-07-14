#include "overworld.h"
#include "env.h"
#include "engine/Camera/camera_remote_controller.h"
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

Overworld::Overworld() {
    world_name = "overworld room";
    player_path = NodePath();
    camera_path = NodePath();
    player = nullptr;
    camera = nullptr;
    music_player = nullptr;
}

Overworld::~Overworld() {}

void Overworld::_bind_methods() {
    // 스크립트 메소드
    BIND_VIRTUAL_METHOD(Overworld, ready, hash_djb2("Overworld_ready"));

    ClassDB::bind_method(D_METHOD("set_property", "value"), &Overworld::set_property);
    ClassDB::bind_method(D_METHOD("get_player"), &Overworld::get_player);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_NONE, "PlayerOverworld", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_player");

    ClassDB::bind_method(D_METHOD("room_init", "data"), &Overworld::room_init);
    ClassDB::bind_method(D_METHOD("_on_saved"), &Overworld::_on_saved);
    ClassDB::bind_method(D_METHOD("summontextbox"), &Overworld::summontextbox);
    ClassDB::bind_method(D_METHOD("start_music_fade_in"), &Overworld::start_music_fade_in);
    
    ClassDB::bind_method(D_METHOD("set_world_name", "name"), &Overworld::set_world_name);
    ClassDB::bind_method(D_METHOD("get_world_name"), &Overworld::get_world_name);
    
    ClassDB::bind_method(D_METHOD("set_player_path", "path"), &Overworld::set_player_path);
    ClassDB::bind_method(D_METHOD("get_player_path"), &Overworld::get_player_path);
    
    ClassDB::bind_method(D_METHOD("set_music", "music"), &Overworld::set_music);
    ClassDB::bind_method(D_METHOD("get_music"), &Overworld::get_music);
    
    ClassDB::bind_method(D_METHOD("set_room_entrances", "entrances"), &Overworld::set_room_entrances);
    ClassDB::bind_method(D_METHOD("get_room_entrances"), &Overworld::get_room_entrances);

    ClassDB::bind_method(D_METHOD("set_camera", "path"), &Overworld::set_camera);
    ClassDB::bind_method(D_METHOD("get_camera_path"), &Overworld::get_camera_path);
    
    // 프로퍼티 등록
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "world_name"), "set_world_name", "get_world_name");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "PlayerOverworld"), "set_player_path", "get_player_path");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "camera_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "CameraRemoteController"), "set_camera", "get_camera_path");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "music", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_music", "get_music");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "room_entrances", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_NODE_TYPE) + ":RoomEntranceNode")
    ,"set_room_entrances", "get_room_entrances");
    
    ADD_SIGNAL(MethodInfo("initialized"));
    ADD_SIGNAL(MethodInfo("room_initialized"));
}

void Overworld::_ready() {
    ResourceLoader* loader = ResourceLoader::get_singleton();
    music = music.is_null() ? Ref<AudioStream>(loader->load("res://Musics/default/overworld.wav")) : music;
    if(isEditor) return;
    text_box = loader->load("res://Overworld/text_box.tscn");
    
    if(player_path.is_empty() || camera_path.is_empty()) {
        ERR_PRINT("player_path 경로 또는 camera_path 경로 비어있음!");
        return;
    }
    CameraRemoteController* control_camera = Object::cast_to<CameraRemoteController>(get_node_internal(camera_path));
    player = Object::cast_to<PlayerOverworld>(get_node_internal(player_path));

    connect("initialized", Callable(this, "start_music_fade_in"));
    connect("initialized", Callable(control_camera, "force_update"));
    connect("initialized", Callable(control_camera, "_set_limits"));
    connect("room_initialized", Callable(control_camera, "force_update"));
    connect("room_initialized", Callable(control_camera, "_set_limits"));
    
    start_music_fade_in();

    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    }else {
        ready();
    }
}

void Overworld::ready() {}

void Overworld::start_music_fade_in() {
    camera = global->get_scene_container()->get_camera();
    music_player = global->get_Music();

    if (!camera || !music_player) {
        ERR_PRINT("Camera 또는 MusicPlayer를 찾을수 없습니다");
        return;
    }

    music_player->set_stream(music);
    music_player->play();
    music_player->set_volume_db(-70.0f);
    
    Ref<Tween> music_tween = get_tree()->create_tween();
    music_tween->tween_property(music_player, "volume_db", 0, 1.0f)->set_trans(Tween::TRANS_QUAD);
}

void Overworld::room_init(const Dictionary& data) {
    if (!player) {
        ERR_PRINT("Overworld: PlayerOverworld를 찾을수 없습니다");
        return;
    }
    global->set_player_can_move(false);
    
    if (!data.has("entrance") || !data["entrance"]) {
        Dictionary overworld_data = global->get_overworld_data();
        Array room_pos = overworld_data.get("room_pos", Array::make(0,0));
        
        if ((room_pos.size() < 2 || 
            (float(room_pos[0]) == 0 && float(room_pos[1]) == 0)) || 
            !global->get_just_died()) {
            return;
        }
        
        player->set_global_position(Vector2(float(room_pos[0]), float(room_pos[1])));
        global->set_just_died(false);
        return;
    }
    
    int entrance_id = data["entrance"];
    for (int i = 0; i < room_entrances.size(); i++) {
        RoomEntranceNode* room = Object::cast_to<RoomEntranceNode>(room_entrances[i]);
        if (room->get_door_id() != entrance_id) {
            continue;
        }
        
        Vector2 position = room->get_global_position() + room->get_pivot_offset();
        player->set_position(position);
        player->force_direction(room->get_facing_direction());
        break;
    }
    
    emit_signal("room_initialized");
}

void Overworld::_on_saved() {
    if (!player) return;
    
    Dictionary overworld_data = global->get_overworld_data();
    Vector2 player_pos = player->get_global_position();
    
    Array pos_array;
    pos_array.push_back(player_pos.x);
    pos_array.push_back(player_pos.y);
    
    overworld_data["room_pos"] = pos_array;
    global->set_overworld_data(overworld_data);
}

TextBox* Overworld::summontextbox() {
    if (text_box.is_null()) {
        ERR_PRINT("Overworld: TextBox가 로드가 안됐습니다");
        return nullptr;
    }
    
    Node* instance = text_box->instantiate();
    add_child(instance);
    
    return Object::cast_to<TextBox>(instance);
}

void Overworld::set_world_name(const String& p_name) {
    world_name = p_name;
}

String Overworld::get_world_name() const {
    return world_name;
}

void Overworld::set_player_path(const NodePath& p_path) {
    player_path = p_path;
}

NodePath Overworld::get_player_path() const {
    return player_path;
}

void Overworld::set_music(const Ref<AudioStream>& p_music) {
    music = p_music;
}

Ref<AudioStream> Overworld::get_music() const {
    return music;
}

void Overworld::set_room_entrances(const TypedArray<RoomEntranceNode>& p_entrances) {
    room_entrances = p_entrances;
}

TypedArray<RoomEntranceNode> Overworld::get_room_entrances() const {
    return room_entrances;
}

void Overworld::set_camera(const NodePath& p_path) {
    camera_path = p_path;
}

NodePath Overworld::get_camera_path() const {
    return camera_path;
}

void Overworld::set_property(Object* value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

PlayerOverworld* Overworld::get_player() {
    return player;
}