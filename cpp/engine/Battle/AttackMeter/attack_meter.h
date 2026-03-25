#ifndef __ATTACK_METER_H__
#define __ATTACK_METER_H__

#include<godot_cpp/classes/node.hpp>
namespace godot {
    class EncounterScript;
    class AttackMeter : public Node {
        GDCLASS(AttackMeter, Node);
        
        protected:
            static void _bind_methods();

        public:
            EncounterScript *encounter_script;
            int enemy_id, enemy_def;

        private:
            void set_property(Variant value);
            int get_enemy_id();
            int get_enemy_def();
            EncounterScript *get_encounter_script();
        
        public:
            AttackMeter();
            ~AttackMeter();

            /* 사용 함수 */
            int get_base_damage();
            
            void set_encounter_script(EncounterScript *value);
            void set_enemy_id(int value);
            void set_enemy_def(int value);
    };
}

#endif