#ifndef __TARGETING_BOX_BEHAVIOUR_H__
#define __TARGETING_BOX_BEHAVIOUR_H__

#include "box_behaviour.h"
namespace godot {
    class TargetingBoxBehaviour : public BattleBoxBehaviour {
        GDCLASS(TargetingBoxBehaviour, BattleBoxBehaviour);

        protected:
            static void _bind_methods();
        
        public:
            void _on_gain_control() override;
            void _on_lose_control() override;
            void input(const Ref<InputEvent>& event) override;
    };
}

#endif