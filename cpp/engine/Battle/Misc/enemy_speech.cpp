#include "enemy_speech.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/viewport.hpp>

EnemySpeech::EnemySpeech() {
    current_character = "DEFAULT";
}

EnemySpeech::~EnemySpeech() {}

void EnemySpeech::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished_speech"));
    ADD_SIGNAL(MethodInfo("ends_typing"));
    ClassDB::bind_method(D_METHOD("character_customize"), &EnemySpeech::character_customize);
    ClassDB::bind_method(D_METHOD("handle_confirm_signal"), &EnemySpeech::handle_confirm_signal);
    ClassDB::bind_method(D_METHOD("on_tween_finished_extended"), &EnemySpeech::on_tween_finished_extended);
}

void EnemySpeech::_ready() {
    if(isEditor) return;
    GenericTextTyper::_ready();
    connect("confirm", Callable(this, "handle_confirm_signal"));
}

void EnemySpeech::character_customize() {
    CharacterSetting* setting = stagehand->get_character(current_character);
    if(!setting) return;

    if(!setting->get_font().is_null()) {
        add_theme_font_override("normal_font", setting->get_font());
        add_theme_font_override("bold_font", setting->get_font());
        add_theme_font_override("italics_font", setting->get_font());
        add_theme_font_override("bold_italics_font", setting->get_font());
        add_theme_font_override("mono_font", setting->get_font());
    }
    
    float font_size = setting->get_text_size();
    add_theme_font_size_override("normal_font_size", font_size);
    add_theme_font_size_override("bold_font_size", font_size);
    add_theme_font_size_override("italics_font_size", font_size);
    add_theme_font_size_override("bold_italics_font_size", font_size);
    add_theme_font_size_override("mono_font_size", font_size);

    set_click(setting);
    set_extra_delay(setting->get_extra_delay());
    set_no_sound(setting->get_no_sound());
    set_entire_text_bbcode(setting->get_entire_text_bbcode());
}

void EnemySpeech::type_text_advanced(const Ref<Dialogues>& dialogues) {
    if (!dialogues.is_valid()) {
        ERR_PRINT("유효하지 않은 dialogues 객체");
        return;
    }
    set_typing(true);
    set_current_dialogue_index(0);
    set_queued_dialogues(dialogues);
    process_next_dialogue();
}

void EnemySpeech::process_next_dialogue() {
    if(!get_queued_dialogues().is_valid()) return;
    
    int current_index = get_current_dialogue_index();
    if (current_index < get_queued_dialogues()->get_dialogues().size()) {
        emit_signal("started_typing", current_index);
        
        Array expressions = get_queued_dialogues()->get_dialogues_single(Dialogues::DIALOGUE_EXPRESSIONS);
        if (expressions.size() > current_index) {
            emit_signal("expression_set", expressions[current_index]);
        }
        Array speed = get_queued_dialogues()->get_dialogues_single(Dialogues::DIALOGUE_SPEED);
        if (speed.size() > current_index) set_interval(speed[current_index]);
        
        TypedArray<Dialogue> dialogues_array = get_queued_dialogues()->get_dialogues();
        if (dialogues_array.size() > current_index) {
            Ref<Dialogue> current_dialogue = dialogues_array[current_index];
            if (current_dialogue.is_valid()) {
                set_pauses(current_dialogue->get_pauses());
            }
        }
        
        type_buffer(get_queued_dialogues(), current_index);
        
        Ref<Tween> visible_tween = get_visible_tween();
        if (visible_tween.is_valid()) {
            Callable calls = Callable(this, "emit_signal").bind("ends_typing");
            if(visible_tween->is_connected("finished", calls)) visible_tween->disconnect("finished", calls);
            visible_tween->connect("finished", calls);
        }
    } else {
        emit_signal("finished_speech");
        set_typing(false);
    }
}

void EnemySpeech::on_tween_finished_extended() {
    emit_signal("ends_typing");
}

void EnemySpeech::handle_confirm_signal() {
    if(!type_buffer_text_complete) return;
    set_current_dialogue_index(get_current_dialogue_index() + 1);
    get_viewport()->set_input_as_handled();
    type_buffer_text_complete = false;

    process_next_dialogue();
}

void EnemySpeech::set_current_character(String p_character) {
    current_character = p_character;
    if(isEditor) return;
    character_customize();
}