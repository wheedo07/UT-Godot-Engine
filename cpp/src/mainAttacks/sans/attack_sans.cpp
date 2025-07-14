#include "attack_sans.h"
using namespace godot;

AttackSans::AttackSans() {}

AttackSans::~AttackSans() {}

void AttackSans::_bind_methods() {
    ClassDB::bind_method(D_METHOD("ready"), &AttackSans::ready);
    ClassDB::bind_method(D_METHOD("start_attack"), &AttackSans::start_attack);
    ADD_SIGNAL(MethodInfo("throws",
        PropertyInfo(Variant::VECTOR2, "direction"),
        PropertyInfo(Variant::INT, "power")
    ));
}

void AttackSans::ready() {
    ResourceLoader* loader = ResourceLoader::get_singleton();
    bone = loader->load("res://Battle/Bullets/Bone/bone.tscn");
    bone_spike = loader->load("res://Battle/Bullets/BoneSpike/bone_spike.tscn");
    blaster = loader->load("res://Battle/Bullets/Blaster/blaster.tscn");
    platform = loader->load("res://Battle/Platform/platform.tscn");
}

void AttackSans::start_attack() {
    if(main->turn_number % 2 == 0)
        turn_2();
    else turn_1();
}

Bone* AttackSans::create_bone(Masking type) {
    Bone* boneObject = Object::cast_to<Bone>(bone->instantiate());
    add_bullet(boneObject, type);
    return boneObject;
}

BoneSpike* AttackSans::create_boneSpike(Masking type) {
    BoneSpike* spikeObject = Object::cast_to<BoneSpike>(bone_spike->instantiate());
    add_bullet(spikeObject, type);
    return spikeObject;
}

Blaster* AttackSans::create_blaster(Masking type) {
    Blaster* blasterObject = Object::cast_to<Blaster>(blaster->instantiate());
    add_bullet(blasterObject, type);
    return blasterObject;
}

BattlePlatform* AttackSans::create_platform(Masking type) {
    BattlePlatform* platformObject = Object::cast_to<BattlePlatform>(platform->instantiate());
    add_bullet(platformObject, type);
    return platformObject;
}