#include "run.h"
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/project_settings.hpp>
#include<godot_cpp/classes/editor_interface.hpp>
#include "undertale-plugin/Dialog/run_scene_dialog.h"
#include "undertale-plugin/Editor/ut_editor_plugin.h"
#include "undertale-plugin/Editor/debugger_editor_plugin.h"
using namespace godot;

void UTEditorRunBtn::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_setup_dialogs"), &UTEditorRunBtn::_setup_dialogs);
    ClassDB::bind_method(D_METHOD("_on_layer_dialog_confirmed"), &UTEditorRunBtn::_on_layer_dialog_confirmed);
}

void UTEditorRunBtn::_ready() {
    set_text("UTGE: Run In Layer");
    call_deferred("_setup_dialogs");
}

void UTEditorRunBtn::_setup_dialogs() {
    EditorInterface *editor = EditorInterface::get_singleton();
    ERR_FAIL_NULL(editor);
    Control *base_control = editor->get_base_control();
    ERR_FAIL_NULL(base_control);

    layer_dialog = memnew(UTRunSceneDialog);
    base_control->add_child(layer_dialog);

    alert_dialog = memnew(AcceptDialog);
    alert_dialog->set_title("UTGE Run");
    base_control->add_child(alert_dialog);

    layer_dialog->connect("confirmed", Callable(this, "_on_layer_dialog_confirmed"));
}

void UTEditorRunBtn::_pressed() {
    EditorInterface *editor = EditorInterface::get_singleton();
    ERR_FAIL_NULL(editor);
    ERR_FAIL_NULL(layer_dialog);
    ERR_FAIL_NULL(alert_dialog);

    Node *edited_scene = editor->get_edited_scene_root();
    if(!edited_scene) {
        _show_alert("Open a scene first.");
        return;
    }

    current_scene_path = edited_scene->get_scene_file_path();
    if(current_scene_path.is_empty()) {
        _show_alert("The current scene must be saved before running.");
        return;
    }

    ProjectSettings *settings = ProjectSettings::get_singleton();
    ERR_FAIL_NULL(settings);

    String main_scene_setting = settings->get_setting("application/run/main_scene");
    if(main_scene_setting.is_empty()) {
        _show_alert("Project main scene is not configured.");
        return;
    }
    
    Ref<PackedScene> main_scene = ResourceLoader::get_singleton()->load(main_scene_setting, "PackedScene");
    if(main_scene.is_null()) {
        _show_alert("Failed to load the project main scene.");
        return;
    }
    
    String main_scene_path = main_scene->get_path();
    if(main_scene_path == current_scene_path) {
        _show_alert("The current scene cannot be the main scene.");
        return;
    }

    Node *main_root = main_scene->instantiate();
    if(!main_root) {
        _show_alert("Failed to instantiate the project main scene.");
        return;
    }

    if(!main_root->is_class("UTGERoot")) {
        main_root->queue_free();
        _show_alert("The project main scene root must be UTGERoot.");
        return;
    }

    Variant layer_parent_value = main_root->get("layer_parent");
    Node *layer_parent = nullptr;
    if(layer_parent_value.get_type() == Variant::OBJECT) {
        Object *obj = layer_parent_value.get_validated_object();
        if(obj) layer_parent = Object::cast_to<Node>(obj);
    }
    if(!layer_parent) layer_parent = main_root;

    PackedStringArray layer_name;
    TypedArray<Node> children = layer_parent->get_children();
    for(int i=0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        if(!child || !child->is_class("UTGELayer")) continue;
        layer_name.push_back(child->get("layer_id"));
    }
    main_root->queue_free();

    if(layer_name.is_empty()) {
        _show_alert("No UTGELayer nodes were found in the main scene.");
        return;
    }

    layer_dialog->set_layers(layer_name);
    layer_dialog->popup_centered(Vector2i(360, 260));
}

void UTEditorRunBtn::_show_alert(String message) {
    ERR_FAIL_NULL(alert_dialog);
    alert_dialog->set_text(message);
    alert_dialog->popup_centered(Vector2i(360, 120));
}

void UTEditorRunBtn::_on_layer_dialog_confirmed() {
    ERR_FAIL_NULL(layer_dialog);

    StringName layer_id = layer_dialog->get_selected_layer();
    if(layer_id == StringName()) {
        _show_alert("Select a layer first.");
        return;
    }

    plugin->current_play(layer_id, current_scene_path);
}

void UTEditorRunBtn::set_plugin(UTEditorPlugin *value) {
    plugin = value;
}