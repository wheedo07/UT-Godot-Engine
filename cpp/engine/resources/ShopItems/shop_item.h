#ifndef SHOP_ITEM_H
#define SHOP_ITEM_H
#include<godot_cpp/classes/resource.hpp>

namespace godot {
    class ShopItem : public Resource {
        GDCLASS(ShopItem, Resource);

        protected:
            static void _bind_methods();
        
        private:
            int item;
            int cost;
        
        public:
            ShopItem();
            ~ShopItem();

            void set_item(int p_item);
            int get_item() const;

            void set_cost(int p_cost);
            int get_cost() const;
    };
}

#endif