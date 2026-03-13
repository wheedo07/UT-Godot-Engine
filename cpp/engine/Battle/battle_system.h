#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "slash.h"
#include "enemy.h"
#include "hud.h"
#include "battle_buttons.h"
#include "Soul/soul.h"
#include "Attacks/attacks_manager.h"
#include "engine/resources/Encounters/encounter.h"
#include "engine/Camera/camera_controller.h"
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/color_rect.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
namespace godot {
    class EncounterScript;

    class BattleMain : public Node2D {
        GDCLASS(BattleMain, Node2D);
        
        protected:
            static void _bind_methods();

        public:
            CameraController *camera;
            BattleButtons *buttons;
            BattleBox *box;
            Node2D *enemies_node;
            TextureRect *bg;
            SoulBattle *soul_battle;
            AttackManager *attacks;
            Node *attacks_parent;
            BattleHUD *hud;
            EncounterScript *script_node;
            int turn_number;
            bool transparent, player_turn;
        
        private:
            AudioStreamPlayer *music_player, *lvlup_sound;

            Ref<PackedScene> attack_scene;
            Ref<PackedScene> slash_scene;
            Ref<PackedScene> damage_info_scene;

            Ref<Encounter> encounter;
            Dictionary rewards;
            Array enemy_names;
            Array enemies;
            Array enemies_def;
            Array enemies_max_hp;
            Array enemies_script;
            bool kr, is_first_turn;
            int completed_enemies, completed_size;

            void initialize();

            void _on_player_turn_start();
            void _on_enemy_turn_start();
            void _on_spare_finished(Enemy *enemy);
            void _no_enemies_handler();
            
            void _on_slash_finished(int damage, int target, bool crit);
            void _on_damage_info_completed(int target, bool miss);
            void _on_fight_used_completed(int target);
            void _on_action(const String& action);
            void _on_kill_enemy(Enemy *enemy);
            void _on_end(bool mercy=false, int id=-1);
            void _on_get_turn();
            void _on_end_turn();
            void _on_transparent();
            void _encounter_script_add(bool on);

            void set_property(Variant value);
            void set_turn_number(int value);
            int get_turn_number();
            BattleButtons *get_buttons();

        public:
            BattleMain();
            ~BattleMain();
            
            void _ready() override;
            void _process(double delta) override;
            
            void _fight(int target);
            void _hit(int damage, int target, bool crit = false);
            void _miss(int target);
            void _act(int target, int option);
            void _mercy(int choice);
            void _item(int item_id);
            void _modify_stats(int id, Dictionary stats);
            void _enemy_script_off();
            PackedStringArray _on_death_player();

            // 사용 함수
            void toggle_transparent();
            void end_encounter();
            int enemy_size();
            bool is_kr();
            void add_enemy(Ref<PackedScene> enemy_scene);
            void kill_enemy(int enemy_id = 0);
            void spare_enemy(int enemy_id = 0);
            bool check_end_encounter();
            bool check_enemy_solo();
            void reset_pos_camera();
            
            void set_encounter(const Ref<Encounter>& p_encounter);
            Ref<Encounter> get_encounter() const;
    };
}

#endif