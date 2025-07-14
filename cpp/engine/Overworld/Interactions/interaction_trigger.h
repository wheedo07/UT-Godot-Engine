#ifndef INTERACTION_TRIGGER_H
#define INTERACTION_TRIGGER_H

#include<godot_cpp/classes/area2d.hpp>
namespace godot {
    class InteractionTrigger : public Area2D {
        GDCLASS(InteractionTrigger, Area2D)
        
        protected:
            static void _bind_methods();
        
        public:
            InteractionTrigger();
            ~InteractionTrigger();

            void enable();
            void disable();
    };
}

#endif