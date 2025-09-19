#ifndef ENEMY_H
#define ENEMY_H

#include "Soul/soul.h"
#include "Box/battle_box.h"
#include "Attacks/attacks_manager.h"
#include "Misc/dialogue_control.h"
#include "engine/Camera/camera_remote_controller.h"
#include "engine/resources/Enemies/enemy_state.h"
#include "engine/resources/Enemies/Act.h"
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

    class Enemy : public CharacterBody2D {
        GDCLASS(Enemy, CharacterBody2D);
        
        protected:
            static void _bind_methods();

        public:
            BattleMain* main;
            AttackManager* attacks;
            BattleBox* box;
            SoulBattle* soul;
            DialogueControl* dialogue;
            CameraRemoteController* camera;
            TextureRect* bg;
        
        private:
            AudioStreamPlayer* hurt_sound;
            AudioStreamPlayer* dust_sound;
            GPUParticles2D* dust;
            Node* non_mask;
            Node* sprites;
            AnimatedSprite2D* e_head;
            AnimatedSprite2D* e_body;

            // 적 속성
            bool kr;
            int id;
            bool solo;
            String enemy_name;
            bool is_first_turn;
            bool dodging;
            Dictionary stats;
            Array dialogues;
            PackedStringArray flavour_text;
            Dictionary rewards;
            bool new_states_override;
            bool one_by_one_overrdie;
            Array enemy_states;
            int current_state;

            NodePath hurt_sound_path;
            NodePath dust_sound_path;
            NodePath e_body_path;
            NodePath e_head_path;
            NodePath sprites_path;
            NodePath dust_path;
            NodePath dialogue_path;

            Ref<ActInfo> _info;
            void set_hurt_sound_path(const NodePath& p_path);
            NodePath get_hurt_sound_path() const;
            
            void set_dust_sound_path(const NodePath& p_path);
            NodePath get_dust_sound_path() const;
            
            void set_dust_path(const NodePath& p_path);
            NodePath get_dust_path() const;
            
            void set_e_head_path(const NodePath& p_path);
            NodePath get_e_head_path() const;
            
            void set_e_body_path(const NodePath& p_path);
            NodePath get_e_body_path() const;
            
            void set_sprites_path(const NodePath& p_path);
            NodePath get_sprites_path() const;
            
            void set_dialogue_path(const NodePath& p_path);
            NodePath get_dialogue_path() const;

            void _handle_typing(int text_index, Ref<Dialogues> dialogue_ref, float duration, bool skip);
            void _on_finished_all_texts_dialogue(int head, int body);
            void _hurt(int amount);
            void _dodge();

            void set_property(Object* value);
            BattleMain* get_main();
            AttackManager* get_attacks();
            BattleBox* get_box();
            SoulBattle* get_soul();
            DialogueControl* get_dialogue();
            CameraRemoteController* get_camera();
            TextureRect* get_bg();
        
        public:
            Enemy();
            ~Enemy();

            void _ready() override;

            virtual void ready();
            virtual void on_fight_used();
            virtual void on_act_used(int option);
            virtual void on_item_used(int option);
            virtual void on_mercy_used();
            virtual void _on_get_turn();
            virtual void _on_end_turn();
            virtual PackedStringArray on_win();
            virtual void on_defeat(bool death);
            GDVIRTUAL0(ready);
            GDVIRTUAL0(on_fight_used);
            GDVIRTUAL1(on_act_used, int);
            GDVIRTUAL1(on_item_used, int);
            GDVIRTUAL0(on_mercy_used);
            GDVIRTUAL0(_on_get_turn);
            GDVIRTUAL0(_on_end_turn);
            GDVIRTUAL0R(PackedStringArray, on_win);
            GDVIRTUAL1(on_defeat, bool);

            // 사용 함수
            void modify_stats(Dictionary value);
            void change_state(int new_state);
            void play_dialogue(int index, float duration=0, bool skip=true);
            void play_set_dialogue(Ref<Dialogues> dialogue_ref, float duration=0, bool skip=true);

            Ref<ActInfo> _get_act_info(int act_choice);
            void on_death();
            void _on_spared(int id_number);
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
            void set_dialogues(const Array& p_dialogues);
            Array get_dialogues() const;
            void set_flavour_text(const PackedStringArray& p_flavour_text);
            PackedStringArray get_flavour_text() const;
            void set_rewards(const Dictionary& p_rewards);
            Dictionary get_rewards() const;
            void set_new_states_override(bool p_new_states_override);
            bool get_new_states_override() const;
            void set_one_by_one_overrdie(bool p_one_by_one_overrdie);
            bool get_one_by_one_overrdie() const;
            void set_enemy_states(const Array& p_enemy_states);
            Array get_enemy_states() const;
            void set_current_state(int p_current_state);
            int get_current_state() const;
            Node* get_sprites() const;
    };
}

#endif