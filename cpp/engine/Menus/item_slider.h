#ifndef ITEM_SLIDER_H
#define ITEM_SLIDER_H

#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/node.hpp>
namespace godot {
    class ItemSlider : public Control {
        GDCLASS(ItemSlider, Control)
        
        protected:
            static void _bind_methods();
        
        private:
            int value;
            float offset;
            float step_size;

            NodePath grabber_path;
            Node* grabber_node;

            const float speed = 40.0f;
        
        public:
            ItemSlider();
            ~ItemSlider();

            void _ready() override;
            void _process(double delta) override;

            void set_value(int p_value);
            int get_value() const;

            void set_offset(float p_offset);
            float get_offset() const;

            void set_step_size(float p_step_size);
            float get_step_size() const;

            void set_grabber_path(const NodePath& p_grabber_path);
            NodePath get_grabber_path() const;
    };
}

#endif