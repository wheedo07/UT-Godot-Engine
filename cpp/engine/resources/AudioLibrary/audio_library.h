#ifndef __AUDIO_LIBRARY_H__
#define __AUDIO_LIBRARY_H__

#include<godot_cpp/classes/resource.hpp>
#include<godot_cpp/classes/audio_stream.hpp>
#include<godot_cpp/classes/audio_server.hpp>
#include<godot_cpp/templates/list.hpp>
namespace godot {
    class AudioLibrary : public Resource {
        GDCLASS(AudioLibrary, Resource)

        protected:
            static void _bind_methods();

        private:
            String name;
            StringName bus;
            float volume_db;
            Ref<AudioStream> audio_stream;
        
        public:
            AudioLibrary();
            ~AudioLibrary();

            void _get_property_list(List<PropertyInfo> *p_list) const;
            bool _set(const StringName& p_name, const Variant& p_value);
            bool _get(const StringName& p_name, Variant& r_ret);

            void set_audio_name(String value);
            String get_audio_name();

            void set_audio_bus(StringName value);
            StringName get_audio_bus();

            void set_volume_db(float value);
            float get_volume_db();

            void set_audio_stream(Ref<AudioStream> value);
            Ref<AudioStream> get_audio_stream();

            static Ref<AudioLibrary> create(String name, Ref<AudioStream> stream, StringName bus = "Master", float volume_db = 0);
            static Ref<AudioLibrary> load(String path);
    };
}

#endif