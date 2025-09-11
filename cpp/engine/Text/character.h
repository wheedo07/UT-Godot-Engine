#ifndef CHARACTER_H
#define CHARACTER_H
#include<godot_cpp/core/class_db.hpp>
enum Character {
    DEFAULT,
    SANS,
    PAPYRUS,
    UNDYNE,
    UNDYNE_THE_UNDYING,
    ALPHYS,
    ASGORE,
    FLOWEY,
    FLOWEY_EVIL,
    GASTER,
    GASTER_TEXT,
    METTATON,
    TEMMIE,
    TORIEL
};
VARIANT_ENUM_CAST(Character);

namespace godot {
    void bind_enum(StringName name, String set_fun="", String get_fun="");
    Dictionary get_character_name();
}

#endif