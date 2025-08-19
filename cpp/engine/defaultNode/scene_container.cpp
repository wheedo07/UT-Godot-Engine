#include "scene_container.h"
#include "env.h"
#include<godot_cpp/classes/display_server.hpp>
#include<godot_cpp/classes/window.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/engine.hpp>

SceneContainer::SceneContainer() {
    current_scene = nullptr;
    camera = nullptr;
}

SceneContainer::~SceneContainer() {}

void SceneContainer::_bind_methods() {
    ADD_SIGNAL(MethodInfo("change_scene"));
    ClassDB::bind_method(D_METHOD("set_current_scene", "scene"), &SceneContainer::set_current_scene);
    ClassDB::bind_method(D_METHOD("get_current_scene"), &SceneContainer::get_current_scene);
    ClassDB::bind_method(D_METHOD("change_scene_to_file", "path"), &SceneContainer::change_scene_to_file);
    ClassDB::bind_method(D_METHOD("change_scene_to_packed", "file"), &SceneContainer::change_scene_to_packed);
    ClassDB::bind_method(D_METHOD("unload_current_scene"), &SceneContainer::unload_current_scene);
    ClassDB::bind_method(D_METHOD("reload_current_scene"), &SceneContainer::reload_current_scene);
    ClassDB::bind_method(D_METHOD("reload_camera"), &SceneContainer::reload_camera);
    ClassDB::bind_method(D_METHOD("init_camera"), &SceneContainer::init_camera);
    ClassDB::bind_method(D_METHOD("get_camera"), &SceneContainer::get_camera);
    ClassDB::bind_method(D_METHOD("_on_fullscreen_toggle", "to"), &SceneContainer::_on_fullscreen_toggle);
    ClassDB::bind_method(D_METHOD("_on_settings_setting_changed", "setting_name", "to"), &SceneContainer::_on_settings_setting_changed);
}

void SceneContainer::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    loader = ResourceLoader::get_singleton();
    camera_scene = loader->load("res://Camera/camera_fx.tscn");
    settings_viewport_container = Object::cast_to<SubViewportContainer>(get_node_internal("SettingsContainer"));
    main_viewport_container = Object::cast_to<SubViewportContainer>(get_node_internal("SubViewportContainer"));
    main_viewport = Object::cast_to<SubViewport>(get_node_internal("SubViewportContainer/MainViewport"));
    border = Object::cast_to<ReferenceRect>(get_node_internal("SettingsContainer/Border"));
    screen_copy = Object::cast_to<TextureRect>(get_node_internal("ScreenCopy"));
    mobile = Object::cast_to<Control>(get_node_internal("Mobile"));

    global = Object::cast_to<Global>(get_node_internal("/root/Globals"));
    scene_changer = Object::cast_to<OverworldSceneChanger>(get_node_internal("/root/OverworldSceneChangers"));
    audio_player = Object::cast_to<AudioPlayer>(get_node_internal("/root/AudioPlayers"));

    global->load_game();
    global->set_scene_container(this);
    global->connect("fullscreen_toggled", Callable(this, "_on_fullscreen_toggle"));
    settings_viewport_container->get_node_internal("SubViewport/Settings")->emit_signal("init");

    if(global->isMobile()) {
        mobile->show();
    }

    reload_camera();
    change_scene_to_file("res://Intro/intro.tscn");
    _on_settings_setting_changed("border", global->get_settings()["border"]);
}

void SceneContainer::set_current_scene(Node* p_scene) {
    current_scene = p_scene;
}

Node* SceneContainer::get_current_scene() const {
    return current_scene;
}

Error SceneContainer::change_scene_to_file(const String& path) {
    if (!loader->exists(path)) {
        ERR_PRINT("change 파일 찾을수 없음");
        return ERR_INVALID_PARAMETER;
    }
    Ref<PackedScene> scene_resource = loader->load(path);
    if (scene_resource.is_null()) {
        ERR_PRINT("change 파일 null");
        return ERR_INVALID_DATA;
    }
    return change_scene_to_packed(scene_resource);
}

Error SceneContainer::change_scene_to_packed(const Ref<PackedScene>& file) {
    unload_current_scene();
    current_scene = file->instantiate();
    if(ObjectDB::get_instance(current_scene->get_instance_id())) {
        main_viewport->add_child(current_scene);
    }else ERR_PRINT("노드 유효하지 않는 에러");
    call_deferred("emit_signal", "change_scene");
    return OK;
}

void SceneContainer::unload_current_scene() {
    if (!current_scene) return;
    current_scene->get_parent()->remove_child(current_scene);
    current_scene->queue_free();
    current_scene = nullptr;
}

void SceneContainer::reload_current_scene() {
    if (!current_scene) return;
    change_scene_to_file(current_scene->get_scene_file_path());
}

void SceneContainer::reload_camera() {
    if(camera) {
        main_viewport->remove_child(camera);
        camera->queue_free();
    }
    camera = Object::cast_to<CameraFx>(camera_scene->instantiate());
    camera->set_process_mode(ProcessMode::PROCESS_MODE_PAUSABLE);
    main_viewport->add_child(camera);
    camera->set_name("GlobalCamera");
    camera->set_unique_name_in_owner(true);
    init_camera();
}

void SceneContainer::init_camera() {
    camera->set_position(Vector2(320, 240));
    camera->reset_smoothing();
}

int SceneContainer::get_fullscreen_scale() const {
    Vector2i sc_size = DisplayServer::get_singleton()->screen_get_size();
    return MIN(Math::floor(sc_size.x / SCREEN_SIZE.x), Math::floor(sc_size.y / SCREEN_SIZE.y));
}

void SceneContainer::_on_fullscreen_toggle(bool to) {
    Vector2 scale = Vector2(to ? get_fullscreen_scale() : 1, to ? get_fullscreen_scale() : 1);
    main_viewport_container->set_scale(scale);
    settings_viewport_container->set_scale(scale);
    mobile->set_scale(scale);
    Vector2 screen_size = DisplayServer::get_singleton()->screen_get_size();
    Vector2 mobile_size = SCREEN_SIZE * scale;
    Vector2 center_pos = (screen_size - mobile_size) / 2;
    mobile->set_position(center_pos);
    if (_just_toggled_border) {
        _refresh_window();
    }
}

void SceneContainer::_refresh_window() {
    _just_toggled_border = false;
    Dictionary settings = global->get_settings();
    bool to = settings["border"];
    Window* window = get_window();
    window->set_size(to ? Vector2(960, 540) : SCREEN_SIZE);
    window->move_to_center();
}

void SceneContainer::_on_settings_setting_changed(const String& setting_name, const Variant& to) {
    if (setting_name == "border") {
        _refresh_window();
        _just_toggled_border = true;
        border->set_visible(to);
        screen_copy->set_visible(to);
        if (global->get_fullscreen()) {
            DisplayServer::get_singleton()->window_set_mode(DisplayServer::WINDOW_MODE_EXCLUSIVE_FULLSCREEN);
        }
    }
}

CameraFx* SceneContainer::get_camera() {
    return camera;
}

SubViewport* SceneContainer::get_main_viewport() {
    return main_viewport;
}