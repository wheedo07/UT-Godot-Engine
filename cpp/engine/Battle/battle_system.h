#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "slash.h"
#include "enemy.h"
#include "hud.h"
#include "battle_buttons.h"
#include "Soul/soul.h"
#include "Attacks/attacks_manager.h"
#include "engine/resources/Encounters/encounter.h"
#include "engine/Camera/camera_remote_controller.h"
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/color_rect.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
namespace godot {
    class BattleMain : public Node2D {
        GDCLASS(BattleMain, Node2D);
        
        protected:
            static void _bind_methods();

        public:
            CameraRemoteController* camera;
            BattleButtons* buttons;
            BattleBox* box;
            Node2D* enemies_node;
            TextureRect* bg;
            ColorRect* colorRect;
            SoulBattle* soul_battle;
            AttackManager* attacks;
            Node* attacks_parent;
            BattleHUD* hud;
            int turn_number;
            bool transparent;
        
        private:
            AudioStreamPlayer* music_player;
            AudioStreamPlayer* lvlup_sound;

            Ref<PackedScene> attack_scene;
            Ref<PackedScene> slash_scene;
            Ref<PackedScene> damage_info_scene;

            Ref<Encounter> encounter;
            Dictionary rewards;
            Array enemy_names;
            Array enemies;
            Array enemies_def;
            Array enemies_max_hp;
            bool kr;
            int completed_enemies, completed_size;

            void initialize();
            bool check_end_encounter();
            bool check_enemy_solo();
            String pure_int_to_short_representation(int input);

            void _on_player_turn_start();
            void _on_enemy_turn_start();
            void _on_damage_info_finished();
            void no_enemies_handler();
            
            void _on_slash_finished(int damage, int target, bool crit);
            void _on_damage_info_completed(int target);
            void _on_fight_used_completed(int target);
            void _on_blitter_finished_all_texts();
            void _on_timer_timeout(const String& action);
            void _on_camera_blind_completed();
            void _on_end(bool mercy=false, int id=-1);
            void _finish_encounter();
            void on_get_turn();
            void on_end_turn();

            void set_property(Variant value);
            int get_turn_number();

            void _on_transparent();

        public:
            BattleMain();
            ~BattleMain();
            
            void _ready() override;
            
            void _fight(int target);
            void hit(int damage, int target, bool crit = false);
            void miss(int target);
            void _act(int target, int option);
            void _mercy(int choice);
            void _item(int item_id);

            void toggle_transparent();
            
            void kill_enemy(int enemy_id = 0);
            void spare_enemy(int enemy_id = 0);
            void end_encounter();
            int enemy_size();

            void modify_stats(int id, Dictionary stats);
            
            void set_encounter(const Ref<Encounter>& p_encounter);
            Ref<Encounter> get_encounter() const;
            
            bool is_kr();
            void _on_death_player();
    };
}

#endif