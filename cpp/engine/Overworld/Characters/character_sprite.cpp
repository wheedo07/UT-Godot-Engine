#include "character_sprite.h"
#include "env.h"

CharacterSprite::CharacterSprite() {
    animation_names = {
        "act", 
        "idle_down", "idle_side", "idle_up",
        "move_down", "move_side", "move_up"
    };
    alert_offset = Vector2(0, -10);
}

CharacterSprite::~CharacterSprite() {}

void CharacterSprite::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_alert_offset", "offset"), &CharacterSprite::set_alert_offset);
    ClassDB::bind_method(D_METHOD("get_alert_offset"), &CharacterSprite::get_alert_offset);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "alert_offset"), "set_alert_offset", "get_alert_offset");
}

void CharacterSprite::_process(double delta) {
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

void CharacterSprite::set_alert_offset(Vector2 p_offset) {
    alert_offset = p_offset;
}

Vector2 CharacterSprite::get_alert_offset() const {
    return alert_offset;
}