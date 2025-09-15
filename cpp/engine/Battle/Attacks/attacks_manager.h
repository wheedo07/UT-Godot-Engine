#ifndef ATTACK_MANAGER_H
#define ATTACK_MANAGER_H

#include<godot_cpp/classes/back_buffer_copy.hpp>
#include<godot_cpp/classes/sprite2d.hpp>
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/variant/typed_array.hpp>
namespace godot {
    class AttackManager : public BackBufferCopy {
        GDCLASS(AttackManager, BackBufferCopy);
        
        protected:
            static void _bind_methods();
        
        public:
            Sprite2D* top_left;
            Sprite2D* bottom_right;
        
        private:
            PackedInt32Array run_attack_id;
            Node2D* mask;
            Array current_attacks;

            bool check_all_attacks_finished();
        
        public:
            AttackManager();
            ~AttackManager();

            void _ready() override;

            Node* add_attack(const Ref<PackedScene>& attack);
            void start_attacks();
            void start_attack(int id);
            void force_end_attacks();
            void end_attack(int id);
    };
}

#endif