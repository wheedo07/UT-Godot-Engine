#ifndef __ARMOR_H__
#define __ARMOR_H__

#include "item.h"
namespace godot {
    class Armor : public Item {
        GDCLASS(Armor, Item);

        protected:
            static void _bind_methods();

        private:
            int attack_amount;
            int defense_amount;

        public:
            Armor();
            ~Armor();

            void set_attack_amount(int p_amount);
            int get_attack_amount();

            void set_defense_amount(int p_amount);
            int get_defense_amount();
    };
}

#endif