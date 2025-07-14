#ifndef FIGHTING_BOX_BEHAVIOUR_H
#define FIGHTING_BOX_BEHAVIOUR_H

#include "box_behaviour.h"
namespace godot {
    class FightingBoxBehaviour : public BattleBoxBehaviour {
        GDCLASS(FightingBoxBehaviour, BattleBoxBehaviour);

        protected:
            static void _bind_methods();
        
        public:
            void _on_gain_control() override;
            void _on_lose_control() override;
    };
}

#endif