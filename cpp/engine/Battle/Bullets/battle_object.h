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
            enum FireMode {
                VELOCITY,
                TWEEN
            };
            Vector2 velocity;
            Ref<Tween> velocity_tween;

        private:
            Tween::TransitionType trans;
            Tween::EaseType ease;

            void on_exit_screen();
            
        public:
            BattleObject();
            ~BattleObject();
           
            void kill();
            void fade();

            void set_trans(Tween::TransitionType value);
            Tween::TransitionType get_trans() const;

            void set_ease(Tween::EaseType value);
            Tween::EaseType get_ease() const;
    };
}
VARIANT_ENUM_CAST(godot::BattleObject::FireMode);

#endif