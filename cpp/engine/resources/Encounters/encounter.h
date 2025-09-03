#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include<godot_cpp/classes/resource.hpp>
#include<godot_cpp/classes/texture2d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/audio_stream.hpp>
namespace godot {
    class Encounter : public Resource {
        GDCLASS(Encounter, Resource)

        protected:
            static void _bind_methods();
        
        private:
            Vector2 offset;
            StringName encounter_name;
            Ref<Texture2D> background;
            TypedArray<PackedScene> enemies;
            Ref<AudioStream> music;
            PackedStringArray mercy_options;
            float flee_chance;
            static const int enemysizelimit = 3;
        
        public:
            Encounter();
            ~Encounter();

            void set_encounter_name(const StringName& p_name);
            StringName get_encounter_name() const;
            
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
    };
}

#endif