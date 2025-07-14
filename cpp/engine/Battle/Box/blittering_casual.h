#ifndef BLITTERING_CASUAL_BOX_BEHAVIOUR_H
#define BLITTERING_CASUAL_BOX_BEHAVIOUR_H

#include "box_behaviour.h"
namespace godot {
    class BlitteringCasualBoxBehaviour : public BattleBoxBehaviour {
        GDCLASS(BlitteringCasualBoxBehaviour, BattleBoxBehaviour);

        protected:
            static void _bind_methods();
        
        public:
            void _on_gain_control() override;
            void _on_lose_control() override;
    };
}

#endif