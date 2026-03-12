#ifndef ENCOUNTER_SCRIPT_H
#define ENCOUNTER_SCRIPT_H

#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/core/binder_common.hpp>
#include<godot_cpp/core/gdvirtual.gen.inc>
#include "battle_system.h"
namespace godot {
    class EncounterScript : public Node {
        GDCLASS(EncounterScript, Node);

        protected:
            static void _bind_methods();

        public:
            BattleMain *main;
            BattleBox *box;
            BattleHUD *hub;
            SoulBattle *soul;
            TextureRect *bg;
            AttackManager *attacks;
            CameraController *camera;

        private:
            void set_property(Variant value);
            BattleMain *get_main();
            BattleBox *get_box();
            BattleHUD *get_hub();
            SoulBattle *get_soul();
            TextureRect *get_bg();
            AttackManager *get_attacks();
            CameraController *get_camera();
            bool get_player_turn();

        public:
            EncounterScript();
            ~EncounterScript();

            void _ready() override;

            virtual void ready();
            GDVIRTUAL0(ready);
    };
}

#endif