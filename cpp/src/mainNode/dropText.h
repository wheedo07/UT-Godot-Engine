#ifndef DropText_H
#define DropText_H
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/font.hpp>
namespace godot {
    class DropText : public Node2D {
        GDCLASS(DropText, Node2D);

        protected:
            static void _bind_methods();

        private:
            bool is;
            float spacing, gravity, font_size;
            Ref<Font> font;
            Array falling_letters;
            Color color;

        public:
            DropText();
            ~DropText();

            void _process(double delta) override;

            void crumble_text(String text, Vector2 start_pos, float time=0.5);
            void start();

            void set_font_size(float value);
            float get_font_size() const;

            void set_color(Color value);
            Color get_color() const;
        
            void set_spacing(float value);
            float get_spacing() const;

            void set_gravity(float value);
            float get_gravity() const;

            void set_font(const Ref<Font>& value);
            Ref<Font> get_font() const;
    };
};

#endif