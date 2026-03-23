#include "name_selection.h"
#include "env.h"
#include "engine/resources/AudioLibrary/audio_library.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/variant/callable.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/translation_server.hpp>

NameSelection::NameSelection() {
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

    // 스크립트 메소드
    GDVIRTUAL_BIND(check_names, "player_name");
   
    // 사용 함수
    ClassDB::bind_method(D_METHOD("react_to_name", "text", "deny"), &NameSelection::react_to_name, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("on_name_allowed"), &NameSelection::on_name_allowed);

    ClassDB::bind_method(D_METHOD("_on_name_input_text_changed", "new_text"), &NameSelection::_on_name_input_text_changed);
    ClassDB::bind_method(D_METHOD("_on_backspace_pressed"), &NameSelection::_on_backspace_pressed);
    ClassDB::bind_method(D_METHOD("_on_name_input_text_submitted"), &NameSelection::_on_name_input_text_submitted);
    ClassDB::bind_method(D_METHOD("_on_pass_name", "is"), &NameSelection::_on_pass_name);
    ClassDB::bind_method(D_METHOD("_on_check_name", "name"), &NameSelection::_on_check_name);
    ClassDB::bind_method(D_METHOD("_on_typer_finished"), &NameSelection::_on_typer_finished);
    ClassDB::bind_method(D_METHOD("_on_choice_made", "id"), &NameSelection::_on_choice_made);
}

void NameSelection::_ready() {
    name = Object::cast_to<Control>(get_node_internal("Name"));
    name_label = Object::cast_to<Label>(get_node_internal("Name/Label"));
    no_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("no"));
    typer = Object::cast_to<GenericTextTyper>(get_node_internal("Typer"));
    confirmation = Object::cast_to<HSplitContainer>(get_node_internal("Confirmation"));
    color_rect = Object::cast_to<ColorRect>(get_node_internal("ColorRect"));
    cymbal = Object::cast_to<AudioStreamPlayer>(get_node_internal("cymbal"));
    prompt = Object::cast_to<RichTextLabel>(get_node_internal("Prompt"));
    camera = Object::cast_to<CameraController>(get_node_internal("Camera"));
    choices.push_back(Object::cast_to<OptionSelectable>(get_node_internal("Confirmation/NO")));
    choices.push_back(Object::cast_to<OptionSelectable>(get_node_internal("Confirmation/YES")));

    confirmation->hide();
    prompt->set_text(String("[shake rate=3 level=5]") + tr("UT_NAME_FALLEN_HUMAN"));
}

void NameSelection::_input(const Ref<InputEvent>& event) {
    if(!confirmable) return;
    
    if(event->is_action_pressed("ui_right")) {
        stagehand->audio_player->play_dynamic(AudioLibrary::load("res://Engine/sfx/library/choice.tres"));
        
        if(choices.size() >= 2) {
            Object::cast_to<OptionSelectable>(choices[0])->reset();
            Object::cast_to<OptionSelectable>(choices[1])->set_selected(true);
            soul_pos = 1;
        }
    }
    
    if(event->is_action_pressed("ui_left")) {
        stagehand->audio_player->play_dynamic(AudioLibrary::load("res://Engine/sfx/library/choice.tres"));
        
        if(choices.size() >= 2) {
            Object::cast_to<OptionSelectable>(choices[1])->reset();
            Object::cast_to<OptionSelectable>(choices[0])->set_selected(true);
            soul_pos = 0;
        }
    }
    
    if (event->is_action_pressed("ut_confirm")) {
        emit_signal("choice", soul_pos);
    }
}

void NameSelection::_on_name_input_text_changed(const String& new_text) {
    if(name_label->get_text().length() >= 6) return;
    
    name_label->set_text(name_label->get_text() + new_text);
}

void NameSelection::_on_backspace_pressed() {
    if(name_label->get_text().length() < 1) return;
    
    String current_text = name_label->get_text();
    name_label->set_text(current_text.substr(0, current_text.length() - 1));
}

void NameSelection::_on_name_input_text_submitted() {
    if(name_label->get_text().is_empty()) {
        no_sound->play();
        
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1, false);
        timer->connect("timeout", Callable(this, "emit_signal").bind("enable"));
        return;
    }
    
    emit_signal("disable");
    stagehand->audio_player->play_dynamic(AudioLibrary::load("res://Engine/sfx/library/select.tres"));
    
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1, false);
    timer->connect("timeout", Callable(this, "_on_check_name").bind(name_label->get_text().to_upper()));
}

void NameSelection::_on_check_name(String name) {
    if(has_method("check_names")) { // C++ 이랑 GDscript 모두 호환되도록
        call("check_names", name);
    }else {
        check_names(name);
    }
}

void NameSelection::react_to_name(const String& text, bool deny) {
    tween_namepos = create_tween()->set_parallel()->set_trans(Tween::TRANS_CUBIC)->set_ease(Tween::EASE_OUT);
    tween_namepos->tween_property(name_label, "scale", Vector2(3,3), 10);
    tween_namepos->tween_property(name_label, "position", Vector2(-630, 120), 10);

    tween_shake = create_tween()->set_loops()->set_parallel()->set_trans(Tween::TRANS_SINE)->set_ease(Tween::EASE_IN_OUT);
    tween_shake->tween_property(name, "rotation_degrees", -0.5, 0.1);
    tween_shake->tween_property(name, "position:x", name->get_position().x - 0.5, 0.1);
    tween_shake->tween_property(name, "position:y", name->get_position().y + 1, 0.1);
    tween_shake->tween_property(name, "position:x", name->get_position().x + 1, 0.1)->set_delay(0.1);
    tween_shake->tween_property(name, "position:y", name->get_position().y - 0.5, 0.1)->set_delay(0.1);
    tween_shake->tween_property(name, "rotation_degrees", 0.5, 0.1)->set_delay(0.1);

    prompt->hide();
    typer->type_text({ text });
    typer->connect("finished_all_texts", Callable(this, "_on_typer_finished").bind(deny), CONNECT_ONE_SHOT);
}

void NameSelection::_on_typer_finished(bool is_deny) {
    if(is_deny) {
        global->_set_player_text_box(true);
        typer->connect("confirm", Callable(this, "emit_signal").bind("pass_name", false), CONNECT_ONE_SHOT);
        typer->connect("confirm", Callable(global, "_set_player_text_box").bind(false), CONNECT_ONE_SHOT);
    }else {
        if(choices.size() < 2 || !confirmation) return;
        
        Object::cast_to<OptionSelectable>(choices[1])->reset(); 
        Object::cast_to<OptionSelectable>(choices[0])->set_selected(true);
        soul_pos = 0;
        
        confirmation->show();
        confirmable = true;
        
        connect("choice", Callable(this, "_on_choice_made"), CONNECT_ONE_SHOT);
    }
}

void NameSelection::_on_choice_made(int id) {
    if(!confirmation) return;
    
    confirmation->hide();
    confirmable = false;
    emit_signal("pass_name", id);
}

void NameSelection::on_name_allowed() {
    typer->hide();
    prompt->hide();
    confirmation->hide();
    confirmable = false;

    Ref<Tween> tween = create_tween()->set_trans(Tween::TRANS_CUBIC)->set_ease(Tween::EASE_IN);
    tween->tween_property(color_rect, "modulate:a", 1, 5);
    global->set_player_name(name_label->get_text());
    cymbal->play();

    cymbal->connect("finished", Callable(scene_changer, "enter_room_default"), CONNECT_ONE_SHOT);
}

void NameSelection::_on_pass_name(bool is) {
    if(is) {
        on_name_allowed();
    }else {
        if(tween_namepos.is_valid()) tween_namepos->kill();
        if(tween_shake.is_valid()) tween_shake->kill();
        name->set_rotation_degrees(0);
        name->set_position(Vector2(0, 111));
        name_label->set_scale(Vector2(1,1));
        name_label->set_position(Vector2(0, 0));
        typer->set_text("");
        call_deferred("emit_signal", "enable");
    }
}

// 수정 필요 할시 수정
void NameSelection::check_names(String player_name) {
    react_to_name(String::utf8("이것이 이름입니까?"));
}