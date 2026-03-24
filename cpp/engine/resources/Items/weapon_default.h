#ifndef __WEAPON_DEFAULT_H__
#define __WEAPON_DEFAULT_H__

#include "weapon.h"
namespace godot {
    class WeaponDefault : public Weapon {
        GDCLASS(WeaponDefault, Weapon);

        protected:
            static void _bind_methods();

        public:
            enum WeaponAnimation {
                KNIFE,
                PUNCH,
                SHOE,
                BOOK,
                PAN,
                GUN
            };
        
        private:
            WeaponAnimation weapon_animation;

        public:
            WeaponDefault();
            ~WeaponDefault();

            void set_weapon_animation(WeaponAnimation value);
            WeaponAnimation get_weapon_animation();
    };
}
VARIANT_ENUM_CAST(godot::WeaponDefault::WeaponAnimation);

#endif