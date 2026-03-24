#ifndef ENCOUNTER_SCRIPT_H
#define ENCOUNTER_SCRIPT_H

#include "battle_system.h"
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/core/binder_common.hpp>
#include<godot_cpp/core/gdvirtual.gen.inc>
namespace godot {
    class Item;
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

            /* 사용 함수 */
            Ref<Item> get_player_weapon();
            Ref<Item> get_player_armor();

            virtual void ready();
            GDVIRTUAL0(ready);
    };
}

#endif