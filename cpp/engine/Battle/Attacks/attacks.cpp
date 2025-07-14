#include "attacks.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>

AttackBase::AttackBase() {
    attack_id = 0;
}

AttackBase::~AttackBase() {}

void AttackBase::_bind_methods() {
    // 스크립트 메소드
    BIND_VIRTUAL_METHOD(AttackBase, ready, hash_djb2("AttackBase_ready"));
    BIND_VIRTUAL_METHOD(AttackBase, start_attack, hash_djb2("AttackBase_start_attack"));

    ADD_SIGNAL(MethodInfo("remove_attack", PropertyInfo(Variant::INT, "id")));
    ADD_SIGNAL(MethodInfo("remove_bullets"));

    BIND_ENUM_CONSTANT(ABSOLUTE_CLIP);
    BIND_ENUM_CONSTANT(ABSOLUTE);
    BIND_ENUM_CONSTANT(RELATIVE_BOX_CLIP);
    BIND_ENUM_CONSTANT(RELATIVE_BOX);

    ClassDB::bind_method(D_METHOD("add_bullet", "bullet", "mask"), &AttackBase::add_bullet, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("quick_bullet", "bullet_scene", "pos", "rot", "mask"), &AttackBase::quick_bullet, DEFVAL(0.0f), DEFVAL(ABSOLUTE_CLIP));
    ClassDB::bind_method(D_METHOD("end_attack"), &AttackBase::end_attack);
   
    ClassDB::bind_method(D_METHOD("get_attack_id"), &AttackBase::get_attack_id);
    ClassDB::bind_method(D_METHOD("set_attack_id", "attack_id"), &AttackBase::set_attack_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_id"), "set_attack_id", "get_attack_id");

    // 속성
    ClassDB::bind_method(D_METHOD("set_property", "value"), &AttackBase::set_property);
    ClassDB::bind_method(D_METHOD("get_main"), &AttackBase::get_main);
    ClassDB::bind_method(D_METHOD("get_box"), &AttackBase::get_box);
    ClassDB::bind_method(D_METHOD("get_soul"), &AttackBase::get_soul);
    ClassDB::bind_method(D_METHOD("get_mask"), &AttackBase::get_mask);
    ClassDB::bind_method(D_METHOD("get_camera"), &AttackBase::get_camera);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "main", PROPERTY_HINT_NONE, "BattleMain", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_main");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "box", PROPERTY_HINT_NONE, "BattleBox", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_box");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "soul", PROPERTY_HINT_NONE, "SoulBattle", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_soul");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mask", PROPERTY_HINT_NONE, "AttackManager", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_mask");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NONE, "CameraRemoteController", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_camera");
}

void AttackBase::_ready() {
    if(isEditor) return;
    main = Object::cast_to<BattleMain>(global->get_scene_container()->get_current_scene());
    non_mask = main->attacks_parent;
    mask = main->attacks;
    camera = main->camera;
    box = main->box;
    box_rect_clip = box->rect_clip;
    box_rect_no_clip = box->rect_no_clip;
    soul = main->soul_battle;

    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    }else {
        ready();
    }
}

void AttackBase::ready() {}

void AttackBase::start_attack() {}

void AttackBase::add_bullet(Node* bullet, Masking mask_value) {
    if (!bullet) {
        UtilityFunctions::push_error("Trying to add null bullet");
        return;
    }
    
    if (bullet->has_signal("shake_camera") && camera) {
        bullet->connect("shake_camera", Callable(camera, "add_shake"));
    }
    
    if (bullet->has_method("fade")) {
        connect("remove_bullets", Callable(bullet, "fade"));
    }
    
    switch (mask_value) {
        case ABSOLUTE:
            if (non_mask) {
                non_mask->add_child(bullet, true);
                non_mask->move_child(bullet, 0);
            }
            break;
        case ABSOLUTE_CLIP:
            if (mask) {
                mask->add_child(bullet, true);
            }
            break;
        case RELATIVE_BOX:
            if (box_rect_no_clip) {
                box_rect_no_clip->add_child(bullet, true);
            }
            break;
        case RELATIVE_BOX_CLIP:
            if (box_rect_clip) {
                box_rect_clip->add_child(bullet, true);
            }
            break;
    }
}

Node* AttackBase::quick_bullet(const Ref<PackedScene>& bullet_scene, const Vector2& pos, float rot, Masking mask_value) {
    if (!bullet_scene.is_valid()) {
        ERR_PRINT("scene을 가져올수 없음");
        return nullptr;
    }
    
    Node* bullet = bullet_scene->instantiate();
    if (!bullet) {
        ERR_PRINT("bullet의 값이 null임");
        return nullptr;
    }
    
    add_bullet(bullet, mask_value);
    
    bullet->call("set_position", pos);
    bullet->call("set_rotation_degrees", rot);
    
    return bullet;
}

void AttackBase::end_attack() {
    emit_signal("remove_bullets");
    emit_signal("remove_attack", attack_id);
}

int AttackBase::get_attack_id() const {
    return attack_id;
}

void AttackBase::set_attack_id(int p_attack_id) {
    attack_id = p_attack_id;
}

void AttackBase::set_property(Object* value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

BattleMain* AttackBase::get_main() {
    return main;
}

BattleBox* AttackBase::get_box() {
    return box;
}

SoulBattle* AttackBase::get_soul() {
    return soul;
}

AttackManager* AttackBase::get_mask() {
    return mask;
}

CameraRemoteController* AttackBase::get_camera() {
    return camera;
}