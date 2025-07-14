#include "shop_item.h"
using namespace godot;

ShopItem::ShopItem() {
    item = 0;
    cost = 10;
}

ShopItem::~ShopItem() {}

void ShopItem::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_item", "item"), &ShopItem::set_item);
    ClassDB::bind_method(D_METHOD("get_item"), &ShopItem::get_item);
    ClassDB::bind_method(D_METHOD("set_cost", "cost"), &ShopItem::set_cost);
    ClassDB::bind_method(D_METHOD("get_cost"), &ShopItem::get_cost);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "item", PROPERTY_HINT_NONE, "The item id corrsesponding to an item id in Globbal.item_list"), 
                "set_item", "get_item");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "cost"), "set_cost", "get_cost");
}

void ShopItem::set_item(int p_item) {
    item = p_item;
}

int ShopItem::get_item() const {
    return item;
}

void ShopItem::set_cost(int p_cost) {
    cost = p_cost;
}

int ShopItem::get_cost() const {
    return cost;
}