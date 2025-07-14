#include "audio_player.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

AudioPlayer::AudioPlayer() {}

AudioPlayer::~AudioPlayer() {}

void AudioPlayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("play", "audio_name"), &AudioPlayer::play);
    ClassDB::bind_method(D_METHOD("stop_audio", "audio_name"), &AudioPlayer::stop_audio);
    ClassDB::bind_method(D_METHOD("loop_audio", "audio"), &AudioPlayer::loop_audio);
    ClassDB::bind_method(D_METHOD("get_audio_stream_player", "audio_name"), &AudioPlayer::get_audio_stream_player);
}

void AudioPlayer::_ready() {
    TypedArray<Node> children = get_children();
    for (int i = 0; i < children.size(); i++) {
        Node* child = Object::cast_to<Node>(children[i]);
        if (!child || !Object::cast_to<AudioStreamPlayer>(child)) continue;
        audio_stream_players[child->get_name()] = Object::cast_to<AudioStreamPlayer>(child);
    }
}

void AudioPlayer::play(const String& audio_name) {
    AudioStreamPlayer** player = audio_stream_players.getptr(audio_name);
    if (!player) {
        ERR_PRINT(vformat("%s 라고 하는 이름의 오디오 플레이어가 없습니다", audio_name));
        return;
    }
    (*player)->play();
}


void AudioPlayer::loop_play(const String& audio_name) {
    AudioStreamPlayer** player = audio_stream_players.getptr(audio_name);
    if (!player) {
        ERR_PRINT(vformat("%s 라고 하는 이름의 오디오 플레이어가 없습니다", audio_name));
        return;
    }
    (*player)->play();
    (*player)->connect("finished", Callable(this, "loop_audio").bind(*player));
}

void AudioPlayer::stop_audio(const String& audio_name) {
    AudioStreamPlayer** player = audio_stream_players.getptr(audio_name);
    if (!player) {
        return;
    }
    if((*player)->is_connected("finished", Callable(this, "loop_audio").bind(*player))) (*player)->disconnect("finished", Callable(this, "loop_audio").bind(*player));
    (*player)->stop();
}

void AudioPlayer::loop_audio(AudioStreamPlayer* audio) {
    audio->play();
}

AudioStreamPlayer* AudioPlayer::get_audio_stream_player(const String& audio_name) {
    AudioStreamPlayer** player = audio_stream_players.getptr(audio_name);
    if (!player) {
        for (auto it = audio_stream_players.begin(); it != audio_stream_players.end(); ++it) {
            return it->value;
        }
        return nullptr;
    }
    return *player;
}