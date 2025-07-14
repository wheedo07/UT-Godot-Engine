#ifndef ENEMY_STATE_H
#define ENEMY_STATE_H
#include<godot_cpp/classes/resource.hpp>
#include "Act.h"
namespace godot {
    class EnemyState : public Resource {
        GDCLASS(EnemyState, Resource)

        protected:
            static void _bind_methods();
        
        private:
            TypedArray<ActInfo> acts;
            bool sparable;

        public:
            EnemyState();
            ~EnemyState();

            void set_acts(TypedArray<ActInfo> p_acts);
            TypedArray<ActInfo> get_acts() const;

            void set_sparable(bool p_sparable);
            bool get_sparable() const;
    };
}

#endif