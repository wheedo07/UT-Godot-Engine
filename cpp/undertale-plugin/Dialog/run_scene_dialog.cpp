#include "run_scene_dialog.h"
using namespace godot;

void UTRunSceneDialog::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_layers", "layers"), &UTRunSceneDialog::set_layers);
    ClassDB::bind_method(D_METHOD("get_selected_layer"), &UTRunSceneDialog::get_selected_layer);
}

void UTRunSceneDialog::_ready() {
    set_title("Select Layer");
    set_ok_button_text("Run");
    set_cancel_button_text("Cancel");
}

void UTRunSceneDialog::_ensure_ui() {
    if(layer_list) return;
    layer_list = memnew(ItemList);
    layer_list->set_select_mode(ItemList::SELECT_SINGLE);
    layer_list->set_custom_minimum_size(Vector2(320, 200));
    add_child(layer_list);
}

void UTRunSceneDialog::set_layers(PackedStringArray layers) {
    _ensure_ui();
    ERR_FAIL_NULL(layer_list);

    layer_list->clear();
    for(int i=0; i < layers.size(); i++) {
        StringName layer_id = layers[i];
        int index = layer_list->add_item(String(layer_id));
        layer_list->set_item_metadata(index, layer_id);
    }

    if(layer_list->get_item_count() > 0) {
        layer_list->select(0);
    }
}

StringName UTRunSceneDialog::get_selected_layer() {
    ERR_FAIL_NULL_V(layer_list, StringName());

    PackedInt32Array selected = layer_list->get_selected_items();
    if(selected.is_empty()) return StringName();

    Variant metadata = layer_list->get_item_metadata(selected[0]);
    if(metadata.get_type() != Variant::STRING_NAME) return StringName();
    return metadata;
}