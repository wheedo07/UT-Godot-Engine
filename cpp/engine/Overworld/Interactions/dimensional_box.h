#ifndef DIMENSIONAL_BOX_H
#define DIMENSIONAL_BOX_H

#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
namespace godot {
    class DimensionalBox : public Sprite2D {
        GDCLASS(DimensionalBox, Sprite2D);
        
        protected:
            static void _bind_methods();

        private:
            Ref<PackedScene> text_scene;
            Ref<PackedScene> ui_scene;

            void _on_textbox_select(int option);
        
        public:
            DimensionalBox();
            ~DimensionalBox();

            void _ready() override;
            void _on_interact_box();
    };
}

#endif