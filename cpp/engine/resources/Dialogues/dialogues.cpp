#include "dialogues.h"
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

Dialogues::Dialogues() {}

Dialogues::~Dialogues() {}

void Dialogues::_bind_methods() {
    BIND_ENUM_CONSTANT(DIALOGUE_TEXT);
    BIND_ENUM_CONSTANT(DIALOGUE_EXPRESSIONS);
    BIND_ENUM_CONSTANT(DIALOGUE_PAUSES);
    BIND_ENUM_CONSTANT(DIALOGUE_SPEED);

    ClassDB::bind_method(D_METHOD("get_dialogues_single", "dialog_type"), &Dialogues::get_dialogues_single);
    ClassDB::bind_method(D_METHOD("from", "text"), &Dialogues::from);
    ClassDB::bind_method(D_METHOD("set_expressions", "expressions"), &Dialogues::set_expressions);
    ClassDB::bind_method(D_METHOD("set_pauses", "pauses"), &Dialogues::set_pauses);
    ClassDB::bind_method(D_METHOD("set_speed", "speeds"), &Dialogues::set_speed);
    ClassDB::bind_method(D_METHOD("get_dialogues"), &Dialogues::get_dialogues);
    ClassDB::bind_method(D_METHOD("set_dialogues", "p_dialogues"), &Dialogues::set_dialogues);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dialogues", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":Dialogue")
    ,"set_dialogues", "get_dialogues");
}

Array Dialogues::get_dialogues_single(DialogueType dialog_type) const {
    Array arr;
    for (int i = 0; i < dialogues.size(); i++) {
        Ref<Dialogue> dialog = dialogues[i];
        if (dialog.is_valid()) {
            switch (dialog_type) {
                case DIALOGUE_TEXT:
                    arr.push_back(dialog->get_dialog_text());
                    break;
                case DIALOGUE_EXPRESSIONS:
                    arr.push_back(dialog->get_dialog_expressions());
                    break;
                case DIALOGUE_SPEED:
                    arr.push_back(dialog->get_dialog_speed());
                    break;
                case DIALOGUE_PAUSES:
                    Array pauses = dialog->get_pauses();
                    if (!pauses.is_empty()) {
                        arr.push_back(pauses);
                    }
                    break;
            }
        }
    }
    return arr;
}

Ref<Dialogues> Dialogues::from(const PackedStringArray& text) {
    for(String t : text) {
        Ref<Dialogue> dialog = memnew(Dialogue);
        dialog->set_dialog_text(t);
        dialogues.push_back(dialog);
    }
    return this;
}

Ref<Dialogues> Dialogues::set_expressions(Array expressions) {
    for(int i=0; i < dialogues.size(); i++) {
        Ref<Dialogue> dialog = dialogues[i];
        if(dialog.is_valid() && i < expressions.size()) {
            if(expressions[i].get_type() == Variant::ARRAY) {
                dialog->set_dialog_expressions(expressions[i]);
            }else {
                if(expressions[i].get_type() != Variant::INT && expressions[i].get_type() != Variant::FLOAT) continue;
                int expr_index = expressions[i];
                dialog->set_dialog_expressions(Array::make(expr_index));
            }
        }
    }
    return this;
}

Ref<Dialogues> Dialogues::set_speed(const Array& speeds) {
    for(int i=0; i < dialogues.size(); i++) {
        Ref<Dialogue> dialog = dialogues[i];
        if(dialog.is_valid()) {
            float speed = 0.08f;
            if(i < speeds.size() && speeds[i].get_type() == Variant::FLOAT) speed = speeds[i];
            dialog->set_dialog_speed(speed);
        }
    }
    return this;
}

Ref<Dialogues> Dialogues::set_pauses(Array pauses) {
    for(int i = 0; i < dialogues.size(); i++) {
        Ref<Dialogue> dialog = dialogues[i];
        if (dialog.is_valid() && i < pauses.size()) {
            Array pause_array;
            
            if(pauses[i].get_type() == Variant::ARRAY) {
                Array input_array = pauses[i];
                for(int j = 0; j < input_array.size(); j++) {
                    Ref<DialoguePause> pause_res = memnew(DialoguePause);
                    
                    if(input_array[j].get_type() == Variant::DICTIONARY) {
                        Dictionary pause_dict = input_array[j];
                        pause_res->set_pause_index(pause_dict.get("index", 0));
                        pause_res->set_pause_duration(pause_dict.get("duration", 0.5));
                    } else {
                        pause_res->set_pause_index((int)input_array[j]);
                        pause_res->set_pause_duration(0.5);
                    }
                    pause_array.push_back(pause_res);
                }
            } else if(pauses[i].get_type() == Variant::DICTIONARY) {
                Ref<DialoguePause> pause_res = memnew(DialoguePause);
                Dictionary pause_dict = pauses[i];
                pause_res->set_pause_index(pause_dict.get("index", 0));
                pause_res->set_pause_duration(pause_dict.get("duration", 0.5));
                pause_array.push_back(pause_res);
            } else {
                ERR_PRINT("문제 발생: pauses 배열의 요소는 Dictionary 또는 Array[Dictionary]여야 합니다.");
                continue;
            }
            
            dialog->set_pauses(pause_array);
        }
    }
    return this;
}

TypedArray<Dialogue> Dialogues::get_dialogues() {
    return dialogues;
}

void Dialogues::set_dialogues(TypedArray<Dialogue> p_dialogues) {
    dialogues = p_dialogues;
}