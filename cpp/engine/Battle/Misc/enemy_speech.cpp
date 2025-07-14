#include "enemy_speech.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

EnemySpeech::EnemySpeech() {
    current_character = DEFAULT;
}

EnemySpeech::~EnemySpeech() {}

void EnemySpeech::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished_speech"));
    ADD_SIGNAL(MethodInfo("ends_typing"));
    ClassDB::bind_method(D_METHOD("character_customize"), &EnemySpeech::character_customize);
    ClassDB::bind_method(D_METHOD("handle_confirm_signal"), &EnemySpeech::handle_confirm_signal);
    ClassDB::bind_method(D_METHOD("on_tween_finished_extended"), &EnemySpeech::on_tween_finished_extended);

    ClassDB::bind_method(D_METHOD("set_current_character", "character"), &EnemySpeech::set_current_character);
    ClassDB::bind_method(D_METHOD("get_current_character"), &EnemySpeech::get_current_character);
    bind_enum(get_class_static(), "set_current_character", "get_current_character");
}

void EnemySpeech::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) return;
    GenericTextTyper::_ready();
    connect("confirm", Callable(this, "handle_confirm_signal"));
}

void EnemySpeech::character_customize() {
    ResourceLoader* loader = ResourceLoader::get_singleton();
    set_click(Object::cast_to<AudioStreamPlayer>(get_node_internal("Sounds/"+String(get_character_name()[Variant(current_character)]))));

    // 캐릭터 추가시 추가
    switch (current_character) {
        case SANS:
            add_theme_font_override("normal_font", loader->load("res://Text/Fonts/character/sans.ttf"));
            break;
        case GASTER:
        case GASTER_TEXT:
            set_click(Object::cast_to<AudioStreamPlayer>(get_node_internal("Sounds/Gaster")));
            if(current_character == GASTER_TEXT) add_theme_font_override("normal_font", loader->load("res://Text/Fonts/character/gaster.otf"));
            break;
        case TEMMIE:
            set_click(Object::cast_to<AudioStreamPlayer>(get_node_internal("Sounds/Generic")));
            set_entire_text_bbcode("[shake amp=6]");
            break;
        default:
            break;
    }
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

void EnemySpeech::set_current_character(Character p_character) {
    current_character = p_character;
    character_customize();
}

Character EnemySpeech::get_current_character() const {
    return current_character;
}