#ifndef Joystick_H
#define Joystick_H
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/texture_rect.hpp>
#include<godot_cpp/classes/input.hpp>
namespace godot {
    class Joystick : public Control {
        GDCLASS(Joystick, Control);

        protected:
            static void _bind_methods();

        public:
            enum Joystick_mode {
                FIXED,    // 조이스틱이 고정된 위치에 있음
                DYNAMIC,  // 터치한 위치에 조이스틱 표시
                FOLLOWING // 손가락이 움직이면 조이스틱도 따라움직임
            };

            enum Visibility_mode {
                ALWAYS,           // 항상 표시
                WHEN_TOUCHED      // 터치할 때만 표시
            };

        private:
            bool isEditor, is_pressed;
            float deadzone_size, clampzone_size, coolTime;
            int touch_index;
            Dictionary actions_time;
            StringName action_left;
            StringName action_right;
            StringName action_up;
            StringName action_down;
            Vector2 output;
            Joystick_mode joystick_mode;
            Visibility_mode visibility_mode;
            TextureRect* base;
            TextureRect* tip;
            Vector2 base_default_position;
            Vector2 tip_default_position;
            Color default_color;
            Color pressed_color;
            
            void _move_base(const Vector2& new_position);
            void _move_tip(const Vector2& new_position);
            bool _is_point_inside_joystick_area(const Vector2& point);
            Vector2 _get_base_radius();
            bool _is_point_inside_base(const Vector2& point);
            void _update_joystick(const Vector2& touch_position);
            void _set_action_state(const String& action, bool pressed, float strength = 0);
            void _reset();
            
            void set_pressed_color(Color value);
            Color get_pressed_color() const;
            void set_joystick_mode(Joystick_mode value);
            Joystick_mode get_joystick_mode() const;
            void set_visibility_mode(Visibility_mode value);
            Visibility_mode get_visibility_mode() const;
            void set_deadzone_size(float value);
            float get_deadzone_size() const;
            void set_clampzone_size(float value);
            float get_clampzone_size() const;
            void set_cooltime(float value);
            float get_cooltime() const;

            void set_action_left(const StringName& value);
            StringName get_action_left() const;
            void set_action_right(const StringName& value);
            StringName get_action_right() const;
            void set_action_up(const StringName& value);
            StringName get_action_up() const;
            void set_action_down(const StringName& value);
            StringName get_action_down() const;
        
        public:
            Joystick();
            ~Joystick();

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;
            void _process(double delta) override;
    };
}
VARIANT_ENUM_CAST(godot::Joystick::Joystick_mode);
VARIANT_ENUM_CAST(godot::Joystick::Visibility_mode);

#endif