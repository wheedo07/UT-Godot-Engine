#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "item.h"
#include<godot_cpp/classes/packed_scene.hpp>

#define WEAPON_DEFAULT_SPEED 1.5f
#define WEAPON_DEFAULT_BARS 1
#define WEAPON_DEFAULT_DELAY 0
#define WEAPON_DEFAULT_CRIT false
namespace godot {
    class Weapon : public Item {
        GDCLASS(Weapon, Item)

        protected:
            static void _bind_methods();

        private:
            int attack_amount;
            int defense_amount;
            
            float weapon_speed;
            int weapon_bars;
            int weapon_delay;
            bool critical_hits;

            Ref<PackedScene> slash_scene;
        
        public:
            Weapon();
            ~Weapon();

            void _get_property_list(List<PropertyInfo> *p_list) const;
            bool _set(const StringName& p_name, const Variant& p_value);
            bool _get(const StringName& p_name, Variant& r_ret);

            void set_attack_amount(int p_amount);
            int get_attack_amount();
            
            void set_defense_amount(int p_amount);
            int get_defense_amount();

            void set_weapon_speed(float value);
            float get_weapon_speed();

            void set_weapon_bars(int value);
            int get_weapon_bars();

            void set_weapon_delay(int value);
            int get_weapon_delay();

            void set_critical_hits(bool value);
            bool get_critical_hits();

            Ref<PackedScene> get_slash_scene();
    };
}

#endif