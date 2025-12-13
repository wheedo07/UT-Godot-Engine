#ifndef BOX_SET_H
#define BOX_SET_H

#include<godot_cpp/classes/resource.hpp>
#include<godot_cpp/classes/texture.hpp>
namespace godot {
    class BoxSet : public Resource {
        GDCLASS(BoxSet, Resource);
        
        protected:
            static void _bind_methods();
        
        private:
            Color board_color;
            Color board_border_color;

            Ref<Texture> meter_texture;
            Vector2 meter_scale;
        
        public:
            BoxSet();
            ~BoxSet();

            void set_board_color(const Color& p_color);
            Color get_board_color() const;

            void set_board_border_color(const Color& p_color);
            Color get_board_border_color() const;

            void set_meter_texture(const Ref<Texture>& p_texture);
            Ref<Texture> get_meter_texture() const;

            void set_meter_scale(const Vector2& p_scale);
            Vector2 get_meter_scale() const;
    };
}

#endif