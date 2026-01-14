#ifndef BattleObject_H
#define BattleObject_H
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/callback_tweener.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
namespace godot {
    class BattleObject : public Control {
        GDCLASS(BattleObject, Control);

        protected:
            static void _bind_methods();

        public:
            BattleObject();
            ~BattleObject();
           
            void fade();
   };
}

#endif