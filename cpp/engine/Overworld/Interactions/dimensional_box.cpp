#include "dimensional_box.h"
#include "env.h"

DimensionalBox::DimensionalBox() {}

DimensionalBox::~DimensionalBox() {}

void DimensionalBox::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_interact_box"), &DimensionalBox::_on_interact_box);
    ClassDB::bind_method(D_METHOD("_on_textbox_select", "option"), &DimensionalBox::_on_textbox_select);
}

void DimensionalBox::_ready() {
    if(isEditor) return;
    ResourceLoader* loader = ResourceLoader::get_singleton();
    text_scene = loader->load("res://Overworld/text_box.tscn");
    ui_scene = loader->load("res://Overworld/ui_dimensionalBox.tscn");
}

void DimensionalBox::_on_interact_box() {
    TextBox* textbox = Object::cast_to<TextBox>(text_scene->instantiate());
    global->get_scene_container()->get_current_scene()->add_child(textbox);
    textbox->connect("selected_option", Callable(this, "_on_textbox_select"), CONNECT_ONE_SHOT);

    Ref<Dialogues> dialogues = memnew(Dialogues);
    dialogues->from({ tr("UT_USE_BOX") });
    textbox->generic(dialogues, { tr("UT_YES"), tr("UT_NO") });
}

void DimensionalBox::_on_textbox_select(int option) {
    if(option == 0) {
        global->get_scene_container()->get_current_scene()->call_deferred("add_child", ui_scene->instantiate());
    }
}