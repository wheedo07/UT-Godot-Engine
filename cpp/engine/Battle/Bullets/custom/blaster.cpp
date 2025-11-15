#include "blaster.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
using namespace godot;

#define SPEED 1000 
#define GROW_TIME 0.2f
#define BEAM_COLLISION_MARGIN 6.0f
Blaster::Blaster() {
    beam = nullptr;
    rect = nullptr;
    anim_player = nullptr;
    tween_trans = Tween::TRANS_QUAD;
    tween_ease = Tween::EASE_IN_OUT;
    target_position = Vector2(0, 0);
}

Blaster::~Blaster() {}

void Blaster::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished_blast"));
    ClassDB::bind_method(D_METHOD("fire", "target", "size", "time", "delay", "duration", "up_delay"), &Blaster::fire, DEFVAL(1.0f), DEFVAL(0.7f), DEFVAL(0.5f), DEFVAL(0.5f), DEFVAL(0.1f));
    ClassDB::bind_method(D_METHOD("_blast", "duration", "up_delay"), &Blaster::_blast);
    ClassDB::bind_method(D_METHOD("_on_blast_finished"), &Blaster::_on_blast_finished);

    ClassDB::bind_method(D_METHOD("set_tweenTrans", "value"), &Blaster::set_tweenTrans);
    ClassDB::bind_method(D_METHOD("get_tweenTrans"), &Blaster::get_tweenTrans);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenTrans", PROPERTY_HINT_ENUM, "Linear,Sine,Quint,Quart,Quad,Expo,Elastic,Cubic,Circ,Bounce,Back,Spring"), "set_tweenTrans", "get_tweenTrans");
    
    ClassDB::bind_method(D_METHOD("set_tweenEase", "value"), &Blaster::set_tweenEase);
    ClassDB::bind_method(D_METHOD("get_tweenEase"), &Blaster::get_tweenEase);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenEase", PROPERTY_HINT_ENUM, "In,Out,InOut,OutIn"), "set_tweenEase", "get_tweenEase");

    ClassDB::bind_method(D_METHOD("set_property", "value"), &Blaster::set_property);
    ClassDB::bind_method(D_METHOD("get_target_position"), &Blaster::get_target_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "target_position", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_target_position");
}

void Blaster::_ready() {
    Bullet::_ready();
    beam = Object::cast_to<Control>(get_node_internal("Sprite/Beam"));
    rect = Object::cast_to<NinePatchRect>(get_node_internal("Sprite/Beam/NinePatchRect"));
    anim_player = Object::cast_to<AnimationPlayer>(get_node_internal("AnimationPlayer"));
    collision = Object::cast_to<CollisionShape2D>(get_node_internal("Area2D/CollisionShape2D"));
    
    Ref<RectangleShape2D> shape = memnew(RectangleShape2D);
    shape->set_size(Vector2(0, 0));
    collision->set_shape(shape);
}

Blaster* Blaster::fire(const Vector2& target, float size, float time, float delay, float duration, float up_delay) {
    Object::cast_to<AudioStreamPlayer>(get_node_internal("load"))->play();
    set_scale(Vector2(Math::max(size, 1.0f), Math::max(size, 1.5f)));
    
    target_position = target;
    Vector2 distance;
    if(get_parent()->is_class("Node2D"))
        distance = target_position - get_global_position();
    else distance = target_position - get_position();
    
    velocity_tween = create_tween()->set_ease(tween_ease)->set_trans(tween_trans)->set_parallel();
    velocity_tween->tween_property(this, "position", distance, time)->as_relative();
    velocity_tween->tween_property(this, "rotation", Math_TAU, time)->as_relative();
    velocity_tween->chain()->tween_interval(delay - time);
    velocity_tween->chain()->tween_callback(Callable(anim_player, "play").bind("prepare"));
    velocity_tween->tween_interval(0.15);
    velocity_tween->chain()->tween_callback(Callable(Object::cast_to<AudioStreamPlayer>(get_node_internal("fire")), "play"));
    velocity_tween->tween_callback(Callable(this, "_blast").bind(duration, up_delay));
    
    return this;
}

void Blaster::_blast(float duration, float up_delay) {
    Ref<RectangleShape2D> shape = collision->get_shape();
    
    Vector2 beam_size = beam->get_size();
    shape->set_size(Vector2(beam_size.x, beam_size.y - Math::max(BEAM_COLLISION_MARGIN, 0.0f)));
    collision->set_scale(Vector2(0, 1));
    
    Vector2 collision_pos = collision->get_position();
    collision_pos.y += beam_size.y / 2.0f;
    collision->set_position(collision_pos);
    beam->show();
    collision->set_disabled(false);
    
    anim_player->play("fire");
    
    Ref<Tween> tween_beam = create_tween()->set_trans(Tween::TRANS_QUAD)->set_loops();
    tween_beam->pause();
    tween_beam->tween_property(rect, "scale:x", 0.75f, GROW_TIME);
    tween_beam->tween_property(rect, "scale:x", 1.0f, GROW_TIME);
    
    Ref<Tween> tw_remove = create_tween()->set_trans(Tween::TRANS_QUAD)->set_parallel(true);
    tw_remove->tween_interval(Math::max(duration, GROW_TIME));
    tw_remove->chain();
    tw_remove->tween_property(beam, "modulate:a", 0, GROW_TIME)->set_trans(Tween::TRANS_LINEAR);
    tw_remove->tween_property(collision, "scale:x", 0, GROW_TIME);
    tw_remove->tween_callback(Callable(this, "_on_blast_finished"))->set_delay(GROW_TIME / 2.0f);
    tw_remove->tween_property(beam, "scale:x", 0, GROW_TIME);
    tw_remove->chain()->tween_callback(Callable(this, "kill"))->set_delay(1.8 + up_delay);

    Ref<Tween> tw = create_tween()->set_trans(Tween::TRANS_QUAD)->set_parallel(true);
    tw->tween_property(beam, "scale:x", 1.0f, GROW_TIME);
    tw->tween_property(collision, "scale:x", 1.0f, GROW_TIME);
    tw->tween_property(beam, "modulate:a", 1, GROW_TIME)->set_trans(Tween::TRANS_LINEAR);
    tw->tween_callback(Callable(tween_beam.ptr(), "play"));
    
    Ref<Tween> tw_move = create_tween()->set_trans(Tween::TRANS_SINE);
    Vector2 up_vector = Vector2(0, -1);
    Vector2 relative_pos = up_vector.rotated(get_rotation()) * SPEED;
    tw_move->chain()->tween_interval(up_delay);
    tw_move->tween_property(this, "position", relative_pos, 1.0f)->as_relative();
}

void Blaster::_on_blast_finished() {
    collision->set_disabled(true);
    emit_signal("finished_blast");
}

void Blaster::set_tweenTrans(Tween::TransitionType value) {
    tween_trans = value;
}

Tween::TransitionType Blaster::get_tweenTrans() const {
    return tween_trans;
}

void Blaster::set_tweenEase(Tween::EaseType value) {
    tween_ease = value;
}

Tween::EaseType Blaster::get_tweenEase() const {
    return tween_ease;
}

void Blaster::set_property(const Variant& value) {
    ERR_PRINT("이 속성은 읽기 전용입니다.");
}

Vector2 Blaster::get_target_position() const {
    return target_position;
}