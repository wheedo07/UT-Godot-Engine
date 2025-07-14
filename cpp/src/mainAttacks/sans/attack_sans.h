#ifndef AttackSans_H
#define AttackSans_H

#include "engine/Battle/Attacks/attacks.h"
#include "engine/Battle/Bullets/custom/bone.h"
#include "engine/Battle/Bullets/custom/bone_spike.h"
#include "engine/Battle/Bullets/custom/blaster.h"
#include "engine/Battle/Bullets/battle_platform.h"
namespace godot {
    class AttackSans : public AttackBase {
        GDCLASS(AttackSans, AttackBase);

        protected:
            static void _bind_methods();

        private:
            Ref<PackedScene> bone;
            Ref<PackedScene> bone_spike;
            Ref<PackedScene> blaster;
            Ref<PackedScene> platform;

            void turn_1();
            void turn_2();

        public:
            AttackSans();
            ~AttackSans();

            void ready() override;
            void start_attack() override;

            Bone* create_bone(Masking type);
            BoneSpike* create_boneSpike(Masking type);
            Blaster* create_blaster(Masking type);
            BattlePlatform* create_platform(Masking type);
    };
}

#endif