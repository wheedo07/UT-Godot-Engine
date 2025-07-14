#ifndef TEST_H
#define TEST_H
#include "engine/Overworld/overworld.h"
namespace godot {
    class TEST_WORLD : public Overworld {
        GDCLASS(TEST_WORLD, Overworld);

        protected:
            static void _bind_methods();

        public:
            TEST_WORLD();
            ~TEST_WORLD();

            void ready() override;
    };
}

#endif