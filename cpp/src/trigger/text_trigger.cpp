#include "text_trigger.h"
#include "env.h"

TextTrigger::TextTrigger() {
    current_character = Character::DEFAULT;
}

TextTrigger::~TextTrigger() {}

void TextTrigger::_bind_methods() {
    ADD_SIGNAL(MethodInfo("dialogue_finished"));
    ClassDB::bind_method(D_METHOD("_on_interact_text"), &TextTrigger::_on_interact_text);

    ClassDB::bind_method(D_METHOD("set_text", "value"), &TextTrigger::set_text);
    ClassDB::bind_method(D_METHOD("get_text"), &TextTrigger::get_text);
    ClassDB::bind_method(D_METHOD("set_current_character", "value"), &TextTrigger::set_current_character);
    ClassDB::bind_method(D_METHOD("get_current_character"), &TextTrigger::get_current_character);
    bind_enum(get_class_static(), "set_current_character", "get_current_character");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_text", "get_text");
}

void TextTrigger::_on_interact_text() {
    Ref<PackedScene> file = ResourceLoader::get_singleton()->load("res://Overworld/text_box.tscn");
    TextBox* textbox = Object::cast_to<TextBox>(file->instantiate());
    global->get_scene_container()->get_current_scene()->add_child(textbox);

    Ref<Dialogues> dia = sys->dia(); 
    dia->from(text);
    if(current_character != Character::DEFAULT) {
        textbox->character(true, current_character, dia);
    }else textbox->generic(dia);
    textbox->connect("dialogue_finished", Callable(this, "emit_signal").bind("dialogue_finished"));
}

void TextTrigger::set_text(PackedStringArray value) {
    text = value;
}

PackedStringArray TextTrigger::get_text() {
    return text;
}

void TextTrigger::set_current_character(Character value) {
    current_character = value;
}

Character TextTrigger::get_current_character() const {
    return current_character;
}