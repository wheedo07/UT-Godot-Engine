#ifndef OVERWORLD_AREA_TRIGGER_H
#define OVERWORLD_AREA_TRIGGER_H

#include<godot_cpp/classes/area2d.hpp>
#include<godot_cpp/classes/collision_shape2d.hpp>
namespace godot {
    class OverworldAreaTrigger : public Area2D {
        GDCLASS(OverworldAreaTrigger, Area2D)

        protected:
            static void _bind_methods();
        
        private:
            StringName required_collider_group;
            int action;
            String new_room;
            int new_room_entrance;
            CollisionShape2D* Collision;
        
        public:
            enum Action {
                ROOM_CHANGE = 1,
                STOP_MOVEMENT = 2,
                CUTSCENE = 4,
                ONE_SHOT = 8,
                NON_ROOM_SCENE_CHANGE = 16
            };
            OverworldAreaTrigger();
            ~OverworldAreaTrigger();

            virtual void _ready() override;
            void _on_area_entered(Area2D* body);
            void _on_body_entered(Node* body);
            void _successful_enter();
            void disable();

            void set_required_collider_group(const StringName& p_group);
            StringName get_required_collider_group() const;

            void set_action(int p_action);
            int get_action() const;

            void set_new_room(const String& p_room);
            String get_new_room() const;

            void set_new_room_entrance(int p_entrance);
            int get_new_room_entrance() const;
    };
}
VARIANT_ENUM_CAST(godot::OverworldAreaTrigger::Action);

#endif