#include "joystick.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/input_event_screen_touch.hpp>
#include<godot_cpp/classes/input_event_screen_drag.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/input_event_key.hpp>
#include<godot_cpp/classes/input_map.hpp>
#include<godot_cpp/classes/display_server.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

Joystick::Joystick() {
    isEditor = true;
    is_pressed = false;
    joystick_mode = Joystick_mode::FIXED;
    visibility_mode = Visibility_mode::ALWAYS;
    touch_index = -1;
    deadzone_size = 10.0f;
    clampzone_size = 75.0f;
    coolTime = 0.1f;
    output = Vector2(0,0);
    pressed_color = Color(0.5, 0.5, 0.5);
    action_left = "ui_left";
    action_right = "ui_right";
    action_up = "ui_up";
    action_down = "ui_down";
}

Joystick::~Joystick() {}

void Joystick::_bind_methods() {
    BIND_ENUM_CONSTANT(FIXED);
    BIND_ENUM_CONSTANT(DYNAMIC);
    BIND_ENUM_CONSTANT(FOLLOWING);
    
    BIND_ENUM_CONSTANT(ALWAYS);
    BIND_ENUM_CONSTANT(WHEN_TOUCHED);

    ClassDB::bind_method(D_METHOD("set_pressed_color", "value"), &Joystick::set_pressed_color);
    ClassDB::bind_method(D_METHOD("get_pressed_color"), &Joystick::get_pressed_color);
    ClassDB::bind_method(D_METHOD("set_joystick_mode", "value"), &Joystick::set_joystick_mode);
    ClassDB::bind_method(D_METHOD("get_joystick_mode"), &Joystick::get_joystick_mode);
    ClassDB::bind_method(D_METHOD("set_visibility_mode", "value"), &Joystick::set_visibility_mode);
    ClassDB::bind_method(D_METHOD("get_visibility_mode"), &Joystick::get_visibility_mode);
    ClassDB::bind_method(D_METHOD("set_deadzone_size", "value"), &Joystick::set_deadzone_size);
    ClassDB::bind_method(D_METHOD("get_deadzone_size"), &Joystick::get_deadzone_size);
    ClassDB::bind_method(D_METHOD("set_clampzone_size", "value"), &Joystick::set_clampzone_size);
    ClassDB::bind_method(D_METHOD("get_clampzone_size"), &Joystick::get_clampzone_size);
    ClassDB::bind_method(D_METHOD("set_cooltime", "value"), &Joystick::set_cooltime);
    ClassDB::bind_method(D_METHOD("get_cooltime"), &Joystick::get_cooltime);
    ClassDB::bind_method(D_METHOD("set_action_left", "value"), &Joystick::set_action_left);
    ClassDB::bind_method(D_METHOD("get_action_left"), &Joystick::get_action_left);
    ClassDB::bind_method(D_METHOD("set_action_right", "value"), &Joystick::set_action_right);
    ClassDB::bind_method(D_METHOD("get_action_right"), &Joystick::get_action_right);
    ClassDB::bind_method(D_METHOD("set_action_up", "value"), &Joystick::set_action_up);
    ClassDB::bind_method(D_METHOD("get_action_up"), &Joystick::get_action_up);
    ClassDB::bind_method(D_METHOD("set_action_down", "value"), &Joystick::set_action_down);
    ClassDB::bind_method(D_METHOD("get_action_down"), &Joystick::get_action_down);

    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "pressed_color"), "set_pressed_color", "get_pressed_color");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "joystick_mode", PROPERTY_HINT_ENUM, "Dynamic,Fixed,Following"), "set_joystick_mode", "get_joystick_mode");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visibility_mode", PROPERTY_HINT_ENUM, "Always Visible,When Touched"), "set_visibility_mode", "get_visibility_mode");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "deadzone_size", PROPERTY_HINT_RANGE, "0,200,1"), "set_deadzone_size", "get_deadzone_size");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "clampzone_size", PROPERTY_HINT_RANGE, "0,500,1"), "set_clampzone_size", "get_clampzone_size");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cooltime", PROPERTY_HINT_RANGE, "0,10,0.01"), "set_cooltime", "get_cooltime");

    ADD_GROUP("Input Actions", "");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "action_left"), "set_action_left", "get_action_left");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "action_right"), "set_action_right", "get_action_right");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "action_up"), "set_action_up", "get_action_up");
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "action_down"), "set_action_down", "get_action_down");
}

void Joystick::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    base = Object::cast_to<TextureRect>(get_node_internal("Base"));
    tip = Object::cast_to<TextureRect>(get_node_internal("Base/Tip"));

    base_default_position = base->get_position();
    tip_default_position = tip->get_position();
    default_color = tip->get_modulate();
    
    actions_time[action_left] = Variant();
    actions_time[action_right] = Variant();
    actions_time[action_up] = Variant();
    actions_time[action_down] = Variant();

    _reset();
}

void Joystick::_process(double delta) {
    if(isEditor) return;
    Array values = actions_time.values();
    Array keys = actions_time.keys();
    for(int i=0; i < values.size(); i++) {
        if(values[i].get_type() == Variant::NIL) continue;

        float time = values[i];
        if(time > coolTime) {
            actions_time[keys[i]] = Variant();
        }else actions_time[keys[i]] = time + delta;
    }
}

void Joystick::_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    
    Ref<InputEventScreenTouch> touch_event = event;
    if (touch_event.is_valid()) {
        if (touch_event->is_pressed()) {
            if (_is_point_inside_joystick_area(touch_event->get_position()) && touch_index == -1) {
                if (joystick_mode == DYNAMIC || joystick_mode == FOLLOWING || 
                   (joystick_mode == FIXED && _is_point_inside_base(touch_event->get_position()))) {
                    if (joystick_mode == DYNAMIC || joystick_mode == FOLLOWING) {
                        _move_base(touch_event->get_position());
                    }
                    if (visibility_mode == WHEN_TOUCHED) {
                        show();
                    }
                    touch_index = touch_event->get_index();
                    tip->set_modulate(pressed_color);
                    _update_joystick(touch_event->get_position());
                    get_viewport()->set_input_as_handled();
                }
            }
        }else if(touch_event->get_index() == touch_index) {
            _reset();
            if (visibility_mode == WHEN_TOUCHED) {
                hide();
            }
            get_viewport()->set_input_as_handled();
        }
    }
    
    Ref<InputEventScreenDrag> drag_event = event;
    if(drag_event.is_valid()) {
        if (drag_event->get_index() == touch_index) {
            _update_joystick(drag_event->get_position());
            get_viewport()->set_input_as_handled();
        }
    }
}

void Joystick::_move_base(const Vector2& new_position) {
    Vector2 scale = get_global_transform_with_canvas().get_scale();
    base->set_global_position(new_position - base->get_pivot_offset() * scale);
}

void Joystick::_move_tip(const Vector2& new_position) {
    Vector2 scale = base->get_global_transform_with_canvas().get_scale();
    tip->set_global_position(new_position - tip->get_pivot_offset() * scale);
}

bool Joystick::_is_point_inside_joystick_area(const Vector2& point) {
    Vector2 scale = get_global_transform_with_canvas().get_scale();
    Vector2 size_scaled = get_size() * scale;
    
    bool x_inside = point.x >= get_global_position().x && 
                    point.x <= get_global_position().x + size_scaled.x;
                    
    bool y_inside = point.y >= get_global_position().y && 
                    point.y <= get_global_position().y + size_scaled.y;
                    
    return x_inside && y_inside;
}

Vector2 Joystick::_get_base_radius() {
    return base->get_size() * base->get_global_transform_with_canvas().get_scale() / 2.0;
}

bool Joystick::_is_point_inside_base(const Vector2& point) {
    Vector2 base_radius = _get_base_radius();
    Vector2 center = base->get_global_position() + base_radius;
    Vector2 vector = point - center;
    
    return vector.length_squared() <= base_radius.x * base_radius.x;
}

void Joystick::_update_joystick(const Vector2& touch_position) {
    Vector2 base_radius = _get_base_radius();
    Vector2 center = base->get_global_position() + base_radius;
    Vector2 vector = touch_position - center;
    
    if(vector.length() > clampzone_size) {
        vector = vector.normalized() * clampzone_size;
    }
    
    if(joystick_mode == FOLLOWING && touch_position.distance_to(center) > clampzone_size) {
        _move_base(touch_position - vector);
    }
    
    _move_tip(center + vector);
    
    if(vector.length_squared() > deadzone_size * deadzone_size) {
        is_pressed = true;
        output = (vector - (vector.normalized() * deadzone_size)) / (clampzone_size - deadzone_size);
    }else {
        is_pressed = false;
        output = Vector2(0, 0);
    }
    
    float threshold = 0.3f;
    
    float left_strength = output.x < -threshold ? -output.x : 0;
    float right_strength = output.x > threshold ? output.x : 0;
    float up_strength = output.y < -threshold ? -output.y : 0;
    float down_strength = output.y > threshold ? output.y : 0;

    _set_action_state(action_left, left_strength > 0, left_strength);
    _set_action_state(action_right, right_strength > 0, right_strength);
    _set_action_state(action_up, up_strength > 0, up_strength);
    _set_action_state(action_down, down_strength > 0, down_strength);
}

void Joystick::_set_action_state(const String& action, bool pressed, float strength) {
    TypedArray<InputEvent> inputs = InputMap::get_singleton()->action_get_events(action);
    for(int i=0; i < inputs.size(); i++) {
        Ref<InputEventKey> input = inputs[i];
        if(input.is_valid()) {
            bool is_action = actions_time[action].get_type() == Variant::NIL;
            Ref<InputEventKey> event = memnew(InputEventKey);
            event->set_keycode(input->get_keycode());
            event->set_echo(!is_action);
            event->set_pressed(pressed);
            Input::get_singleton()->parse_input_event(event);

            if(is_action) actions_time[action] = 0;
            break;
        }
    }

    if(pressed) {
        Ref<InputEventAction> event = memnew(InputEventAction);
        event->set_action(action);
        event->set_strength(strength);
        Input::get_singleton()->parse_input_event(event);
    }
}

void Joystick::_reset() {
    is_pressed = false;
    output = Vector2(0, 0);
    touch_index = -1;
    
    tip->set_modulate(default_color);
    base->set_position(base_default_position);
    tip->set_position(tip_default_position);
    
    _set_action_state(action_left, false);
    _set_action_state(action_right, false);
    _set_action_state(action_up, false);
    _set_action_state(action_down, false);
}

void Joystick::set_pressed_color(Color value) {
    pressed_color = value;
}

Color Joystick::get_pressed_color() const {
    return pressed_color;
}

void Joystick::set_joystick_mode(Joystick_mode value) {
    joystick_mode = value;
}

Joystick::Joystick_mode Joystick::get_joystick_mode() const {
    return joystick_mode;
}

void Joystick::set_visibility_mode(Visibility_mode value) {
    visibility_mode = value;
}

Joystick::Visibility_mode Joystick::get_visibility_mode() const {
    return visibility_mode;
}

void Joystick::set_deadzone_size(float value) {
    deadzone_size = value;
}

float Joystick::get_deadzone_size() const {
    return deadzone_size;
}

void Joystick::set_clampzone_size(float value) {
    clampzone_size = value;
}

float Joystick::get_clampzone_size() const {
    return clampzone_size;
}

void Joystick::set_action_left(const StringName& value) {
    action_left = value;
}

StringName Joystick::get_action_left() const {
    return action_left;
}

void Joystick::set_action_right(const StringName& value) {
    action_right = value;
}

StringName Joystick::get_action_right() const {
    return action_right;
}

void Joystick::set_action_up(const StringName& value) {
    action_up = value;
}

StringName Joystick::get_action_up() const {
    return action_up;
}

void Joystick::set_action_down(const StringName& value) {
    action_down = value;
}

StringName Joystick::get_action_down() const {
    return action_down;
}

void Joystick::set_cooltime(float value) {
    coolTime = value;
}

float Joystick::get_cooltime() const {
    return coolTime;
}