#pragma once

#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/sub_viewport.hpp>
namespace godot {
    class UTGELayer : public Node {
        GDCLASS(UTGELayer, Node);

        protected:
            static void _bind_methods();

        public:
            enum RenderMode {
                DIRECT,
                SUBVIEWPORT
            };

        private:
            StringName layer_id = "main";
            RenderMode render_mode = DIRECT;

            SubViewport::DefaultCanvasItemTextureFilter default_texture_filter =
            SubViewport::DefaultCanvasItemTextureFilter::DEFAULT_CANVAS_ITEM_TEXTURE_FILTER_LINEAR;

            SubViewport::DefaultCanvasItemTextureRepeat default_texture_repeat =
            SubViewport::DefaultCanvasItemTextureRepeat::DEFAULT_CANVAS_ITEM_TEXTURE_REPEAT_DISABLED;

            Node *parent = nullptr;
            void set_layer_id(StringName value);
            StringName get_layer_id();

            void set_render_mode(RenderMode value);
            RenderMode get_render_mode();

            void _apply_viewport_settings();

        public:
            void _ready() override;

            void _get_property_list(List<PropertyInfo> *p_list) const;
            bool _set(const StringName& p_name, const Variant& p_value);
            bool _get(const StringName& p_name, Variant& r_ret);
    };
}