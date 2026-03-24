#ifndef __CONSUMABLE_H__
#define __CONSUMABLE_H__

#include "item.h"
namespace godot {
    class RecoveryItem : public Item {
        GDCLASS(RecoveryItem, Item);

        protected:
            static void _bind_methods();

        private:
            int heal_amount;
            int attack_amount;
            int defense_amount;

        public:
            RecoveryItem();
            ~RecoveryItem();

            void set_heal_amount(int p_amount);
            int get_heal_amount();

            void set_attack_amount(int p_amount);
            int get_attack_amount();

            void set_defense_amount(int p_amount);
            int get_defense_amount();
    };
}

#endif