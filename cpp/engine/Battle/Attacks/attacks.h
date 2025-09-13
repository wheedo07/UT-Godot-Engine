#ifndef ATTACK_BASE_H
#define ATTACK_BASE_H

#include "attacks_manager.h"
#include "engine/Battle/battle_system.h"
#include "engine/Battle/Soul/soul.h"
#include "engine/Battle/Box/battle_box.h"
#include "engine/Camera/camera_remote_controller.h"
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/core/binder_common.hpp>
#include<godot_cpp/core/gdvirtual.gen.inc>
namespace godot {
    class AttackBase : public Node {
        GDCLASS(AttackBase, Node);
        
        protected:
            static void _bind_methods();
        
        public:
            enum Masking {
                ABSOLUTE_CLIP,
                ABSOLUTE,
                RELATIVE_BOX_CLIP,
                RELATIVE_BOX
            };
            BattleMain* main;
            BattleBox* box;
            SoulBattle* soul;
            AttackManager* mask;
            CameraRemoteController* camera;
        
        private:
            Node* non_mask;
            Control* box_rect_clip;
            Control* box_rect_no_clip;
            int attack_id;

            void set_property(Object* value);
            BattleMain* get_main();
            BattleBox* get_box();
            SoulBattle* get_soul();
            AttackManager* get_mask();
            CameraRemoteController* get_camera();
        
        public:
            AttackBase();
            ~AttackBase();
            
            void _ready() override;

            virtual void start_attack();
            virtual void ready();
            GDVIRTUAL0(ready);
            GDVIRTUAL0(start_attack);
            
            // 사용 함수
            void add_bullet(Node* bullet, Masking mask = ABSOLUTE_CLIP);
            Node* quick_bullet(const Ref<PackedScene>& bullet_scene, const Vector2& pos, float rot_deg = 0, Masking mask = ABSOLUTE_CLIP);
            void end_attack();
            
            int get_attack_id() const;
            void set_attack_id(int p_attack_id);
    };
}
VARIANT_ENUM_CAST(godot::AttackBase::Masking);

#endif