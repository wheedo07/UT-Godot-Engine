#ifndef __SLASH_ANIMATION_H__
#define __SLASH_ANIMATION_H__

#include<godot_cpp/classes/node2d.hpp>
namespace godot {
    class SlashAnimation : public Node2D {
        GDCLASS(SlashAnimation, Node2D);
        
        protected:
            static void _bind_methods();

        private:
            bool crit;
            float dmg_mult;
        
        public:
            SlashAnimation();
            ~SlashAnimation();

            void set_crit(bool value);
            bool get_crit();

            void set_dmg_mult(float value);
            float get_dmg_mult();
    };
}

#endif