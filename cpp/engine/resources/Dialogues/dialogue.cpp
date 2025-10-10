#include "dialogue.h"
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

Dialogue::Dialogue() {
    dialog_text = "test";
    dialog_speed = 0.08;
    dialog_expressions = { -1 };
}

Dialogue::~Dialogue() {}

void Dialogue::_bind_methods() {
    BIND_ENUM_CONSTANT(CHAR);
    BIND_ENUM_CONSTANT(INDEX_LIST);

    ClassDB::bind_method(D_METHOD("set_dialog_text", "text"), &Dialogue::set_dialog_text);
    ClassDB::bind_method(D_METHOD("get_dialog_text"), &Dialogue::get_dialog_text);
    ClassDB::bind_method(D_METHOD("set_dialog_speed", "speed"), &Dialogue::set_dialog_speed);
    ClassDB::bind_method(D_METHOD("get_dialog_speed"), &Dialogue::get_dialog_speed);
    ClassDB::bind_method(D_METHOD("set_dialog_expressions", "expressions"), &Dialogue::set_dialog_expressions);
    ClassDB::bind_method(D_METHOD("get_dialog_expressions"), &Dialogue::get_dialog_expressions);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "dialog_text", PROPERTY_HINT_MULTILINE_TEXT), "set_dialog_text", "get_dialog_text");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dialog_speed"), "set_dialog_speed", "get_dialog_speed");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dialog_expressions", PROPERTY_HINT_ARRAY_TYPE, String::num(Variant::INT) + ":"), "set_dialog_expressions", "get_dialog_expressions");
}

void Dialogue::set_dialog_text(const String& p_text) {
    dialog_text = p_text;
}

String Dialogue::get_dialog_text() const {
    return dialog_text;
}

void Dialogue::set_dialog_speed(float p_speed) {
    dialog_speed = p_speed;
}

float Dialogue::get_dialog_speed() const {
    return dialog_speed;
}

void Dialogue::set_dialog_expressions(Array p_expressions) {
    dialog_expressions = p_expressions;
}

Array Dialogue::get_dialog_expressions() const {
    return dialog_expressions;
}

void Dialogue::set_pauses(const TypedArray<DialoguePause>& p_pauses) {
    pauses = p_pauses;
}

TypedArray<DialoguePause> Dialogue::get_pauses() const {
    return pauses;
}