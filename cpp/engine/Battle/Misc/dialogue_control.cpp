#include "dialogue_control.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>

DialogueControl::DialogueControl() {
    bubble_text = nullptr;
    tween_in_progress = false;
    custom_character = DEFAULT;
    text_size = 14;
}

DialogueControl::~DialogueControl() {
}

void DialogueControl::_bind_methods() {
    ADD_SIGNAL(MethodInfo("set_expression", PropertyInfo(Variant::ARRAY, "expressions")));
    ADD_SIGNAL(MethodInfo("finished_all_texts_dialogue"));
    
    ClassDB::bind_method(D_METHOD("DialogueText", "dialogues"), &DialogueControl::DialogueText);
    ClassDB::bind_method(D_METHOD("on_text_click_played", "is", "duration"), &DialogueControl::on_text_click_played, DEFVAL(true), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("set_key", "is"), &DialogueControl::set_key);
    ClassDB::bind_method(D_METHOD("_on_text_expression_set", "expr"), &DialogueControl::_on_text_expression_set);
    ClassDB::bind_method(D_METHOD("_on_tween_finished"), &DialogueControl::_on_tween_finished);
    ClassDB::bind_method(D_METHOD("_on_all_texts_finished"), &DialogueControl::_on_all_texts_finished);
    ClassDB::bind_method(D_METHOD("_on_ends_typing"), &DialogueControl::_on_ends_typing);
    
    ClassDB::bind_method(D_METHOD("set_custom_character", "character"), &DialogueControl::set_custom_character);
    ClassDB::bind_method(D_METHOD("get_custom_character"), &DialogueControl::get_custom_character);
    bind_enum(get_class_static(), "set_custom_character", "get_custom_character");

    ClassDB::bind_method(D_METHOD("set_text_size", "size"), &DialogueControl::set_text_size);
    ClassDB::bind_method(D_METHOD("get_text_size"), &DialogueControl::get_text_size);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "text_size", PROPERTY_HINT_RANGE, "8,32,1"), "set_text_size", "get_text_size");
}

void DialogueControl::_ready() {
    if(isEditor) return;
    bubble_text = Object::cast_to<EnemySpeech>(get_node_internal("TextContainer/Text"));
    if (!bubble_text) {
        ERR_PRINT("TextContainer/Text 노드를 찾을 수 없습니다");
        return;
    }
    
    bubble_text->set_text("");
    set_modulate(Color(1.0, 1.0, 1.0, 0.0));
    bubble_text->set_current_character(custom_character);
    bubble_text->character_customize();
    bubble_text->add_theme_font_size_override("normal_font_size", text_size);
    
    bubble_text->connect("expression_set", Callable(this, "_on_text_expression_set"));
    bubble_text->connect("finished_speech", Callable(this, "_on_all_texts_finished"));
}

void DialogueControl::DialogueText(const Ref<Dialogues>& dialogues) {
    if (!bubble_text) {
        ERR_PRINT("bubble_text가 초기화되지 않았습니다");
        return;
    }
    global->set_battle_text_box(true);

    Callable call = Callable(this, "_on_ends_typing");
    if(bubble_text->is_connected("ends_typing", call)) bubble_text->disconnect("ends_typing", call);
    
    bubble_text->set_text("");
    bubble_text->set_queued_dialogues(dialogues);
    
    active_tween = create_tween();
    tween_in_progress = true;
    
    active_tween->tween_property(this, "modulate:a", 1, 0.1);
    active_tween->connect("finished", Callable(this, "_on_tween_finished"));
}

void DialogueControl::on_text_click_played(bool is, double duration) {
    if(is) {
        bubble_text->kill_tweens(true);
        bubble_text->emit_signal("confirm");
    }else {
        text_duration = duration;
        Callable call = Callable(this, "_on_ends_typing");
        if(bubble_text->is_connected("ends_typing", call)) bubble_text->disconnect("ends_typing", call);
        bubble_text->connect("ends_typing", call);
    }
}

void DialogueControl::_on_ends_typing() {
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(text_duration);
    timer->connect("timeout", Callable(this, "on_text_click_played"));
}

void DialogueControl::set_key(bool is) {
    bubble_text->set_process_unhandled_input(is);
}

void DialogueControl::_on_tween_finished() {
    tween_in_progress = false;
    
    if (bubble_text) {
        bubble_text->call_deferred("type_text_advanced", bubble_text->get_queued_dialogues());
    }
}

void DialogueControl::_on_all_texts_finished() {
    active_tween = create_tween();
    active_tween->tween_property(this, "modulate:a", 0, 0.1);
    
    global->set_battle_text_box(false);
    emit_signal("finished_all_texts_dialogue");
}

void DialogueControl::_on_text_expression_set(const Array& expr) {
    emit_signal("set_expression", expr);
}

void DialogueControl::set_custom_character(Character p_character) {
    custom_character = p_character;
    
    if (bubble_text) {
        bubble_text->set_current_character(custom_character);
        bubble_text->character_customize();
    }
}

Character DialogueControl::get_custom_character() const {
    return custom_character;
}

void DialogueControl::set_text_size(int size) {
    text_size = size;

    if (bubble_text) {
        bubble_text->add_theme_font_size_override("normal_font_size", text_size);
    }
}

int DialogueControl::get_text_size() {
    return text_size;
}