#include "overworld_sprite.h"
#include "env.h"

OverworldSprite::OverworldSprite() {
    animation_names = {
        "act", 
        "idle_down", "idle_side", "idle_up",
        "move_down", "move_side", "move_up"
    };
    alert_offset = Vector2(0, -10);
}

OverworldSprite::~OverworldSprite() {}

void OverworldSprite::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_alert_offset", "offset"), &OverworldSprite::set_alert_offset);
    ClassDB::bind_method(D_METHOD("get_alert_offset"), &OverworldSprite::get_alert_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "alert_offset"), "set_alert_offset", "get_alert_offset");
}

void OverworldSprite::_process(double delta) {
    if(!isEditor) return;
    if(get_sprite_frames().is_valid()) return;

    Ref<SpriteFrames> new_sprite = memnew(SpriteFrames);
    for(String anim_name : animation_names) {
        new_sprite->add_animation(anim_name);
    }
    new_sprite->remove_animation("default");
    set_sprite_frames(new_sprite);
    set_animation("idle_down");
}

void OverworldSprite::set_alert_offset(Vector2 p_offset) {
    alert_offset = p_offset;
}

Vector2 OverworldSprite::get_alert_offset() const {
    return alert_offset;
}