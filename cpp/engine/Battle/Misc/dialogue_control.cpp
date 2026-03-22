#include "dialogue_control.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>

DialogueControl::DialogueControl() {
    bubble_text = nullptr;
    tween_in_progress = false;
    isKill = false;
    character_name = "DEFAULT";
}

DialogueControl::~DialogueControl() {}

void DialogueControl::_bind_methods() {
    ADD_SIGNAL(MethodInfo("set_expression", PropertyInfo(Variant::NIL, "expressions")));
    ADD_SIGNAL(MethodInfo("started_dialogue", PropertyInfo(Variant::INT, "line")));
    ADD_SIGNAL(MethodInfo("finished_all_texts_dialogue"));
    
    ClassDB::bind_method(D_METHOD("_on_text_click_played", "duration"), &DialogueControl::_on_text_click_played);
    ClassDB::bind_method(D_METHOD("_on_text_expression_set", "expr"), &DialogueControl::_on_text_expression_set);
    ClassDB::bind_method(D_METHOD("_on_started_dialogue", "line"), &DialogueControl::_on_started_dialogue);
    ClassDB::bind_method(D_METHOD("_on_tween_finished"), &DialogueControl::_on_tween_finished);
    ClassDB::bind_method(D_METHOD("_on_all_texts_finished"), &DialogueControl::_on_all_texts_finished);
    ClassDB::bind_method(D_METHOD("_on_ends_typing"), &DialogueControl::_on_ends_typing);
    ClassDB::bind_method(D_METHOD("_text_kill"), &DialogueControl::_text_kill);
    
    ClassDB::bind_method(D_METHOD("set_character_name", "character"), &DialogueControl::set_character_name);
    ClassDB::bind_method(D_METHOD("get_character_name"), &DialogueControl::get_character_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "character_name"), "set_character_name", "get_character_name");
    
    ClassDB::bind_method(D_METHOD("type_text_bubble", "dialogues"), &DialogueControl::type_text_bubble);
    ClassDB::bind_method(D_METHOD("kill"), &DialogueControl::kill);
}

void DialogueControl::_ready() {
    bubble_text = Object::cast_to<EnemySpeech>(get_node_internal("TextContainer/Text"));
    if(!bubble_text) {
        ERR_PRINT("TextContainer/Text 노드를 찾을 수 없습니다");
        return;
    }
    
    bubble_text->set_text("");
    set_modulate(Color(1.0, 1.0, 1.0, 0.0));
    bubble_text->set_current_character(character_name);
    bubble_text->character_customize();
    
    bubble_text->connect("expression_set", Callable(this, "_on_text_expression_set"));
    bubble_text->connect("finished_speech", Callable(this, "_on_all_texts_finished"));
}

void DialogueControl::type_text_bubble(const Ref<Dialogues>& dialogues) {
    if(!bubble_text) {
        ERR_PRINT("bubble_text가 초기화되지 않았습니다");
        return;
    }
    bubble_text->character_customize();
    global->_set_battle_text_box(true);
    bubble_text->set_effects(stagehand->get_global_effects());

    Callable call = Callable(this, "_on_ends_typing");
    if(bubble_text->is_connected("ends_typing", call)) bubble_text->disconnect("ends_typing", call);
   
    current_dialogues = dialogues;
    bubble_text->set_text("");
    bubble_text->set_queued_dialogues(dialogues);
    
    active_tween = create_tween();
    tween_in_progress = true;
    
    active_tween->tween_property(this, "modulate:a", 1, 0.1);
    active_tween->connect("finished", Callable(this, "_on_tween_finished"));
}

void DialogueControl::kill() {
    if(isKill) return;
    isKill = true;
    Callable call = Callable(this, "_on_ends_typing");
    if(bubble_text->is_connected("ends_typing", call)) bubble_text->disconnect("ends_typing", call);
    bubble_text->set_click(nullptr);

    _text_kill();
    int size = current_dialogues->get_dialogues().size();
    for(int i=0; i < size; i++) {
        emit_signal("started_dialogue", i);
    }
    _on_all_texts_finished();
}

void DialogueControl::_on_text_click_played(double duration) {
    if(isKill) return;
    text_duration = duration;
    Callable call = Callable(this, "_on_ends_typing");
    if(bubble_text->is_connected("ends_typing", call)) bubble_text->disconnect("ends_typing", call);
    bubble_text->connect("ends_typing", call);
}

void DialogueControl::_on_ends_typing() {
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(text_duration, false);
    timer->connect("timeout", Callable(this, "_text_kill"));
}

void DialogueControl::_text_kill() {
    bubble_text->kill_tweens(true);
    bubble_text->emit_signal("confirm");
}

void DialogueControl::_set_key(bool is) {
    bubble_text->set_process_unhandled_input(is);
}

void DialogueControl::_on_tween_finished() {
    tween_in_progress = false;
    
    if(bubble_text) {
        bubble_text->call_deferred("type_text_advanced", bubble_text->get_queued_dialogues());
    }
}

void DialogueControl::_on_all_texts_finished() {
    active_tween = create_tween();
    active_tween->tween_property(this, "modulate:a", 0, 0.1);
    
    global->_set_battle_text_box(false);
    emit_signal("finished_all_texts_dialogue");
}

void DialogueControl::_on_started_dialogue(int line) {
    emit_signal("started_dialogue", line);
}

void DialogueControl::_on_text_expression_set(Array expr) {
    emit_signal("set_expression", expr);
}

void DialogueControl::set_character_name(String p_character) {
    character_name = p_character;
    
    if(bubble_text) {
        bubble_text->set_current_character(character_name);
        bubble_text->character_customize();
    }
}

String DialogueControl::get_character_name() const {
    return character_name;
}