#include "dust_transition.h"
#include "enemy.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/viewport_texture.hpp>
#include<godot_cpp/classes/rendering_server.hpp>
#include<godot_cpp/classes/image.hpp>
#include<godot_cpp/classes/tree.hpp>
using namespace godot;

DustTransition::DustTransition() {
    is_active = false;
    one_shot = false;
    sprite = nullptr;
    enemy = nullptr;
    viewport = nullptr;
    dissolve_height = 0;
    dissolve_progress = 0;
    sprite_max_y = 0;
    sprite_min_y = 0;

    viewport_size = Vector2i(200, 200);
    sample_rate = 2;
    alpha_threshold = 0.1f;
    gravity = -20;
    dust_lifetime = 2.0f;
    velocity_range_x = Vector2(-20, 20);
    velocity_range_y = Vector2(-30, -20);
    dissolve_speed = 0.4f;
    transition_speed = 3.0f;
    is_debug = false;
    particle_color_tint = Color(1,1,1,1);
    particle_size = 1;
    size_shrink_factor = 0.3f;
    transition_size_multiplier = 1.5f;
    easing_type = EASING_TYPE::CUBIC;
    transition_movement_factor = 0.1f;
}

DustTransition::~DustTransition() {}

void DustTransition::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished"));
    BIND_ENUM_CONSTANT(LINEAR);
    BIND_ENUM_CONSTANT(EASE_OUT);
    BIND_ENUM_CONSTANT(EASE_IN_OUT);
    BIND_ENUM_CONSTANT(CUBIC);

    ClassDB::bind_method(D_METHOD("_on_frame_waited", "start"), &DustTransition::_on_frame_waited);
    ClassDB::bind_method(D_METHOD("_on_sort_custom", "a", "b"), &DustTransition::_on_sort_custom);

    ADD_GROUP("Particle Setting", "");
    ClassDB::bind_method(D_METHOD("set_sample_rate", "rate"), &DustTransition::set_sample_rate);
    ClassDB::bind_method(D_METHOD("get_sample_rate"), &DustTransition::get_sample_rate);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "sample_rate", PROPERTY_HINT_RANGE, "1,5,1"), "set_sample_rate", "get_sample_rate");
    ClassDB::bind_method(D_METHOD("set_alpha_threshold", "threshold"), &DustTransition::set_alpha_threshold);
    ClassDB::bind_method(D_METHOD("get_alpha_threshold"), &DustTransition::get_alpha_threshold);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "alpha_threshold", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_alpha_threshold", "get_alpha_threshold");
    ClassDB::bind_method(D_METHOD("set_particle_color_tint", "color"), &DustTransition::set_particle_color_tint);
    ClassDB::bind_method(D_METHOD("get_particle_color_tint"), &DustTransition::get_particle_color_tint);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "particle_color_tint"), "set_particle_color_tint", "get_particle_color_tint");
    ClassDB::bind_method(D_METHOD("set_dissolve_speed", "speed"), &DustTransition::set_dissolve_speed);
    ClassDB::bind_method(D_METHOD("get_dissolve_speed"), &DustTransition::get_dissolve_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dissolve_speed", PROPERTY_HINT_RANGE, "0.1,5.0,0.1"), "set_dissolve_speed", "get_dissolve_speed");
    ClassDB::bind_method(D_METHOD("set_transition_speed", "speed"), &DustTransition::set_transition_speed);
    ClassDB::bind_method(D_METHOD("get_transition_speed"), &DustTransition::get_transition_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transition_speed", PROPERTY_HINT_RANGE, "0.1,10.0,0.1"), "set_transition_speed", "get_transition_speed");

    ADD_GROUP("Dust Physics", "");
    ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &DustTransition::set_gravity);
    ClassDB::bind_method(D_METHOD("get_gravity"), &DustTransition::get_gravity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gravity"), "set_gravity", "get_gravity");
    ClassDB::bind_method(D_METHOD("set_dust_lifetime", "lifetime"), &DustTransition::set_dust_lifetime);
    ClassDB::bind_method(D_METHOD("get_dust_lifetime"), &DustTransition::get_dust_lifetime);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dust_lifetime", PROPERTY_HINT_RANGE, "0.1,10.0,0.1"), "set_dust_lifetime", "get_dust_lifetime");
    ClassDB::bind_method(D_METHOD("set_velocity_range_x", "range"), &DustTransition::set_velocity_range_x);
    ClassDB::bind_method(D_METHOD("get_velocity_range_x"), &DustTransition::get_velocity_range_x);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity_range_x"), "set_velocity_range_x", "get_velocity_range_x");
    ClassDB::bind_method(D_METHOD("set_velocity_range_y", "range"), &DustTransition::set_velocity_range_y);
    ClassDB::bind_method(D_METHOD("get_velocity_range_y"), &DustTransition::get_velocity_range_y);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity_range_y"), "set_velocity_range_y", "get_velocity_range_y");

    ADD_GROUP("Visual", "");
    ClassDB::bind_method(D_METHOD("set_viewport_size", "size"), &DustTransition::set_viewport_size);
    ClassDB::bind_method(D_METHOD("get_viewport_size"), &DustTransition::get_viewport_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "viewport_size"), "set_viewport_size", "get_viewport_size");
    ClassDB::bind_method(D_METHOD("set_particle_size", "size"), &DustTransition::set_particle_size);
    ClassDB::bind_method(D_METHOD("get_particle_size"), &DustTransition::get_particle_size);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "particle_size", PROPERTY_HINT_RANGE, "0.1,10.0,0.1"), "set_particle_size", "get_particle_size");
    ClassDB::bind_method(D_METHOD("set_size_shrink_factor", "factor"), &DustTransition::set_size_shrink_factor);
    ClassDB::bind_method(D_METHOD("get_size_shrink_factor"), &DustTransition::get_size_shrink_factor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "size_shrink_factor", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_size_shrink_factor", "get_size_shrink_factor");
    ClassDB::bind_method(D_METHOD("set_transition_size_multiplier", "multiplier"), &DustTransition::set_transition_size_multiplier);
    ClassDB::bind_method(D_METHOD("get_transition_size_multiplier"), &DustTransition::get_transition_size_multiplier);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transition_size_multiplier", PROPERTY_HINT_RANGE, "1.0,5.0,0.1"), "set_transition_size_multiplier", "get_transition_size_multiplier");

    ADD_GROUP("Transition", "");
    ClassDB::bind_method(D_METHOD("set_easing_type", "type"), &DustTransition::set_easing_type);
    ClassDB::bind_method(D_METHOD("get_easing_type"), &DustTransition::get_easing_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "easing_type", PROPERTY_HINT_ENUM, "LINEAR,EASE_OUT,EASE_IN_OUT,CUBIC"), "set_easing_type", "get_easing_type");
    ClassDB::bind_method(D_METHOD("set_transition_movement_factor", "factor"), &DustTransition::set_transition_movement_factor);
    ClassDB::bind_method(D_METHOD("get_transition_movement_factor"), &DustTransition::get_transition_movement_factor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transition_movement_factor", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_transition_movement_factor", "get_transition_movement_factor");

    ADD_GROUP("Debug", "");
    ClassDB::bind_method(D_METHOD("set_is_debug", "debug"), &DustTransition::set_is_debug);
    ClassDB::bind_method(D_METHOD("get_is_debug"), &DustTransition::get_is_debug);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_debug"), "set_is_debug", "get_is_debug");

    ClassDB::bind_method(D_METHOD("start_transition"), &DustTransition::start_transition);
}

void DustTransition::_process(double delta) {
    if(!is_active) return;

    dissolve_progress += dissolve_speed * delta;
    dissolve_height = UtilityFunctions::lerp(sprite_min_y, sprite_max_y, dissolve_progress);

    bool all_dead = true;
    for(int i=0; i < dust.size(); i++) {
        Dictionary particle = dust[i];
        float distance = dissolve_height - float(particle["start_y"]);
        int state = int(particle["state"]);

        if(state == 0) {
            if(distance > 0) {
                particle["state"] = 1;
            }else all_dead = false;
        }else if(state == 1) {
            all_dead = false;
            particle["transition_progress"] = float(particle["transition_progress"]) + (transition_speed * delta);
            if(float(particle["transition_progress"]) >= 1.0f) {
                particle["state"] = 2;
                particle["transition_progress"] = 1.0f;
            }
        }else if(state == 2) {
            particle["tmr"] = float(particle["tmr"]) + delta;
            if(float(particle["tmr"]) < dust_lifetime) {
                all_dead = false;
                Vector2 velocity = Vector2(particle["velocity"]);
                Vector2 position = Vector2(particle["position"]);

                velocity.y += gravity * delta;
                position += velocity * delta;
                particle["velocity"] = velocity;
                particle["position"] = position;
            }
        }
    }

    if(all_dead || dissolve_progress >= 1.5f) {
        is_active = false;
        one_shot = true;
        dust.clear();
        if(is_debug) print_line(String::utf8("DustTransition: 완료."));
        emit_signal("finished");
    }
    queue_redraw();
}

void DustTransition::_draw() {
    if(!is_active || dust.is_empty()) return;

    Vector2 offset = sprite->get("position");
    RID canvas = get_canvas_item();
    RenderingServer *rs = RenderingServer::get_singleton();
    
    for(int i=0; i < dust.size(); i++) {
        Dictionary particle = dust[i];
        
        Color color = particle["color"];
        Vector2 position = particle["position"];
        Vector2 velocity = particle["velocity"];
        float psize = sample_rate * particle_size;
        float start_x = particle["start_x"];
        float start_y = particle["start_y"];
        int state = particle["state"];

        Rect2 rect;
        if(state == 0) {
            rect = Rect2(
                offset.x + start_x - psize/2,
                offset.y + start_y - psize/2,
                psize, psize
            );
        }else if(state == 1) {
            float t = apply_easing(particle["transition_progress"]);

            Color tinted_color = color.lerp(color * particle_color_tint, t);
            tinted_color.a = color.a * (1.0f - t);
            color = tinted_color;

            position.x = UtilityFunctions::lerp(start_x, start_x + velocity.x * transition_movement_factor, t);
            position.y = UtilityFunctions::lerp(start_y, start_y + velocity.y * transition_movement_factor, t);
            psize = UtilityFunctions::lerp(psize, psize * transition_size_multiplier, t);

            rect = Rect2(
                offset.x + position.x - psize/2,
                offset.y + position.y - psize/2,
                psize, psize
            );
        }else if(state == 2) {
            float tmr = particle["tmr"];
            float progress = tmr / dust_lifetime;
            if(progress >= 1.0f) continue;

            color = color * particle_color_tint;
            color.a *= (1.0f - progress);
            psize = particle_size * 1.5 * (1.0 - progress * size_shrink_factor);

            rect = Rect2(
                offset.x + position.x - psize/2,
                offset.y + position.y - psize/2,
                psize, psize
            );
        }
        
        rs->canvas_item_add_rect(canvas, rect, color);
    }

    if(is_debug && dissolve_progress < 1.0f) {
        draw_line(
            Vector2(offset.x - 60, offset.y + dissolve_height),
            Vector2(offset.x + 60, offset.y + dissolve_height),
            Color(1,0,0,1),
            2.0f
        );
    }
}

void DustTransition::start_transition() {
    dust.clear();
    if(one_shot) {
        emit_signal("finished");
        return;
    }
    if(!sprite) {
        ERR_PRINT("DustTransition: Enemy 노드에 sprite 노드가 설정되지 않았습니다.");
        return;
    }

    viewport = memnew(SubViewport);
    viewport->set_size(viewport_size);
    viewport->set_transparent_background(true);
    viewport->set_update_mode(SubViewport::UPDATE_ALWAYS);
    viewport->set_default_canvas_item_texture_filter(Viewport::DEFAULT_CANVAS_ITEM_TEXTURE_FILTER_NEAREST);
    enemy->add_child(viewport);

    sprite_pos = Vector2(viewport_size.x / 2, viewport_size.y / 2);
    Node* sprite_copy = sprite->duplicate();
    sprite_copy->set("position", sprite_pos);
    viewport->add_child(sprite_copy);

    get_tree()->connect("process_frame", Callable(this, "_on_frame_waited").bind(false), CONNECT_ONE_SHOT);
}

void DustTransition::_on_frame_waited(bool start) {
    if(!start) {
        get_tree()->connect("process_frame", Callable(this, "_on_frame_waited").bind(true), CONNECT_ONE_SHOT);
        return;
    }

    Ref<Image> img = viewport->get_texture()->get_image();
    if(img.is_null()) {
        ERR_PRINT("DustTransition: ViewportTexture에서 이미지를 가져오지 못했습니다.");
        viewport->queue_free();
        return;
    }
    Vector2 size = img->get_size();
	sprite_min_y = 999999.0;
	sprite_max_y = -999999.0;

    for(int y=0; y < size.y; y += sample_rate) {
        for(int x=0; x < size.x; x += sample_rate) {
            Color pixel = img->get_pixel(x, y);

            if(pixel.a < alpha_threshold) continue;
            float particle_y = y - sprite_pos.y;
            sprite_min_y = MIN(sprite_min_y, particle_y);
            sprite_max_y = MAX(sprite_max_y, particle_y);

            Dictionary particle;
            particle["position"] = Vector2(x - sprite_pos.x, particle_y);
            particle["start_x"] = x - sprite_pos.x;
            particle["start_y"] = particle_y;
            particle["velocity"] = Vector2(
                UtilityFunctions::randf_range(velocity_range_x.x, velocity_range_x.y),
                UtilityFunctions::randf_range(velocity_range_y.x, velocity_range_y.y)
            );
            particle["color"] = pixel;
            particle["tmr"] = 0.0f;
            particle["state"] = 0; // 0=원본, 1=전환중, 2=날아감
            particle["transition_progress"] = 0.0f;
            dust.append(particle);
        }
    }

    viewport->queue_free();
    if(dust.is_empty()) {
        ERR_PRINT("DustTransition: 생성된 먼지 입자가 없습니다.");
        return;
    }
    if(sprite->has_method("hide")) {
        sprite->call("hide");
    }else {
        ERR_PRINT("DustTransition: sprite 노드에 hide() 메서드가 없습니다.");
        return;
    }
    dust.sort_custom(Callable(this, "_on_sort_custom"));
    dissolve_height = sprite_min_y;
    is_active = true;

    get_node<AudioStreamPlayer>("sound")->play();
    queue_redraw();
}

bool DustTransition::_on_sort_custom(Dictionary a, Dictionary b) {
    float a_startY = a["start_y"];
    float b_startY = b["start_y"];
    return a_startY < b_startY;
}

float DustTransition::apply_easing(float delta) {
    switch(easing_type) {
        case EASING_TYPE::LINEAR:
            return delta;
        case EASING_TYPE::EASE_OUT:
            return 1 - pow(1 - delta, 3);
        case EASING_TYPE::EASE_IN_OUT:
            return delta * delta * (3 - 2 * delta);
        case EASING_TYPE::CUBIC:
            return 1.0 - pow(1.0 - delta, 3);
        default:
            return delta;
    }
}

void DustTransition::set_viewport_size(const Vector2i& p_size) {
    viewport_size = p_size;
}

Vector2i DustTransition::get_viewport_size() const {
    return viewport_size;
}

void DustTransition::set_sample_rate(int p_rate) {
    sample_rate = p_rate;
}

int DustTransition::get_sample_rate() const {
    return sample_rate;
}

void DustTransition::set_alpha_threshold(float p_threshold) {
    alpha_threshold = p_threshold;
}

float DustTransition::get_alpha_threshold() const {
    return alpha_threshold;
}

void DustTransition::set_gravity(float p_gravity) {
    gravity = p_gravity;
}

float DustTransition::get_gravity() const {
    return gravity;
}

void DustTransition::set_dust_lifetime(float p_lifetime) {
    dust_lifetime = p_lifetime;
}

float DustTransition::get_dust_lifetime() const {
    return dust_lifetime;
}

void DustTransition::set_velocity_range_x(const Vector2& p_range) {
    velocity_range_x = p_range;
}

Vector2 DustTransition::get_velocity_range_x() const {
    return velocity_range_x;
}

void DustTransition::set_velocity_range_y(const Vector2& p_range) {
    velocity_range_y = p_range;
}

Vector2 DustTransition::get_velocity_range_y() const {
    return velocity_range_y;
}

void DustTransition::set_dissolve_speed(float p_speed) {
    dissolve_speed = p_speed;
}

float DustTransition::get_dissolve_speed() const {
    return dissolve_speed;
}

void DustTransition::set_transition_speed(float p_speed) {
    transition_speed = p_speed;
}

float DustTransition::get_transition_speed() const {
    return transition_speed;
}

void DustTransition::set_is_debug(bool p_debug) {
    is_debug = p_debug;
}

bool DustTransition::get_is_debug() const {
    return is_debug;
}

void DustTransition::set_particle_color_tint(const Color& p_color) {
    particle_color_tint = p_color;
}

Color DustTransition::get_particle_color_tint() const {
    return particle_color_tint;
}

void DustTransition::set_particle_size(float p_size) {
    particle_size = p_size;
}

float DustTransition::get_particle_size() const {
    return particle_size;
}

void DustTransition::set_size_shrink_factor(float p_factor) {
    size_shrink_factor = p_factor;
}

float DustTransition::get_size_shrink_factor() const {
    return size_shrink_factor;
}

void DustTransition::set_transition_size_multiplier(float p_multiplier) {
    transition_size_multiplier = p_multiplier;
}

float DustTransition::get_transition_size_multiplier() const {
    return transition_size_multiplier;
}

void DustTransition::set_easing_type(EASING_TYPE p_type) {
    easing_type = p_type;
}

DustTransition::EASING_TYPE DustTransition::get_easing_type() const {
    return easing_type;
}

void DustTransition::set_transition_movement_factor(float p_factor) {
    transition_movement_factor = p_factor;
}

float DustTransition::get_transition_movement_factor() const {
    return transition_movement_factor;
}