#ifndef BOX_SET_H
#define BOX_SET_H

#include<godot_cpp/classes/resource.hpp>
#include<godot_cpp/classes/texture.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
namespace godot {
    class BoxSet : public Resource {
        GDCLASS(BoxSet, Resource);
        
        protected:
            static void _bind_methods();
        
        private:
            Color board_color;
            Color board_border_color;

            bool isCustomMeter;

            Ref<PackedScene> meter_scene;

            Ref<Texture> meter_texture;
            Vector2 meter_scale;
        
        public:
            BoxSet();
            ~BoxSet();

            void _get_property_list(List<PropertyInfo> *p_list) const;
            bool _set(const StringName& p_name, const Variant& p_value);
            bool _get(const StringName& p_name, Variant& r_ret);

            void set_board_color(const Color& p_color);
            Color get_board_color() const;

            void set_board_border_color(const Color& p_color);
            Color get_board_border_color() const;

            void set_is_custom_meter(bool p_is_custom);
            bool is_custom_meter() const;

            void set_meter_scene(const Ref<PackedScene>& p_scene);
            Ref<PackedScene> get_meter_scene() const;

            void set_meter_texture(const Ref<Texture>& p_texture);
            Ref<Texture> get_meter_texture() const;

            void set_meter_scale(const Vector2& p_scale);
            Vector2 get_meter_scale() const;
    };
}

#endif