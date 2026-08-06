#include "layer.h"
#include<godot_cpp/classes/project_settings.hpp>
using namespace godot;

void UTGELayer::_bind_methods() {
    BIND_ENUM_CONSTANT(DIRECT);
    BIND_ENUM_CONSTANT(SUBVIEWPORT);

    ClassDB::bind_method(D_METHOD("set_layer_id", "value"), &UTGELayer::set_layer_id);
    ClassDB::bind_method(D_METHOD("get_layer_id"), &UTGELayer::get_layer_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "layer_id"), "set_layer_id", "get_layer_id");

    ClassDB::bind_method(D_METHOD("set_render_mode", "value"), &UTGELayer::set_render_mode);
    ClassDB::bind_method(D_METHOD("get_render_mode"), &UTGELayer::get_render_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "render_mode", PROPERTY_HINT_ENUM, "Direct,SubViewport", PROPERTY_USAGE_DEFAULT), "set_render_mode", "get_render_mode");
}

void UTGELayer::_get_property_list(List<PropertyInfo> *p_list) const {
    if(render_mode == SUBVIEWPORT) {
        p_list->push_back(PropertyInfo(Variant::NIL, "SubViewport Settings", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_CATEGORY));
        p_list->push_back(PropertyInfo(Variant::INT, "default_texture_filter", PROPERTY_HINT_ENUM, "Nearest,Linear,Linear with Mipmaps,Nearest with Mipmaps"));
        p_list->push_back(PropertyInfo(Variant::INT, "default_texture_repeat", PROPERTY_HINT_ENUM, "Disabled,Enabled,Mirror"));
    }
}

bool UTGELayer::_set(const StringName& p_name, const Variant& p_value) {
    String name = p_name;
    if(name == "default_texture_filter") {
        default_texture_filter = SubViewport::DefaultCanvasItemTextureFilter(int(p_value));
        _apply_viewport_settings();
        return true;
    }else if(name == "default_texture_repeat") {
        default_texture_repeat = SubViewport::DefaultCanvasItemTextureRepeat(int(p_value));
        _apply_viewport_settings();
        return true;
    }
    return false;
}

bool UTGELayer::_get(const StringName& p_name, Variant& r_ret) {
    String name = p_name;
    if(name == "default_texture_filter") {
        r_ret = default_texture_filter;
        return true;
    }else if(name == "default_texture_repeat") {
        r_ret = default_texture_repeat;
        return true;
    }
    return false;
}

void UTGELayer::_ready() {
    if(render_mode != SUBVIEWPORT) {
        parent = this;
        return;
    }
    SubViewport *viewport = memnew(SubViewport);
    parent = viewport;
    _apply_viewport_settings();
}

void UTGELayer::_apply_viewport_settings() {
    if(!parent || !parent->is_class("SubViewport")) return;
    ProjectSettings *setting = ProjectSettings::get_singleton();
    int width = int(setting->get_setting("display/window/size/viewport_width"));
    int height = int(setting->get_setting("display/window/size/viewport_height"));

    SubViewport *viewport = Object::cast_to<SubViewport>(parent);
    viewport->set_default_canvas_item_texture_filter(default_texture_filter);
    viewport->set_default_canvas_item_texture_repeat(default_texture_repeat);
    viewport->set_transparent_background(true);
    viewport->set_size(Vector2(width, height));
}

void UTGELayer::set_layer_id(StringName value) {
    layer_id = value;
}

StringName UTGELayer::get_layer_id() {
    return layer_id;
}

void UTGELayer::set_render_mode(RenderMode value) {
    render_mode = value;
}

UTGELayer::RenderMode UTGELayer::get_render_mode() {
    return render_mode;
}