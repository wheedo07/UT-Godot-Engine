#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include<godot_cpp/classes/resource.hpp>
#include<godot_cpp/classes/texture2d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/audio_stream.hpp>
#include<godot_cpp/classes/script.hpp>
#include "button_set.h"
#include "box_set.h"
namespace godot {
    class Encounter : public Resource {
        GDCLASS(Encounter, Resource)

        protected:
            static void _bind_methods();
        
        private:
            StringName encounter_name;
            PackedStringArray flavour_text;
            TypedArray<PackedScene> enemies;
            Ref<Script> script;

            Ref<Texture2D> background;
            Vector2 offset;

            Ref<AudioStream> music;

            PackedStringArray mercy_options;
            float flee_chance;

            Ref<ButtonSet> button_set;
            Ref<BoxSet> box_set;
            String kr_text;
            Color kr_color;
        
        public:
            Encounter();
            ~Encounter();

            void set_encounter_name(const StringName& p_name);
            StringName get_encounter_name() const;

            void set_flavour_text(const PackedStringArray& p_text);
            PackedStringArray get_flavour_text() const;

            void set_encounter_script(const Ref<Script>& p_script);
            Ref<Script> get_encounter_script() const;
            
            void set_background(const Ref<Texture2D>& p_background);
            Ref<Texture2D> get_background() const;

            void set_offset(const Vector2& p_offset);
            Vector2 get_offset() const;
            
            void set_enemies(const TypedArray<PackedScene>& p_enemies);
            TypedArray<PackedScene> get_enemies() const;
            
            void set_music(const Ref<AudioStream>& p_music);
            Ref<AudioStream> get_music() const;

            void set_mercy_options(const PackedStringArray& p_options);
            PackedStringArray get_mercy_options() const;
            
            void set_flee_chance(float p_chance);
            float get_flee_chance() const;

            void set_button_set(const Ref<ButtonSet>& p_set);
            Ref<ButtonSet> get_button_set() const;

            void set_box_set(const Ref<BoxSet>& p_set);
            Ref<BoxSet> get_box_set() const;

            void set_kr_text(const String& p_text);
            String get_kr_text() const;

            void set_kr_color(const Color& p_color);
            Color get_kr_color() const;
    };
}

#endif