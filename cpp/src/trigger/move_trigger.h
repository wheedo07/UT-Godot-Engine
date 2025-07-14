#ifndef MOVE_TRIGGER_H
#define MOVE_TRIGGER_H

#include "engine/Overworld/Interactions/overworld_area_trigger.h"
namespace godot {
    class MoveTrigger : public OverworldAreaTrigger {
        GDCLASS(MoveTrigger, OverworldAreaTrigger);
        
        protected:
            static void _bind_methods();

        private:
            PackedStringArray text;
        
        public:
            MoveTrigger();
            ~MoveTrigger();

            void _on_interact_move();
            void finished_interact_move();

            void set_text(PackedStringArray value);
            PackedStringArray get_text();
    };
}

#endif