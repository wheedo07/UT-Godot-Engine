#ifndef OPTIONS_H
#define OPTIONS_H

#include<godot_cpp/classes/reference_rect.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/variant/variant.hpp>
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/project_settings.hpp>
namespace godot {
    class Options : public ReferenceRect {
        GDCLASS(Options, ReferenceRect)

        protected:
            static void _bind_methods();
        
        private:
            int current_pos;
            Array option_nodes;
        
        public:
            Options();
            ~Options();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            void enable(int x);
            void disable();
            void refresh_thing(int action = 0);
            int get_current_pos() const;

            void set_current_pos(int pos);
    };
}

#endif