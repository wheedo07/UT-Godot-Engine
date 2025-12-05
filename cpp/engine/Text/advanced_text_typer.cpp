#include "advanced_text_typer.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

AdvancedTextTyper::AdvancedTextTyper() {
    type_buffer_text_complete = false;
}

AdvancedTextTyper::~AdvancedTextTyper() {}

void AdvancedTextTyper::_bind_methods() {
    ClassDB::bind_method(D_METHOD("type_text_advanced", "dialogues"), &AdvancedTextTyper::type_text_advanced);
    ClassDB::bind_method(D_METHOD("get_queued_dialogues"), &AdvancedTextTyper::get_queued_dialogues);

    ClassDB::bind_method(D_METHOD("_playclick_advanced"), &AdvancedTextTyper::_playclick_advanced);
    ClassDB::bind_method(D_METHOD("_on_confirm_advanced"), &AdvancedTextTyper::_on_confirm_advanced);
    ClassDB::bind_method(D_METHOD("_on_tween_finished_advanced"), &AdvancedTextTyper::_on_tween_finished_advanced);
    
    ClassDB::bind_method(D_METHOD("get_current_dialogue_index"), &AdvancedTextTyper::get_current_dialogue_index);
    ClassDB::bind_method(D_METHOD("set_current_dialogue_index", "p_index"), &AdvancedTextTyper::set_current_dialogue_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_dialogue_index", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_current_dialogue_index", "get_current_dialogue_index");

    ADD_SIGNAL(MethodInfo("advanced_finished_all_texts"));
    ADD_SIGNAL(MethodInfo("click_played", PropertyInfo(Variant::INT, "index"), PropertyInfo(Variant::INT, "max")));
    ADD_SIGNAL(MethodInfo("expression_set", PropertyInfo(Variant::ARRAY, "expr")));
}

void AdvancedTextTyper::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) return;
    GenericTextTyper::_ready();
    connect("confirm", Callable(this, "_on_confirm_advanced"));
}

void AdvancedTextTyper::type_text(const PackedStringArray& text) {
    ERR_PRINT("여기서는 사용하지 마세요");
}

void AdvancedTextTyper::type_text_advanced(const Ref<Dialogues>& dialogues) {
    current_dialogue_index = 0;
    type_buffer_text_complete = false;
    queued_dialogues = dialogues;
    expressions = dialogues->get_dialogues_single(Dialogues::DIALOGUE_EXPRESSIONS);
    process_next_buffer();
}

void AdvancedTextTyper::type_buffer(const Ref<Dialogues>& dialogues, int index) {
    text_size_counter = 0;
    pauses_done_counter = 0;
    Array txts = dialogues->get_dialogues_single(Dialogues::DIALOGUE_TEXT);
    Array speeds = dialogues->get_dialogues_single(Dialogues::DIALOGUE_SPEED);
    set_interval(speeds[index]);
    type_buffer_text(tr(txts[index]));
}

void AdvancedTextTyper::process_next_buffer() {
    if (current_dialogue_index < queued_dialogues->get_dialogues().size()) {
        pauses = Object::cast_to<Dialogue>(queued_dialogues->get_dialogues()[current_dialogue_index])->get_pauses();
        emit_signal("started_typing", current_dialogue_index);
        emit_signal("expression_set", expressions[current_dialogue_index]);
        type_buffer(queued_dialogues, current_dialogue_index);
    } else {
        emit_signal("advanced_finished_all_texts");
    }
}

void AdvancedTextTyper::type_buffer_text(const String& txt) {
    set_text(get_entire_text_bbcode() + txt);
    set_chache_parsed_text(get_parsed_text());
    create_tweeners();
    set_visible_ratio(0.0);
    String parsed_text = get_parsed_text();

    get_visible_tween()->tween_property(this, "visible_ratio", 1.0, get_interval() * parsed_text.length());
    get_sound_tween()->set_loops(parsed_text.length());
    get_sound_tween()->tween_callback(Callable(this, "_playclick_advanced"));
    get_sound_tween()->tween_interval(get_interval());
    
    if(get_visible_tween()->is_connected("finished", Callable(this, "_on_tween_finished_advanced"))) {
        get_visible_tween()->disconnect("finished", Callable(this, "_on_tween_finished_advanced"));
    }
    get_visible_tween()->connect("finished", Callable(this, "_on_tween_finished_advanced"));
}

void AdvancedTextTyper::_on_tween_finished_advanced() {
    type_buffer_text_complete = true;
}

void AdvancedTextTyper::_on_confirm_advanced() {
    if (type_buffer_text_complete) {
        type_buffer_text_complete = false;
        current_dialogue_index++;
        process_next_buffer();
    }
}

int AdvancedTextTyper::get_actual_character_count(const String& text, int byte_position) {
    int char_count = 0;
    for(int i=0; i < byte_position && i < text.length(); i++) {
        char_count++;
    }
    return char_count;
}

void AdvancedTextTyper::_playclick_advanced() {
    text_size_counter++;
    String cached_text = get_chache_parsed_text();
    int visible_chars = get_visible_characters();

    if (visible_chars < 0 || visible_chars >= cached_text.length()) return;
    
    int actual_char_position = get_actual_character_count(cached_text, visible_chars);
    char32_t current_char = cached_text[visible_chars];
    String char_str = "";
    char_str += current_char;

    bool should_pause = false;
    float pause_duration = get_interval();
    
    if (pauses.size() > pauses_done_counter && 
        actual_char_position == Object::cast_to<DialoguePause>(pauses[pauses_done_counter])->get_pause_index()) {
        should_pause = true;
        pause_duration += Object::cast_to<DialoguePause>(pauses[pauses_done_counter])->get_pause_duration();
        pauses_done_counter++;
    }
    
    if (get_extra_delay().find(char_str) != -1 || should_pause) {
        if (!get_visible_tween()->is_running() || !get_sound_tween()->is_running()) {
            return;
        }
        get_sound_tween()->pause();
        get_visible_tween()->pause();
        set_pause_tween(create_tween());
        get_pause_tween()->set_parallel(true);
        
        get_pause_tween()->tween_callback(Callable(get_visible_tween().ptr(), "play"))->set_delay(pause_duration);
        get_pause_tween()->tween_callback(Callable(get_sound_tween().ptr(), "play"))->set_delay(pause_duration);
        
        if (get_no_sound().find(char_str) != -1) {
            return;
        }
    }
    
    if (get_no_sound().find(char_str) == -1 && get_click()) {
        get_click()->play();
    }
    emit_signal("click_played", text_size_counter, cached_text.length());
}

int AdvancedTextTyper::get_current_dialogue_index() const {
    return current_dialogue_index;
}

void AdvancedTextTyper::set_current_dialogue_index(int p_index) {
    current_dialogue_index = p_index;
}

Ref<Dialogues> AdvancedTextTyper::get_queued_dialogues() const {
    return queued_dialogues;
}

void AdvancedTextTyper::set_queued_dialogues(Ref<Dialogues> value) {
    queued_dialogues = value;
}

void AdvancedTextTyper::set_pauses(TypedArray<DialoguePause> value) {
    pauses = value;
}