#include "cyan_detection.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/area2d.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

CyanDetection::CyanDetection() {
    can_move = false;
    isEditor = false;
}

CyanDetection::~CyanDetection() {}

void CyanDetection::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_property", "value"), &CyanDetection::set_property);
    ClassDB::bind_method(D_METHOD("get_can_move"), &CyanDetection::get_can_move);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_move", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_can_move");
}

void CyanDetection::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    sprite = Object::cast_to<Sprite2D>(get_node_internal("Sprite2D"));
    glow = Object::cast_to<GPUParticles2D>(get_node_internal("Sprite2D/Glow"));
}

void CyanDetection::_process(double delta) {
    if(isEditor) return;
    can_move = false;
   
    glow->set_emitting(false);
    TypedArray<Area2D> areas = get_overlapping_areas();
    for(int i=0; i < areas.size(); i++) {
        Area2D* area = Object::cast_to<Area2D>(areas[i]);
        
        if(area && area->is_class("BulletArea")) {
            Color modulate = sprite->get_modulate();
            modulate.a = Math::lerp(modulate.a, 1, float(delta) * SPEED);
            sprite->set_modulate(modulate);
            
            can_move = true;
            glow->set_emitting(true);
        }else {
            Color modulate = sprite->get_modulate();
            modulate.a = Math::lerp(modulate.a, 0, float(delta) * SPEED * 2.0f);
            sprite->set_modulate(modulate);
        }
    }
    
    if (areas.size() == 0 && sprite) {
        Color modulate = sprite->get_modulate();
        modulate.a = Math::lerp(modulate.a, 0, float(delta) * SPEED * 2.0f);
        sprite->set_modulate(modulate);
    }
}

void CyanDetection::set_property(bool value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

bool CyanDetection::get_can_move() const {
    return can_move;
}