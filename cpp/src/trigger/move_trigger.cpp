#include "move_trigger.h"
#include "env.h"

MoveTrigger::MoveTrigger() {}

MoveTrigger::~MoveTrigger() {}

void MoveTrigger::_bind_methods() {
    ADD_SIGNAL(MethodInfo("dialogue_finished"));
    ClassDB::bind_method(D_METHOD("_on_interact_move"), &MoveTrigger::_on_interact_move);
    ClassDB::bind_method(D_METHOD("finished_interact_move"), &MoveTrigger::finished_interact_move);

    ClassDB::bind_method(D_METHOD("set_text", "value"), &MoveTrigger::set_text);
    ClassDB::bind_method(D_METHOD("get_text"), &MoveTrigger::get_text);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "text", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_text", "get_text");
}

void MoveTrigger::_on_interact_move() {
    Ref<PackedScene> file = ResourceLoader::get_singleton()->load("res://Overworld/text_box.tscn");
    TextBox* textbox = Object::cast_to<TextBox>(file->instantiate());
    global->get_scene_container()->get_current_scene()->add_child(textbox);

    textbox->generic(sys->dia()->from(text));
    textbox->connect("dialogue_finished", Callable(this, "finished_interact_move"));
}

void MoveTrigger::finished_interact_move() {
    global->set_player_can_move(true);
    emit_signal("dialogue_finished");
}

void MoveTrigger::set_text(PackedStringArray value) {
    text = value;
}

PackedStringArray MoveTrigger::get_text() {
    return text;
}