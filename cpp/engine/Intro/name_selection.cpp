#include "name_selection.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/variant/callable.hpp>
#include<godot_cpp/classes/engine.hpp>

NameSelection::NameSelection() {
    name_text = "";
    confirmable = false;
    soul_pos = 0;
    
    name_label = nullptr;
    no_sound = nullptr;
    typer = nullptr;
    confirmation = nullptr;
    color_rect = nullptr;
    cymbal = nullptr;
    prompt = nullptr;
    camera = nullptr;
}

NameSelection::~NameSelection() {}

void NameSelection::_bind_methods() {
    ADD_SIGNAL(MethodInfo("disable"));
    ADD_SIGNAL(MethodInfo("enable"));
    ADD_SIGNAL(MethodInfo("pass_name", PropertyInfo(Variant::BOOL, "allowed")));
    ADD_SIGNAL(MethodInfo("choice", PropertyInfo(Variant::INT, "id")));
   
    ClassDB::bind_method(D_METHOD("on_name_input_text_changed", "new_text"), &NameSelection::on_name_input_text_changed);
    ClassDB::bind_method(D_METHOD("on_backspace_pressed"), &NameSelection::on_backspace_pressed);
    ClassDB::bind_method(D_METHOD("on_name_input_text_submitted"), &NameSelection::on_name_input_text_submitted);
    ClassDB::bind_method(D_METHOD("check_names", "name"), &NameSelection::check_names);
    ClassDB::bind_method(D_METHOD("react_to_name", "text", "deny"), &NameSelection::react_to_name, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("await_confirm"), &NameSelection::await_confirm);
    ClassDB::bind_method(D_METHOD("on_pass_name", "is"), &NameSelection::on_pass_name);
    
    ClassDB::bind_method(D_METHOD("on_typer_finished"), &NameSelection::on_typer_finished);
    ClassDB::bind_method(D_METHOD("on_choice_made", "id"), &NameSelection::on_choice_made);
}

void NameSelection::_ready() {
    if(isEditor) return;
    name_label = Object::cast_to<Label>(get_node_internal("Name"));
    no_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("no"));
    typer = Object::cast_to<GenericTextTyper>(get_node_internal("Typer"));
    confirmation = Object::cast_to<HSplitContainer>(get_node_internal("Confirmation"));
    color_rect = Object::cast_to<ColorRect>(get_node_internal("ColorRect"));
    cymbal = Object::cast_to<AudioStreamPlayer>(get_node_internal("cymbal"));
    prompt = get_node_internal("Prompt");
    camera = Object::cast_to<CameraRemoteController>(get_node_internal("Camera"));
    choices.push_back(Object::cast_to<OptionSelectable>(get_node_internal("Confirmation/YES")));
    choices.push_back(Object::cast_to<OptionSelectable>(get_node_internal("Confirmation/NO")));
    confirmation->hide();
}

void NameSelection::on_name_input_text_changed(const String& new_text) {
    if(name_label->get_text().length() >= 6) return;
    
    name_label->set_text(name_label->get_text() + new_text);
}

void NameSelection::on_backspace_pressed() {
    if(name_label->get_text().length() < 1) return;
    
    String current_text = name_label->get_text();
    name_label->set_text(current_text.substr(0, current_text.length() - 1));
}

void NameSelection::on_name_input_text_submitted() {
    if(name_label->get_text().is_empty()) {
        audio_player->play("hurt");
        
        SceneTree* tree = get_tree();
        if (tree) {
            Ref<SceneTreeTimer> timer = tree->create_timer(0.1);
            timer->connect("timeout", Callable(this, "emit_signal").bind("enable"));
        }
        return;
    }
    
    emit_signal("disable");
    audio_player->play("select");
    
    name_text = name_label->get_text().to_upper();
    
    SceneTree* tree = get_tree();
    if (tree) {
        Ref<SceneTreeTimer> timer = tree->create_timer(0.1);
        timer->connect("timeout", Callable(this, "check_names").bind(name_text));
    }
}

void NameSelection::check_names(const String& name) {
    if(name == "GASTER") {
        global->alert("NO", "ERROR");
    }else if (name == "SANS") {
        react_to_name("[center]NO!!!!!", true);
    }else {
        react_to_name(String::utf8("[center]이것이 이름입니까?"));
    }
}

void NameSelection::react_to_name(const String& text, bool deny) {
    if (typer->is_connected("finished_all_texts", Callable(this, "on_typer_finished"))) {
        typer->disconnect("finished_all_texts", Callable(this, "on_typer_finished"));
    }
    
    Array text_array;
    text_array.push_back(text);
    typer->type_text(text_array);
    is_deny = deny;
    
    typer->connect("finished_all_texts", Callable(this, "on_typer_finished"), Object::CONNECT_ONE_SHOT);
}

void NameSelection::on_typer_finished() {
    if (is_deny) {
        global->set_player_text_box(true);
        typer->connect("confirm", Callable(this, "emit_signal").bind("pass_name", false), CONNECT_ONE_SHOT);
        typer->connect("confirm", Callable(global, "set_player_text_box").bind(false), CONNECT_ONE_SHOT);
    } else {
        await_confirm();
    }
}

void NameSelection::await_confirm() {
    if (choices.size() < 2 || !confirmation) return;
    
    Object::cast_to<OptionSelectable>(choices[1])->reset(); 
    Object::cast_to<OptionSelectable>(choices[0])->set_selected(true);
    soul_pos = 0;
    
    confirmation->show();
    confirmable = true;
    
    connect("choice", Callable(this, "on_choice_made"), Object::CONNECT_ONE_SHOT);
}

void NameSelection::on_choice_made(int id) {
    if (!confirmation) return;
    
    confirmation->hide();
    confirmable = false;
    
    emit_signal("pass_name", !id);
}

void NameSelection::on_name_allowed() {
    typer->hide();
    global->set_player_name(name_label->get_text());
    
    CameraFx* camerafx = global->get_scene_container()->get_camera();
    Ref<Tween> tween = create_tween()->set_trans(Tween::TRANS_CUBIC)->set_ease(Tween::EASE_OUT);
    tween->set_parallel(true);
    
    tween->tween_property(prompt, "modulate:a", 0, 0.3);
    
    camerafx->tween_zoom(Vector2(5,5), 6);
    tween->tween_property(color_rect, "modulate:a", 1, 6);
    tween->set_ease(Tween::EASE_IN);
    
    tween->tween_property(name_label, "position:y", 235, 1);
    tween->tween_callback(Callable(cymbal, "play"))->set_delay(0.89);
    
    tween->connect("finished", Callable(camerafx, "kill"), CONNECT_ONE_SHOT);
    tween->connect("finished", Callable(scene_changer, "enter_room_default"), CONNECT_ONE_SHOT);
}

void NameSelection::_input(const Ref<InputEvent>& event) {
    if(isEditor || !confirmable) return;
    
    if (event->is_action_pressed("ui_right")) {
        audio_player->play("choice");
        
        if (choices.size() >= 2) {
            Object::cast_to<OptionSelectable>(choices[0])->reset();
            Object::cast_to<OptionSelectable>(choices[1])->set_selected(true);
            soul_pos = 1;
        }
    }
    
    if (event->is_action_pressed("ui_left")) {
        audio_player->play("choice");
        
        if (choices.size() >= 2) {
            Object::cast_to<OptionSelectable>(choices[1])->reset();
            Object::cast_to<OptionSelectable>(choices[0])->set_selected(true);
            soul_pos = 0;
        }
    }
    
    if (event->is_action_pressed("ui_accept")) {
        emit_signal("choice", soul_pos);
    }
}

void NameSelection::on_pass_name(bool is) {
    if(is) {
        on_name_allowed();
    }else {
        typer->set_text("");
        call_deferred("emit_signal", "enable");
    }
}