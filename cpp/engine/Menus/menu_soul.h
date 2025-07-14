#ifndef MENU_SOUL_H
#define MENU_SOUL_H

#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>

namespace godot {
    class MenuSoul : public Sprite2D {
        GDCLASS(MenuSoul, Sprite2D)

        protected:
            static void _bind_methods();
        
        private:
            const float TIME = 0.15f;
            Ref<Tween> tw;
        
        public:
            MenuSoul();
            ~MenuSoul();

            void move_global(const Vector2& pos);
            void move(const Vector2& pos);
    };
}

#endif