#include "dialogues.h"
#include <godot_cpp/variant/utility_functions.hpp>
using namespace godot;

Dialogues::Dialogues() {}

Dialogues::~Dialogues() {}

void Dialogues::_bind_methods() {
    BIND_ENUM_CONSTANT(DIALOGUE_TEXT);
    BIND_ENUM_CONSTANT(DIALOGUE_EXPRESSIONS);
    BIND_ENUM_CONSTANT(DIALOGUE_EXPRESSION_HEAD);
    BIND_ENUM_CONSTANT(DIALOGUE_EXPRESSION_BODY);
    BIND_ENUM_CONSTANT(DIALOGUE_PAUSES);
    BIND_ENUM_CONSTANT(DIALOGUE_SPEED);

    ClassDB::bind_method(D_METHOD("get_dialogues_single", "dialog_type"), &Dialogues::get_dialogues_single);
    ClassDB::bind_method(D_METHOD("from", "text"), &Dialogues::from);
    ClassDB::bind_method(D_METHOD("set_expressions", "heads", "bodies"), &Dialogues::set_expressions, DEFVAL(Array()));
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
                case DIALOGUE_EXPRESSION_HEAD:
                    arr.push_back(dialog->get_dialog_expressions()[0]);
                    break;
                case DIALOGUE_EXPRESSION_BODY:
                    arr.push_back(dialog->get_dialog_expressions()[1]);
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

Dialogues* Dialogues::from(const PackedStringArray& text) {
    for(String t : text) {
        Ref<Dialogue> dialog = memnew(Dialogue);
        dialog->set_dialog_text(t);
        dialogues.push_back(dialog);
    }
    return this;
}

Dialogues* Dialogues::set_expressions(const Array& heads, const Array& bodies) {
    for (int i = 0; i < dialogues.size(); i++) {
        Ref<Dialogue> dialog = dialogues[i];
        if (dialog.is_valid()) {
            Array expressions;
            expressions.resize(2);
            expressions[0] = i < heads.size() ? heads[i] : Variant(0);
            expressions[1] = i < bodies.size() ? bodies[i] : Variant(0);
            dialog->set_dialog_expressions(expressions);
        }
    }
    return this;
}

Dialogues* Dialogues::set_speed(const Array& speeds) {
    for (int i = 0; i < dialogues.size(); i++) {
        Ref<Dialogue> dialog = dialogues[i];
        if (dialog.is_valid()) {
            dialog->set_dialog_speed(i < speeds.size() ? (float)speeds[i] : 0.1f);
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