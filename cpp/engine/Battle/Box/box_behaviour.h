#ifndef BATTLE_BOX_BEHAVIOUR_H
#define BATTLE_BOX_BEHAVIOUR_H

#include "battle_box.h"
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/input_event.hpp>
namespace godot {
    class BattleBoxBehaviour : public Node {
        GDCLASS(BattleBoxBehaviour, Node)
        
        protected:
            static void _bind_methods();
        
        private:
            NodePath box_path;
            bool enabled;
        
        public:
            BattleBox* box;
            bool isEditor;
        
        public:
            BattleBoxBehaviour();
            ~BattleBoxBehaviour();

            virtual void _on_gain_control();
            virtual void _on_lose_control();
            virtual void input(const Ref<InputEvent>& event);
            virtual void _on_changed_state(int new_state);

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;

            void gain_control();
            void lose_control();

            void set_box_path(const NodePath& p_box_path);
            NodePath get_box_path() const;
    };
}

#endif