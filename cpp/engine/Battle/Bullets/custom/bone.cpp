#include "bone.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

Bone::Bone()  {
    collision_margin = 4.0f;
    scroll_offset = Vector2(0, 0); 
}

Bone::~Bone() {}

void Bone::_bind_methods() {
    ClassDB::bind_method(D_METHOD("scroll_texture", "speed"), &Bone::scroll_texture, DEFVAL(Vector2(0,0)));
    ClassDB::bind_method(D_METHOD("tween_height", "new_height", "time"), &Bone::tween_height);
    ClassDB::bind_method(D_METHOD("tween_width", "new_width", "time"), &Bone::tween_width);
}

void Bone::ready() {
    sprite_rect = Object::cast_to<NinePatchRect>(get_node_internal(get_sprite_path()));
    collision = Object::cast_to<CollisionShape2D>(get_node_internal(get_collision_path()));
    area = Object::cast_to<BulletArea>(get_node_internal("Area2D"));
    loader = ResourceLoader::get_singleton();

    Ref<RectangleShape2D> shape = memnew(RectangleShape2D);
    shape->set_size(sprite_rect->get_size());
    collision->set_shape(shape);
}

void Bone::_process(double delta) {
    if(!is_inside_tree() || isEditor) return;
    
    Vector2 size = sprite_rect->get_size();
    Vector2 sprite_pos = sprite_rect->get_position();
    Vector2 sprite_center = sprite_pos + size / 2.0f;
    Vector2 area_pos = area->get_position();
    
    Vector2 collision_pos;
    collision_pos.x = sprite_center.x - area_pos.x;
    collision_pos.y = sprite_center.y - area_pos.y;
    
    collision->set_position(collision_pos);
    
    Ref<RectangleShape2D> shape = collision->get_shape();
    shape->set_size(Vector2(size.x - collision_margin, size.y - collision_margin));

    if(scroll_shader.is_valid()) {
        shader_time += delta;
        if (shader_time >= 1.0) {
            shader_time -= 1.0;
            scroll_offset += scroll_shader->get_shader_parameter("speed");
            scroll_offset = Vector2(UtilityFunctions::fmod(scroll_offset.x, 1.0), UtilityFunctions::fmod(scroll_offset.y, 1.0));
        }
        scroll_shader->set_shader_parameter("time", shader_time);
        scroll_shader->set_shader_parameter("offset", scroll_offset);
    }
}

void Bone::scroll_texture(Vector2 speed) {
    if(speed.is_zero_approx()) {
        sprite_rect->set_material(memnew(ShaderMaterial));
        scroll_shader.unref();
        return;
    }
    Ref<ShaderMaterial> shader = memnew(ShaderMaterial);
    shader->set_shader(loader->load("res://Shaders/default/scroll_texture.gdshader"));
    shader->set_shader_parameter("speed", speed);
    sprite_rect->set_material(shader);
    shader_time = 0;
    scroll_shader = shader;
}

Ref<PropertyTweener> Bone::tween_height(float new_height, float time) {
    if (h_tween.is_valid()) h_tween->kill();
    h_tween = create_tween();
    return h_tween->tween_property(sprite_rect, "size:y", new_height, time);
}

Ref<PropertyTweener> Bone::tween_width(float new_width, float time) {
    if (w_tween.is_valid()) w_tween->kill();
    w_tween = create_tween();
    return w_tween->tween_property(sprite_rect, "size:x", new_width, time);
}