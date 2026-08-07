#include "root.h"
using namespace godot;

void UTGERoot::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_layer", "layer_id"), &UTGERoot::get_layer, DEFVAL("main"));
}

void UTGERoot::_ready() {
    TypedArray<Node> children = get_children();
    for(int i=0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        if(child->is_class("UTGELayer")) {
            UTGELayer *layer = Object::cast_to<UTGELayer>(child);
            layers[layer->get_layer_id()] = layer;
        }
    }
}

UTGELayer *UTGERoot::get_layer(StringName layer_id) {
    UTGELayer **layer_ptr = layers.getptr(layer_id);
    if(layer_ptr) {
        return *layer_ptr;
    }else return nullptr;
}