#include "settings.h"
#include "env.h"
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/shader_material.hpp>
#include<godot_cpp/classes/dir_access.hpp>

#define TIME 0.6f
Settings::Settings() {
    enabled = false;
    class_exclude = {
        "Enemy",
        "Bullet",
        "AttackBase",
        "BulletArea",
    };
}

Settings::~Settings() {
    if(load_thread.is_valid() && load_thread->is_started()) {
        load_thread->wait_to_finish();
    }
}

void Settings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("toggle"), &Settings::toggle);
    ClassDB::bind_method(D_METHOD("on_setting_changed", "btn"), &Settings::on_setting_changed);
    ClassDB::bind_method(D_METHOD("_scene_input", "text"), &Settings::_scene_input);
    ClassDB::bind_method(D_METHOD("_change_debug", "index"), &Settings::_change_debug);
    ClassDB::bind_method(D_METHOD("_change_process", "value"), &Settings::_change_process);
    ClassDB::bind_method(D_METHOD("_get_path_list"), &Settings::_get_path_list);
    ClassDB::bind_method(D_METHOD("_on_path_list_loaded"), &Settings::_on_path_list_loaded);

    ADD_SIGNAL(MethodInfo("init"));
    ADD_SIGNAL(MethodInfo("setting_changed", 
        PropertyInfo(Variant::STRING, "setting_name"),
        PropertyInfo(Variant::NIL, "to")));
}

void Settings::_ready() {
    Darken = Object::cast_to<Panel>(get_node_internal("Darken"));
    Blur = Object::cast_to<CanvasItem>(get_node_internal("Blur"));
    BusContainer = Object::cast_to<HBoxContainer>(get_node_internal("BusContainer"));
    AnimPlayer = Object::cast_to<AnimationPlayer>(get_node_internal("AnimationPlayer"));
    Options = Object::cast_to<VBoxContainer>(get_node_internal("Options/VBoxContainer"));
    process_edit = Object::cast_to<SpinBox>(get_node_internal("Options2/NinePatchRect/process_edit"));
    debug_edit = Object::cast_to<LineEdit>(get_node_internal("Options2/NinePatchRect/debug_edit"));
    debug_edit2 = Object::cast_to<OptionButton>(get_node_internal("Options2/NinePatchRect/debug_edit2"));
    os = OS::get_singleton();

    AnimPlayer->set_speed_scale(1.0f / TIME);
    Darken->set_modulate(Color(1, 1, 1, 0));
    Ref<ShaderMaterial> shader = Blur->get_material();
    shader->set_shader_parameter("lod", 0);
    AnimPlayer->play("RESET");

    TypedArray<Node> settings = Options->get_children();
    for(int i=0; i < settings.size(); i++) {
        Node* setting = Object::cast_to<Node>(settings[i]);
        setting->connect("pressed", Callable(this, "on_setting_changed").bind(setting));
    }

    if(os->is_debug_build() || os->has_feature("debug_op")) {
        load_thread.instantiate();
        load_thread->start(Callable(this, "_get_path_list"));
    }
}

void Settings::toggle() {
    enabled = !enabled;
    global->isSetting = enabled;
    process_edit->set_value(Engine::get_singleton()->get_time_scale());
    emit_signal("init");
    
    if (tw.is_valid()) {
        tw->kill();
    }

    Input::get_singleton()->set_mouse_mode(
        enabled ? Input::MOUSE_MODE_VISIBLE : Input::MOUSE_MODE_HIDDEN
    );

    tw = create_tween()->set_trans(TRANSTYPE)
    ->set_ease(Tween::EaseType(enabled ? 1 : 0))
    ->set_parallel(true);

    tw->tween_property(
        Blur,
        "material:shader_parameter/lod",
        enabled ? 1 : 0,
        TIME
    );

    tw->tween_property(
        Darken,
        "modulate:a",
        enabled ? 1 : 0,
        TIME
    );

    if(enabled) {
        AnimPlayer->play("toggle");
        process_edit->set_editable(true);
        debug_edit->set_editable(true);
        debug_edit2->set_disabled(path_list.size() == 0);
    }else {
        AnimPlayer->play_backwards("toggle");
        process_edit->set_editable(false);
        debug_edit->set_editable(false);
        debug_edit2->set_disabled(true);
    }
}

void Settings::_unhandled_input(const Ref<InputEvent>& event) {
    if(!global) return;
    if(event->is_action_pressed("ui_setting") && global->get_debugmode() &&
        (os->is_debug_build() || os->has_feature("debug_op"))) {
        toggle();
    } 
}

void Settings::on_setting_changed(Node* btn) {
    if(!enabled) return;
    emit_signal("setting_changed", btn->call("get_setting_name"), btn->call("is_pressed"));
}

void Settings::_get_path_list() {
    PackedStringArray paths;
    _scan_directory("res://Game/", paths);
    paths.sort();
    
    path_list = paths;
    call_deferred("_on_path_list_loaded");
}

void Settings::_scan_directory(const String& path, PackedStringArray& paths) {
    Ref<DirAccess> dir = DirAccess::open(path);
    if (dir.is_null()) return;
    
    dir->list_dir_begin();
    String file = dir->get_next();
    
    while(file != "") {
        if (file != "." && file != "..") {
            String full_path = path + file;
            
            if(dir->current_is_dir()) {
                _scan_directory(full_path + "/", paths);
            } else {
                String ext = file.get_extension().to_lower();
                if(ext == "tscn" || ext == "tres" || ext == "scn") {
                    paths.push_back(full_path);
                }
            }
        }
        file = dir->get_next();
    }
    dir->list_dir_end();
}

void Settings::_on_path_list_loaded() {
    if(load_thread.is_valid() && load_thread->is_started()) {
        load_thread->wait_to_finish();
    }
   
    for(int i=0; i < path_list.size(); i++) {
        debug_edit2->add_item(path_list[i].get_file(), i);
    }
}

void Settings::_scene_input(String text) {
    if(!enabled || !global) return;
    if(text.find("res://Game") == -1) {
        global->alert(tr("UT_CANT_HERE"), "Error");
        return;
    }

    ResourceLoader* loader = ResourceLoader::get_singleton();
    if(loader->exists(text)) {
        Ref<Resource> res = loader->load(text);
        if(res.is_null()) {
            global->alert(tr("UT_RES_LOAD_FAIL"), "Error");
            return;
        }
        String type = res->get_class();
        if(type == "Encounter") {
            scene_changer->load_battle(res, false);
            print_line(tr("UT_WARN_NODE_LOSS"));
        }else if(type == "PackedScene") {
            Ref<PackedScene> scene = loader->load(text);
            Node* node = scene->instantiate();
            for(String cls : class_exclude) {
                if(node->is_class(cls)) {
                    global->alert(tr("UT_RES_SCENE_FAIL"), "Error");
                    node->queue_free();
                    node = nullptr;
                    return;
                }
            }
            global->get_scene_container()->change_scene_to_file(text);
            print_line(tr("UT_WARN_NODE_LOSS"));
        }else {
            global->alert(tr("UT_RES_SCENE_FAIL"), "Error");
        }
    }else {
        global->alert(tr("UT_RES_LOAD_FAIL"), "Error");
    }
}

void Settings::_change_process(double value) {
    if(!enabled || !global) return;
    Engine::get_singleton()->set_time_scale(value);
}

void Settings::_change_debug(int index) {
    if(!enabled || !global) return;
    if(index < 0 || index >= path_list.size()) return;
    String text = path_list[index];
    _scene_input(text);
}