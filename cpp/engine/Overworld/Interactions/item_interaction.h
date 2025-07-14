#ifndef ITEM_INTERACTION_H
#define ITEM_INTERACTION_H

#include "interaction_trigger.h"
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/variant/packed_string_array.hpp>
namespace godot {
    class ItemInteraction : public InteractionTrigger {
        GDCLASS(ItemInteraction, InteractionTrigger)
        
        protected:
            static void _bind_methods();
        
        public:
            enum Decisions {
                ITEM_PICK_UP,
                ITEM_LEAVE_ALONE,
            };
        
        private:
            PackedStringArray discover_text;
            PackedStringArray accept_text;
            PackedStringArray reject_text;
            PackedStringArray full_text;
            PackedStringArray option_names;

            int item_id;
            bool one_item;

            Ref<PackedScene> text_box;

            void _on_text_box_selected_option(int decision);
            
        public:
            ItemInteraction();
            ~ItemInteraction();
            
            void _ready() override;
            
            void discover();
            void disable_item();
            
            void set_discover_text(const PackedStringArray& p_text);
            PackedStringArray get_discover_text() const;
            
            void set_accept_text(const PackedStringArray& p_text);
            PackedStringArray get_accept_text() const;
            
            void set_reject_text(const PackedStringArray& p_text);
            PackedStringArray get_reject_text() const;
            
            void set_full_text(const PackedStringArray& p_text);
            PackedStringArray get_full_text() const;
            
            void set_option_names(const PackedStringArray& p_names);
            PackedStringArray get_option_names() const;
            
            void set_item_id(int p_id);
            int get_item_id() const;
            
            void set_one_item(bool p_one_item);
            bool get_one_item() const;
    };
}
VARIANT_ENUM_CAST(godot::ItemInteraction::Decisions);

#endif