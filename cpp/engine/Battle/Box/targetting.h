#ifndef TARGETTING_BOX_BEHAVIOUR_H
#define TARGETTING_BOX_BEHAVIOUR_H

#include "box_behaviour.h"
namespace godot {
    class TargettingBoxBehaviour : public BattleBoxBehaviour {
        GDCLASS(TargettingBoxBehaviour, BattleBoxBehaviour);

        protected:
            static void _bind_methods();
        
        public:
            void _on_gain_control() override;
            void _on_lose_control() override;
            void input(const Ref<InputEvent>& event) override;
    };
}

#endif