#include "dialogue.h"
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

Dialogue::Dialogue() {
    dialog_text = "test";
    // 기본 출력 속도
    dialog_speed = 0.08;
    dialog_expressions = Array::make( 0, 0 );
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
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dialog_expressions"), "set_dialog_expressions", "get_dialog_expressions");
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

void Dialogue::set_dialog_expressions(const TypedArray<int>& p_expressions) {
    dialog_expressions.clear();
    for (int i = 0; i < p_expressions.size(); i++) {
        dialog_expressions.push_back(p_expressions[i]);
    }
}

TypedArray<int> Dialogue::get_dialog_expressions() const {
    TypedArray<int> arr;
    for (int i = 0; i < dialog_expressions.size(); i++) {
        int expression = dialog_expressions[i];
        arr.push_back(expression);
    }
    return arr;
}

void Dialogue::set_pauses(const TypedArray<DialoguePause>& p_pauses) {
    pauses.clear();
    for (int i = 0; i < p_pauses.size(); i++) {
        pauses.push_back(p_pauses[i]);
    }
}

TypedArray<DialoguePause> Dialogue::get_pauses() const {
    TypedArray<DialoguePause> arr;
    for (int i = 0; i < pauses.size(); i++) {
        Ref<DialoguePause> pause = pauses[i];
        arr.push_back(pause);
    }
    return arr;
}