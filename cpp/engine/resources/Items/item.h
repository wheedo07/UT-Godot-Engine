#ifndef __ITEM_H__
#define __ITEM_H__

#include<godot_cpp/classes/resource.hpp>
namespace godot {
    class Item : public Resource {
        GDCLASS(Item, Resource)

        protected:
            static void _bind_methods();
        
        private:
            String item_name;
            bool isConsumable;
            
            PackedStringArray use_message;
            PackedStringArray item_information;
            PackedStringArray throw_message;

        public:
            Item();
            ~Item();

            void _get_property_list(List<PropertyInfo> *p_list) const;
            bool _set(const StringName& p_name, const Variant& p_value);
            bool _get(const StringName& p_name, Variant& r_ret);
            
            void set_item_name(const String& p_name);
            String get_item_name() const;
            String get_item_name_tr() const;

            void set_consumable(bool p_consumable);
            bool is_consumable() const;

            void set_use_message(const PackedStringArray& p_message);
            PackedStringArray get_use_message() const;
            
            void set_item_information(const PackedStringArray& p_info);
            PackedStringArray get_item_information() const;
            
            void set_throw_message(const PackedStringArray& p_message);
            PackedStringArray get_throw_message() const;
    };
}

#endif