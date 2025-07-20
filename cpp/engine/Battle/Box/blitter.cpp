#include "blitter.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

Blitter::Blitter() {
    container = nullptr;
    default_volume = 0;
    current_text_index = 0;
    is_typing = false;
}

Blitter::~Blitter() {}

void Blitter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("blitter", "turn"), &Blitter::blitter);
    ClassDB::bind_method(D_METHOD("blittertext", "alltext"), &Blitter::blittertext);
    ClassDB::bind_method(D_METHOD("type_text", "text"), &Blitter::type_text, DEFVAL("Blank"));
    
    ClassDB::bind_method(D_METHOD("_on_confirm_pressed"), &Blitter::_on_confirm_pressed);
    ClassDB::bind_method(D_METHOD("_continue_typing_next_line"), &Blitter::_continue_typing_next_line);
    
    ClassDB::bind_method(D_METHOD("get_flavour_texts"), &Blitter::get_flavour_texts);
    ClassDB::bind_method(D_METHOD("set_flavour_texts", "texts"), &Blitter::set_flavour_texts);
    
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "flavour_texts"), "set_flavour_texts", "get_flavour_texts");
}

void Blitter::_ready() {
    if(Engine::get_singleton()->is_editor_hint()) return;
    GenericTextTyper::_ready();
    container = Object::cast_to<MarginContainer>(get_parent());
    
    default_volume = get_click()->get_volume_db();
    
    connect("confirm", Callable(this, "_on_confirm_pressed"));
}

void Blitter::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) return;
    bool is_visible = container->is_visible();
    
    AudioStreamPlayer* audio = get_click(); 
    if (audio) {
        audio->set_volume_db(is_visible ? default_volume : -80.0f);
    }
}

void Blitter::blitter(int turn) {
    int index = turn;
    if(turn >= MAX<int>(1, flavour_texts.size())) index = flavour_texts.size() - 1;
    type_text(flavour_texts[index]);
}

void Blitter::blittertext(const PackedStringArray& alltext) {
    type_text(alltext);
}

void Blitter::type_text(const Variant text) {
    get_viewport()->set_input_as_handled();
    
    if(text.get_type() != Variant::ARRAY && text.get_type() != Variant::PACKED_STRING_ARRAY) {
        current_texts.clear();
        current_texts.append(text);
    } else {
        current_texts = TypedArray<String>(text);
    }
    
    current_text_index = 0;
    is_typing = true;
    
    if (current_texts.size() > 0) {
        emit_signal("started_typing", 0);
        _type_one_line(current_texts[0]);
    } else {
        emit_signal("finished_all_texts");
    }
}

void Blitter::_on_confirm_pressed() {
    _continue_typing_next_line();
}

void Blitter::_continue_typing_next_line() {
    current_text_index++;
    
    if (current_text_index < current_texts.size()) {
        emit_signal("started_typing", current_text_index);
        _type_one_line(current_texts[current_text_index]);
    } else {
        is_typing = false;
        emit_signal("finished_all_texts");
    }
}

PackedStringArray Blitter::get_flavour_texts() const {
    return flavour_texts;
}

void Blitter::set_flavour_texts(const PackedStringArray& texts) {
    flavour_texts = texts;
}