#ifndef UI_BOX_H
#define UI_BOX_H

#include<godot_cpp/classes/nine_patch_rect.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/callback_tweener.hpp>
#include<godot_cpp/variant/vector2.hpp>
namespace godot {
    class UI_Box : public NinePatchRect {
        GDCLASS(UI_Box, NinePatchRect)

        protected:
            static void _bind_methods();
        
        private:
            Vector2 defsize;
            Ref<Tween> tw;
        
        public:
            UI_Box();
            ~UI_Box();

            void _ready() override;

            Ref<Tween> get_tw();

            void grow();
            void shrink();
            void hide_box();
    };  
}

#endif