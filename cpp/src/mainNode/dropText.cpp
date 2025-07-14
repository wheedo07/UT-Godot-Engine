#include "dropText.h"
#include "env.h"
#include<godot_cpp/classes/label.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>

DropText::DropText() {
    is = false;
    spacing = 16;
    gravity = 800;
    font_size = 20;
}

DropText::~DropText() {}

void DropText::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished_drop"));
    ADD_SIGNAL(MethodInfo("stated_typeing"));

    ClassDB::bind_method(D_METHOD("crumble_text", "text", "start_pos", "time"), &DropText::crumble_text, DEFVAL(0.5));
    ClassDB::bind_method(D_METHOD("start"), &DropText::start);

    ClassDB::bind_method(D_METHOD("set_spacing", "value"), &DropText::set_spacing);
    ClassDB::bind_method(D_METHOD("get_spacing"), &DropText::get_spacing);
    ClassDB::bind_method(D_METHOD("set_gravity", "value"), &DropText::set_gravity);
    ClassDB::bind_method(D_METHOD("get_gravity"), &DropText::get_gravity);
    ClassDB::bind_method(D_METHOD("set_font", "value"), &DropText::set_font);
    ClassDB::bind_method(D_METHOD("get_font"), &DropText::get_font);
    ClassDB::bind_method(D_METHOD("set_font_size", "value"), &DropText::set_font_size);
    ClassDB::bind_method(D_METHOD("get_font_size"), &DropText::get_font_size);
    ClassDB::bind_method(D_METHOD("set_color", "value"), &DropText::set_color);
    ClassDB::bind_method(D_METHOD("get_color"), &DropText::get_color);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spacing"), "set_spacing", "get_spacing");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gravity"), "set_gravity", "get_gravity");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "font_size"), "set_font_size", "get_font_size");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "font", PROPERTY_HINT_RESOURCE_TYPE, "Font"), "set_font", "get_font");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
}

void DropText::_process(double delta) {
    if(!is || isEditor) return;
    for(int i=0; i < falling_letters.size(); i++) {
        Label* label = Object::cast_to<Label>(falling_letters[i]);
		Vector2 v = label->get_meta("velocity");
		float g = label->get_meta("gravity");
		float r_speed = label->get_meta("rotation_speed");

		v.y += g * delta;
		label->set_position(label->get_position() + (v * delta));
		label->set_rotation(label->get_rotation() + (r_speed * delta));
		label->set_meta("velocity", v);

		if(label->get_position().y > 800) {
            falling_letters.remove_at(i);
            label->queue_free();
        }
    }

    if(falling_letters.size() == 0) {
        is = false;
        emit_signal("finished_drop");
    }
}

void DropText::crumble_text(String text, Vector2 start_pos, float time) {
    set_modulate(color);
    for(int i=0; i < text.length(); i++) {
        String c = text.substr(i, 1);
        Label* label = memnew(Label);
		label->set_text(c);
		label->set_position(start_pos + Vector2(spacing * i, 0));
		label->add_theme_font_override("font", font);
        label->add_theme_font_size_override("font_size", font_size);

        float letter_delay = i * (time / 2);
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(letter_delay);
        timer->connect("timeout", Callable(this, "add_child").bind(label));
        timer->connect("timeout", Callable(this, "emit_signal").bind("stated_typeing"));

		Vector2 velocity = Vector2(UFus::randf_range(-150, 150), UFus::randf_range(-300, -100));
		label->set_meta("velocity", velocity);
		label->set_meta("rotation_speed", UFus::randf_range(-2.0, 2.0));
		label->set_meta("gravity", gravity);
		falling_letters.append(label);
    }
}

void DropText::start() {
    is = true;
}

void DropText::set_spacing(float value) {
    spacing = value;
}

float DropText::get_spacing() const {
    return spacing;
}

void DropText::set_gravity(float value) {
    gravity = value;
}

float DropText::get_gravity() const {
    return gravity;
}

void DropText::set_font(const Ref<Font>& value) {
    font = value;
}

Ref<Font> DropText::get_font() const {
    return font;
}

void DropText::set_font_size(float value) {
    font_size = value;
}

float DropText::get_font_size() const {
    return font_size;
}

void DropText::set_color(Color value) {
    color = value;
}

Color DropText::get_color() const {
    return color;
}