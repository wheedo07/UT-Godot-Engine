#include "text_box_options.h"
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

TextBoxOptionWriter::TextBoxOptionWriter() {}

TextBoxOptionWriter::~TextBoxOptionWriter() {}

void TextBoxOptionWriter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("type_text_advanced", "dialogues"), &TextBoxOptionWriter::type_text_advanced);
    ClassDB::bind_method(D_METHOD("process_next_dialogue"), &TextBoxOptionWriter::process_dialogue);
    
    ADD_SIGNAL(MethodInfo("finished_typing_options"));
}

void TextBoxOptionWriter::_ready() {
    GenericTextTyper::_ready();
    set_process_unhandled_input(false);
}

void TextBoxOptionWriter::type_text_advanced(const Ref<Dialogues>& dialogues) {
    set_typing(true);
    set_queued_dialogues(dialogues);
    set_current_dialogue_index(0);
    process_dialogue();
}

void TextBoxOptionWriter::process_dialogue() {
    if(get_current_dialogue_index() < get_queued_dialogues()->get_dialogues().size()) {
        emit_signal("started_typing", get_current_dialogue_index());
        
        Array expressions = get_queued_dialogues()->get_dialogues_single(Dialogues::DIALOGUE_EXPRESSIONS);
        emit_signal("expression_set", expressions[get_current_dialogue_index()]);
        
        set_pauses(Object::cast_to<Dialogue>(get_queued_dialogues()->get_dialogues()[get_current_dialogue_index()])->get_pauses());
        
        type_buffer(get_queued_dialogues(), get_current_dialogue_index());

        Ref<Tween> visible_tween = get_visible_tween();
        if (visible_tween.is_valid()) {
            Callable calls = Callable(this, "emit_signal").bind("finished_typing_options");
            if(visible_tween->is_connected("finished", calls)) visible_tween->disconnect("finished", calls);
            visible_tween->connect("finished", calls);
        }
    }else {
        set_typing(false);
    }
}