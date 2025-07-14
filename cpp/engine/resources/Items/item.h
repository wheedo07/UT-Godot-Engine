#ifndef ITEM_H
#define ITEM_H

#include<godot_cpp/classes/resource.hpp>
#include<godot_cpp/variant/typed_array.hpp>
namespace godot {
    class Item : public Resource {
        GDCLASS(Item, Resource)

        protected:
            static void _bind_methods();
        
        public:
            enum WeaponType {
                KNIFE,
                PUNCH,
                SHOE,
                BOOK,
                PAN,
                GUN
            };
            enum ItemType {
                CONSUMABLE,
                WEAPON,
                ARMOR
            };
            Item();
            ~Item();
            
            void set_item_type(ItemType p_type);
            ItemType get_item_type() const;
            
            void set_item_name(const String& p_name);
            String get_item_name() const;
            
            void set_weapon_speed(float p_speed);
            float get_weapon_speed() const;
            
            void set_weapon_bars(int p_bars);
            int get_weapon_bars() const;
            
            void set_weapon_type(WeaponType p_type);
            WeaponType get_weapon_type() const;
            
            void set_critical_hits(bool p_critical);
            bool get_critical_hits() const;
            
            void set_use_message(const PackedStringArray& p_message);
            PackedStringArray get_use_message() const;
            
            void set_item_information(const PackedStringArray& p_info);
            PackedStringArray get_item_information() const;
            
            void set_throw_message(const PackedStringArray& p_message);
            PackedStringArray get_throw_message() const;
            
            void set_heal_amount(int p_amount);
            int get_heal_amount() const;
            
            void set_attack_amount(int p_amount);
            int get_attack_amount() const;
            
            void set_defense_amount(int p_amount);
            int get_defense_amount() const;

        private:
            ItemType item_type;
            String item_name;
            float weapon_speed;
            int weapon_bars;
            WeaponType weapon_type;
            bool critical_hits;
            PackedStringArray use_message;
            PackedStringArray item_information;
            PackedStringArray throw_message;
            int heal_amount;
            int attack_amount;
            int defense_amount;
    };
}
VARIANT_ENUM_CAST(godot::Item::WeaponType);
VARIANT_ENUM_CAST(godot::Item::ItemType);

#endif