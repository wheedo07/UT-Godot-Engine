#include "register_types.h"
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

void register_init() {
    GDREGISTER_RUNTIME_CLASS(MainNode);

    // overworld Node
    GDREGISTER_RUNTIME_CLASS(TEST_WORLD);

    // enemy Node
    GDREGISTER_RUNTIME_CLASS(Enemy_SANS);

    // attackNode
    GDREGISTER_RUNTIME_CLASS(AttackSans);

    // Bullets Node
}
