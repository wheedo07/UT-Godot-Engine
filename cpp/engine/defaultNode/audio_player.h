#ifndef __AUDIO_PLAYER_H__
#define __AUDIO_PLAYER_H__
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/templates/hash_map.hpp>
namespace godot {
    class AudioLibrary;
    class AudioPlayer : public Node {
        GDCLASS(AudioPlayer, Node)

        protected:
            static void _bind_methods();
        
        private:
            Array audio_libraries;
            HashMap<String, AudioStreamPlayer*> audio_stream_players;

            AudioStreamPlayer *_create(Ref<AudioLibrary> library);
            void _loop_audio(AudioStreamPlayer *audio);
            void _tree_exited(AudioStreamPlayer *audio);
        
        public:
            AudioPlayer();
            ~AudioPlayer();
             
            void _ready() override;
            AudioStreamPlayer *create(Ref<AudioLibrary> library);
            AudioStreamPlayer *dynamic(Ref<AudioLibrary> library);
            void play(String audio_name);
            void loop_play(const String& audio_name);
            void stop(const String& audio_name);
            AudioStreamPlayer *get_audio_stream_player(const String& audio_name);

            void set_audio_libraries(Array value);
            Array get_audio_libraries();
    };
}

#endif