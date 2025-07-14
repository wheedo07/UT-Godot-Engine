#include "menu_soul.h"
using namespace godot;

MenuSoul::MenuSoul() {}

MenuSoul::~MenuSoul() {
    if (!tw.is_null() && tw->is_valid()) {
        tw->kill();
    }
}

void MenuSoul::_bind_methods() {
    ClassDB::bind_method(D_METHOD("move_global", "pos"), &MenuSoul::move_global);
    ClassDB::bind_method(D_METHOD("move", "pos"), &MenuSoul::move);
}

void MenuSoul::move_global(const Vector2& pos) {
    if (!tw.is_null() && tw->is_valid()) {
        tw->kill();
    }
    
    tw = create_tween()->set_trans(Tween::TRANS_CUBIC);
    tw->tween_property(this, "global_position", pos, TIME);
}

void MenuSoul::move(const Vector2& pos) {
    if (!tw.is_null() && tw->is_valid()) {
        tw->kill();
    }
    
    tw = create_tween();
    tw->tween_property(this, "position", pos, TIME);
}