#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H
#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/templates/hash_map.hpp>
namespace godot {
    class AudioPlayer : public Node {
        GDCLASS(AudioPlayer, Node)

        protected:
            static void _bind_methods();
        
        private:
            HashMap<String, AudioStreamPlayer*> audio_stream_players;

            void loop_audio(AudioStreamPlayer* audio);
        
        public:
            AudioPlayer();
            ~AudioPlayer();
        
            void _ready() override;
            void play(const String& audio_name);
            void loop_play(const String& audio_name);
            void stop_audio(const String& audio_name);
            AudioStreamPlayer* get_audio_stream_player(const String& audio_name);
    };
}

#endif