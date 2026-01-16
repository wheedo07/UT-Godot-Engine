#ifndef ENEMY_ACT_H
#define ENEMY_ACT_H
#include<godot_cpp/classes/resource.hpp>
#include "Act.h"
namespace godot {
    class EnemyAct : public Resource {
        GDCLASS(EnemyAct, Resource)

        protected:
            static void _bind_methods();
        
        private:
            TypedArray<ActInfo> acts;
            bool sparable;

        public:
            EnemyAct();
            ~EnemyAct();

            void set_acts(TypedArray<ActInfo> p_acts);
            TypedArray<ActInfo> get_acts() const;

            void set_sparable(bool p_sparable);
            bool get_sparable() const;
    };
}

#endif