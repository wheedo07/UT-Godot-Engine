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
            Ref<Tween> velocity_tween;
            Vector2 velocity;

        private:
            void on_exit_screen();

            void set_velocity_tween(Ref<Tween> value);
            Ref<Tween> get_velocity_tween() const;

            void set_velocity(Vector2 value);
            Vector2 get_velocity() const;
            
        public:
            BattleObject();
            ~BattleObject();
           
            void kill();
            void fade();
   };
}

#endif