#include "text_box_writer.h"
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

TextBoxWriter::TextBoxWriter() {
    is_last_dialogue = false;
    current_dialogue_finished = false;
    waiting_for_confirm = false;
}

TextBoxWriter::~TextBoxWriter() {}

void TextBoxWriter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_tween_finished_extended"), &TextBoxWriter::on_tween_finished_extended);
    ClassDB::bind_method(D_METHOD("process_next_dialogue"), &TextBoxWriter::process_next_dialogue);
    ClassDB::bind_method(D_METHOD("handle_confirm_signal"), &TextBoxWriter::handle_confirm_signal);
    
    ADD_SIGNAL(MethodInfo("finished_all_texts_textbox"));
}

void TextBoxWriter::_ready() {
    connect("confirm", Callable(this, "handle_confirm_signal"));
}

void TextBoxWriter::type_text_advanced(const Ref<Dialogues>& dialogues) {
    set_typing(true);
    set_current_dialogue_index(0);
    set_queued_dialogues(dialogues);
    process_next_dialogue();
}

void TextBoxWriter::process_next_dialogue() {
    if (get_current_dialogue_index() < get_queued_dialogues()->get_dialogues().size()) {
        emit_signal("started_typing", get_current_dialogue_index());
        
        Array expressions = get_queued_dialogues()->get_dialogues_single(Dialogues::DIALOGUE_EXPRESSIONS);
        if (expressions.size() > get_current_dialogue_index()) {
            emit_signal("expression_set", expressions[get_current_dialogue_index()]);
        }
        
        TypedArray<Dialogue> dialogues_array = get_queued_dialogues()->get_dialogues();
        if(dialogues_array.size() > get_current_dialogue_index()) {
            Ref<Dialogue> current_dialogue = Object::cast_to<Dialogue>(dialogues_array[get_current_dialogue_index()]);
            if (current_dialogue.is_valid()) {
                set_pauses(current_dialogue->get_pauses());
            }
        }
        
        is_last_dialogue = (get_current_dialogue_index() == get_queued_dialogues()->get_dialogues().size() - 1);
        
        type_buffer(get_queued_dialogues(), get_current_dialogue_index());
        
        Ref<Tween> visible_tween = get_visible_tween();
        if (visible_tween.is_valid()) {
            Callable calls = Callable(this, "on_tween_finished_extended");
            if (visible_tween->is_connected("finished", calls)) visible_tween->disconnect("finished", calls);
            visible_tween->connect("finished", calls);
        }
    } else {
        set_typing(false);
    }
}

void TextBoxWriter::on_tween_finished_extended() {
    on_tween_finished_advanced();
    
    current_dialogue_finished = true;
    waiting_for_confirm = true;
}

void TextBoxWriter::handle_confirm_signal() {
    if (waiting_for_confirm && current_dialogue_finished) {
        waiting_for_confirm = false;
        current_dialogue_finished = false;

        if (is_last_dialogue) {
            emit_signal("finished_all_texts_textbox");
            return;
        }
        
        set_current_dialogue_index(get_current_dialogue_index() + 1);
        get_viewport()->set_input_as_handled();
        process_next_dialogue();
    }
}