#include "text_typer.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/input_event_action.hpp>

GenericTextTyper::GenericTextTyper() {
    click_path = NodePath();
    interval = 0.1f;
    queued_texts_handling = AWAIT_FINISH;
    typing = false;
    extra_delay = "@#$%^&+=_-~`<>\"|\\*{}()[].,!?";
    no_sound = "@#$%^&+=_-~`<>\"|\\*{}()[].,!? ";
    
    click = nullptr;
}

GenericTextTyper::~GenericTextTyper() {}

void GenericTextTyper::_bind_methods() {
    BIND_ENUM_CONSTANT(AWAIT_FINISH);
    BIND_ENUM_CONSTANT(OVERRIDE_CURRENT);
    BIND_ENUM_CONSTANT(VOID_QUEUED);
    
    ClassDB::bind_method(D_METHOD("set_click_path", "path"), &GenericTextTyper::set_click_path);
    ClassDB::bind_method(D_METHOD("get_click_path"), &GenericTextTyper::get_click_path);
    
    ClassDB::bind_method(D_METHOD("set_interval", "interval"), &GenericTextTyper::set_interval);
    ClassDB::bind_method(D_METHOD("get_interval"), &GenericTextTyper::get_interval);
    
    ClassDB::bind_method(D_METHOD("set_queued_texts_handling", "mode"), &GenericTextTyper::set_queued_texts_handling);
    ClassDB::bind_method(D_METHOD("get_queued_texts_handling"), &GenericTextTyper::get_queued_texts_handling);
    
    ClassDB::bind_method(D_METHOD("set_entire_text_bbcode", "text"), &GenericTextTyper::set_entire_text_bbcode);
    ClassDB::bind_method(D_METHOD("get_entire_text_bbcode"), &GenericTextTyper::get_entire_text_bbcode);
    
    ClassDB::bind_method(D_METHOD("kill_tweens", "complete_text"), &GenericTextTyper::kill_tweens, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("type_text", "text"), &GenericTextTyper::type_text);
    ClassDB::bind_method(D_METHOD("create_tweeners"), &GenericTextTyper::create_tweeners);
    ClassDB::bind_method(D_METHOD("_type_one_line", "line"), &GenericTextTyper::_type_one_line);
    ClassDB::bind_method(D_METHOD("play_click"), &GenericTextTyper::play_click);
    
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "click_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AudioStreamPlayer"), "set_click_path", "get_click_path");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "interval"), "set_interval", "get_interval");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "queued_texts_handling", PROPERTY_HINT_ENUM, "AWAIT_FINISH,OVERRIDE_CURRENT,VOID_QUEUED"), "set_queued_texts_handling", "get_queued_texts_handling");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "entire_text_bbcode", PROPERTY_HINT_MULTILINE_TEXT), "set_entire_text_bbcode", "get_entire_text_bbcode");
    
    ADD_SIGNAL(MethodInfo("started_typing", PropertyInfo(Variant::INT, "line")));
    ADD_SIGNAL(MethodInfo("confirm"));
    ADD_SIGNAL(MethodInfo("skip"));
    ADD_SIGNAL(MethodInfo("finished_all_texts"));
}

void GenericTextTyper::_ready() {
    if(isEditor) return;
    if(!click_path.is_empty()) click = Object::cast_to<AudioStreamPlayer>(get_node_internal(click_path));
}

void GenericTextTyper::_process(double delta) {
    if(typing) _process_typing();
}

void GenericTextTyper::_unhandled_input(const Ref<InputEvent>& event) {
    if(global->get_player_text_box() || global->get_battle_start()) {
        if(event->is_action_pressed("ui_cancel") && visible_tween.is_valid() && visible_tween->is_valid()) {
            emit_signal("skip");
            kill_tweens(true);
        }
        if(event->is_action_pressed("ui_accept") && (!visible_tween.is_valid() || !visible_tween->is_valid())) {
            emit_signal("confirm");
        }
    }
}

void GenericTextTyper::kill_tweens(bool complete_text) {
    if (pause_tween.is_valid() && pause_tween->is_valid()) {
        pause_tween->kill();
    }
    if (sound_tween.is_valid() && sound_tween->is_valid()) {
        sound_tween->kill();
    }
    if (visible_tween.is_valid() && visible_tween->is_valid()) {
        if (complete_text) {
            visible_tween->custom_step(10000.0);
        }else {
            visible_tween->kill();
        }
    }
}

void GenericTextTyper::type_text(const PackedStringArray& text) {
    typing = true;
    current_line_index = 0;
    queued_texts = text;
    line_typing = false;
}

void GenericTextTyper::create_tweeners() {
    visible_tween.unref();
    sound_tween.unref();
    visible_tween = create_tween();
    if(!sound_tween.is_valid()) sound_tween = create_tween();
}

bool GenericTextTyper::_type_one_line(const String& line) {
    set_text(entire_text_bbcode + line);
    chache_parsed_text = get_parsed_text();
    
    switch (queued_texts_handling) {
        case AWAIT_FINISH:
            if (visible_tween.is_valid() && visible_tween->is_running()) {
                return false;
            }
            break;
        case OVERRIDE_CURRENT:
            kill_tweens();
            break;
        case VOID_QUEUED:
            if (visible_tween.is_valid() && visible_tween->is_valid()) {
                return false;
            }
            break;
    }
    
    create_tweeners();
    set_visible_ratio(0.0);
    String parsed_text = get_parsed_text();
    
    visible_tween->tween_property(this, "visible_ratio", 1.0, interval * parsed_text.length());
    sound_tween->set_loops(parsed_text.length());
    sound_tween->tween_callback(Callable(this, "play_click"));
    sound_tween->tween_interval(interval);
    
    return true;
}

void GenericTextTyper::_process_typing() {
    if (current_line_index < queued_texts.size()) {
        if (!line_typing) {
            emit_signal("started_typing", current_line_index);
            if (_type_one_line(queued_texts[current_line_index])) {
                line_typing = true;
            }
        } else if (visible_tween.is_valid() && !visible_tween->is_running()) {
            line_typing = false;
            current_line_index++;
        }
    } else {
        emit_signal("finished_all_texts");
        typing = false;
    }
}

void GenericTextTyper::play_click() {
    if (get_visible_characters() >= chache_parsed_text.length() || get_visible_characters() < 0) return;
    
    char32_t current_char = chache_parsed_text[get_visible_characters()];
    String char_str;
    char_str += current_char;
    
    if (extra_delay.find(char_str) != -1) {
        if (!visible_tween.is_valid() || !visible_tween->is_valid() || !sound_tween.is_valid() || !sound_tween->is_valid()) {
            return;
        }
        
        sound_tween->pause();
        visible_tween->pause();
        
        pause_tween = create_tween();
        pause_tween->set_parallel(true);
        pause_tween->tween_callback(Callable(visible_tween.ptr(), "play"))->set_delay(interval);
        pause_tween->tween_callback(Callable(sound_tween.ptr(), "play"))->set_delay(interval);
        return;
    }
    
    if (no_sound.find(char_str) != -1) {
        return;
    }
    
    if (click) {
        click->play();
    }
}

void GenericTextTyper::set_click_path(const NodePath& p_path) {
    click_path = p_path;
}

NodePath GenericTextTyper::get_click_path() const {
    return click_path;
}

void GenericTextTyper::set_interval(float p_interval) {
    interval = p_interval;
}

float GenericTextTyper::get_interval() const {
    return interval;
}

void GenericTextTyper::set_queued_texts_handling(TextQueueMode p_mode) {
    queued_texts_handling = p_mode;
}

GenericTextTyper::TextQueueMode GenericTextTyper::get_queued_texts_handling() const {
    return queued_texts_handling;
}

void GenericTextTyper::set_entire_text_bbcode(const String& p_text) {
    entire_text_bbcode = p_text;
}

String GenericTextTyper::get_entire_text_bbcode() const {
    return entire_text_bbcode;
}

void GenericTextTyper::set_typing(bool value) {
    typing = value;
}

bool GenericTextTyper::get_typing() {
    return typing;
}

void GenericTextTyper::set_chache_parsed_text(String value) {
    chache_parsed_text = value;
}

String GenericTextTyper::get_chache_parsed_text() {
    return chache_parsed_text;
}

void GenericTextTyper::set_visible_tween(Ref<Tween> value) {
    visible_tween = value;
}

Ref<Tween> GenericTextTyper::get_visible_tween() {
    return visible_tween;
}

void GenericTextTyper::set_sound_tween(Ref<Tween> value) {
    sound_tween = value;
}

Ref<Tween> GenericTextTyper::get_sound_tween() {
    return sound_tween;
}

void GenericTextTyper::set_pause_tween(Ref<Tween> value) {
    pause_tween = value;
}

Ref<Tween> GenericTextTyper::get_pause_tween() {
    return pause_tween;
}

void GenericTextTyper::set_extra_delay(String value) {
    extra_delay = value;
}

String GenericTextTyper::get_extra_delay() {
    return extra_delay;
}

void GenericTextTyper::set_no_sound(String value) {
    no_sound = value;
}

String GenericTextTyper::get_no_sound() {
    return no_sound;
}

void GenericTextTyper::set_click(AudioStreamPlayer* value) {
    click = value;
}

AudioStreamPlayer* GenericTextTyper::get_click() {
    return click;
}