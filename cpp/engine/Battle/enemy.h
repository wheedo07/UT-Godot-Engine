#ifndef ENEMY_H
#define ENEMY_H

#include "Soul/soul.h"
#include "Box/battle_box.h"
#include "Attacks/attacks_manager.h"
#include "Misc/dialogue_control.h"
#include "engine/Camera/camera_controller.h"
#include "engine/resources/Enemies/enemy_act.h"
#include "engine/resources/Enemies/Act.h"
#include "engine/resources/Dialogues/dialogue_asset.h"
#include<godot_cpp/classes/character_body2d.hpp>
#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/gpu_particles2d.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/random_number_generator.hpp>
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/core/binder_common.hpp>
#include<godot_cpp/core/gdvirtual.gen.inc>
namespace godot {
    class BattleMain;
    class DustTransition;
    class EncounterScript;

    class Enemy : public CharacterBody2D {
        GDCLASS(Enemy, CharacterBody2D);
        
        protected:
            static void _bind_methods();

        public:
            BattleMain *main;
            AttackManager *attacks;
            BattleBox *box;
            SoulBattle *soul;
            DialogueControl *dialogue;
            CameraController *camera;
            TextureRect *bg;
            EncounterScript *encounter_script;
        
        private:
            AudioStreamPlayer *hurt_sound;
            DustTransition *dust;
            GPUParticles2D *spare;
            Node *non_mask, *sprites;
            Array expression_sprites;

            // 적 속성
            bool kr;
            int id;
            bool solo;
            String enemy_name;
            bool is_first_turn;
            bool dodging;
            Dictionary stats;
            Ref<DialogueAsset> dialogues;
            Dictionary rewards;
            bool one_by_one_override;
            Array enemy_acts;
            int current_act;

            NodePath hurt_sound_path;
            NodePath sprites_path;
            NodePath dust_path;
            NodePath spare_path;
            NodePath dialogue_path;

            Ref<ActInfo> _info;
            bool isReady;
            void set_hurt_sound_path(const NodePath& p_path);
            NodePath get_hurt_sound_path() const;
            
            void set_dust_path(const NodePath& p_path);
            NodePath get_dust_path() const;

            void set_spare_path(const NodePath& p_path);
            NodePath get_spare_path() const;

            void set_expression_sprites(const Array& p_animated_sprites);
            Array get_expression_sprites() const;

            void set_sprites_path(const NodePath& p_path);
            NodePath get_sprites_path() const;
            
            void set_dialogue_path(const NodePath& p_path);
            NodePath get_dialogue_path() const;

            void _handle_typing(int text_index, Ref<Dialogues> dialogue_ref, float duration);
            void _on_finished_all_texts_dialogue(PackedInt32Array arr, bool keep_expression);
            void _hurt(int amount);
            void _dodge(int dodge_sign);

            void set_property(Object *value);
            BattleMain *get_main();
            AttackManager *get_attacks();
            BattleBox *get_box();
            SoulBattle *get_soul();
            DialogueControl *get_dialogue();
            CameraController *get_camera();
            TextureRect *get_bg();
            EncounterScript *get_encounter_script();
        
        public:
            Enemy();
            ~Enemy();

            void _ready() override;

            virtual void ready();
            virtual void _on_get_turn();
            virtual void _on_end_turn();
            virtual void on_fight(bool miss);
            virtual void on_act(int option);
            virtual void on_item(int option);
            virtual void on_mercy();
            virtual void on_defeat(bool death);
            virtual String damage_info(int amount);
            virtual PackedStringArray handle_victory();
            GDVIRTUAL0(ready);
            GDVIRTUAL0(_on_get_turn);
            GDVIRTUAL0(_on_end_turn);
            GDVIRTUAL1(on_fight, bool);
            GDVIRTUAL1(on_act, int);
            GDVIRTUAL1(on_item, int);
            GDVIRTUAL0(on_mercy);
            GDVIRTUAL1(on_defeat, bool);
            GDVIRTUAL1R(String, damage_info, int);
            GDVIRTUAL0R(PackedStringArray, handle_victory);

            // 사용 함수
            Node *get_sprites() const;
            void modify_stats(Dictionary value);
            void change_act(int act_index);
            void play_dialogue(int index, float duration=0, bool skip=true, bool keep_expression=false);
            void play_set_dialogue(Ref<Dialogues> dialogue_ref, float duration=0, bool skip=true, bool keep_expression=false);

            Ref<ActInfo> _get_act_info(int act_choice);
            void _get_act(int state, int option);

            void set_kr(bool p_kr);
            bool get_kr() const;
            void set_id(int p_id);
            int get_id() const;
            void set_solo(bool p_solo);
            bool get_solo() const;
            void set_enemy_name(const String& p_enemy_name);
            String get_enemy_name() const;
            void set_is_first_turn(bool p_is_first_turn);
            bool get_is_first_turn() const;
            void set_dodging(bool p_dodging);
            bool get_dodging() const;
            void set_stats(const Dictionary& p_stats);
            Dictionary get_stats() const;
            void set_dialogues(const Ref<DialogueAsset>& p_dialogues);
            Ref<DialogueAsset> get_dialogues() const;
            void set_rewards(const Dictionary& p_rewards);
            Dictionary get_rewards() const;
            void set_one_by_one_override(bool p_one_by_one_override);
            bool get_one_by_one_override() const;
            void set_enemy_acts(const Array& p_enemy_states);
            Array get_enemy_acts() const;
            void set_current_act(int p_current_state);
            int get_current_act() const;
            GPUParticles2D *get_spare() const;
            DustTransition *get_dust() const;
    };
}

#endif