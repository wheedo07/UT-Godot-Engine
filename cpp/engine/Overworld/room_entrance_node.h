#ifndef ROOM_ENTRANCE_NODE_H
#define ROOM_ENTRANCE_NODE_H
#include<godot_cpp/classes/reference_rect.hpp>
#include "engine/Overworld/Interactions/overworld_area_trigger.h"
namespace godot {
    class RoomEntranceNode : public ReferenceRect {
        GDCLASS(RoomEntranceNode, ReferenceRect)

        protected:
            static void _bind_methods();
        
        private:
            Vector2 facing_direction;
            int door_id;
            int door_margin;
            String new_room;
            bool special_room;
            int new_room_entrance_id;

            OverworldAreaTrigger* Area;
        
        public:
            RoomEntranceNode();
            ~RoomEntranceNode();
            
            void force_enter();
            virtual void _ready() override;
            
            void set_facing_direction(const Vector2& p_direction);
            Vector2 get_facing_direction() const;
            
            void set_door_id(int p_id);
            int get_door_id() const;
            
            void set_door_margin(int p_margin);
            int get_door_margin() const;
            
            void set_new_room(const String& p_room);
            String get_new_room() const;
            
            void set_special_room(bool p_special);
            bool get_special_room() const;
            
            void set_new_room_entrance_id(int p_id);
            int get_new_room_entrance_id() const;
    };
}

#endif