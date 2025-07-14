#include "character.h"

namespace godot {
    Dictionary get_character_name() {
        Dictionary character_name;
        character_name[Variant(SANS)] = "Sans";
        character_name[Variant(PAPYRUS)] = "Papyrus";
        character_name[Variant(UNDYNE)] = "Undyne";
        character_name[Variant(UNDYNE_THE_UNDYING)] = "UndyneTheUndying";
        character_name[Variant(ALPHYS)] = "Alphys";
        character_name[Variant(ASGORE)] = "Asgore";
        character_name[Variant(FLOWEY)] = "Flowey";
        character_name[Variant(FLOWEY_EVIL)] = "FloweyEvil";
        character_name[Variant(GASTER)] = "Gaster";
        character_name[Variant(GASTER_TEXT)] = "GasterText";
        character_name[Variant(METTATON)] = "Mettaton";
        character_name[Variant(TEMMIE)] = "Temmie";
        character_name[Variant(TORIEL)] = "Toriel";
        return character_name;
    }

    void bind_enum(StringName name, String set_fun, String get_fun) {
        auto get_class_static = [name]() {
            return name;
        };
        BIND_ENUM_CONSTANT(DEFAULT);
        BIND_ENUM_CONSTANT(SANS);
        BIND_ENUM_CONSTANT(PAPYRUS);
        BIND_ENUM_CONSTANT(UNDYNE);
        BIND_ENUM_CONSTANT(UNDYNE_THE_UNDYING);
        BIND_ENUM_CONSTANT(ALPHYS);
        BIND_ENUM_CONSTANT(ASGORE);
        BIND_ENUM_CONSTANT(FLOWEY);
        BIND_ENUM_CONSTANT(FLOWEY_EVIL);
        BIND_ENUM_CONSTANT(GASTER);
        BIND_ENUM_CONSTANT(GASTER_TEXT);
        BIND_ENUM_CONSTANT(METTATON);
        BIND_ENUM_CONSTANT(TEMMIE);
        BIND_ENUM_CONSTANT(TORIEL);

        Array arr = get_character_name().values();
        String enum_string = "";
        for(int i=0; i < arr.size(); i++) {
            enum_string += String(arr[i]) + ",";
        }
        enum_string = enum_string.substr(0, enum_string.length() - 1);

        ADD_PROPERTY(PropertyInfo(Variant::INT, "character", PROPERTY_HINT_ENUM, 
        "DEFAULT,"+enum_string)
        , set_fun, get_fun);
    }
}