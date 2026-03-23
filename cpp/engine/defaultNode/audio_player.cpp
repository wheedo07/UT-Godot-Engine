#include "audio_player.h"
#include "engine/resources/AudioLibrary/audio_library.h"
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
using namespace godot;

AudioPlayer::AudioPlayer() {}

AudioPlayer::~AudioPlayer() {}

void AudioPlayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_loop_audio", "audio"), &AudioPlayer::_loop_audio);
    ClassDB::bind_method(D_METHOD("_tree_exited", "audio"), &AudioPlayer::_tree_exited);
     
    ClassDB::bind_method(D_METHOD("create", "library"), &AudioPlayer::create);
    ClassDB::bind_method(D_METHOD("dynamic", "library"), &AudioPlayer::dynamic);
    ClassDB::bind_method(D_METHOD("play", "audio_name"), &AudioPlayer::play);
    ClassDB::bind_method(D_METHOD("loop_play", "audio_name"), &AudioPlayer::loop_play);
    ClassDB::bind_method(D_METHOD("stop", "audio_name"), &AudioPlayer::stop);
    ClassDB::bind_method(D_METHOD("get_audio_stream_player", "audio_name"), &AudioPlayer::get_audio_stream_player);

    ClassDB::bind_method(D_METHOD("set_audio_libraries", "value"), &AudioPlayer::set_audio_libraries);
    ClassDB::bind_method(D_METHOD("get_audio_libraries"), &AudioPlayer::get_audio_libraries);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "audio_libraries", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":AudioLibrary")
    , "set_audio_libraries", "get_audio_libraries");
}

void AudioPlayer::_ready() {
    for(int i=0; i < audio_libraries.size(); i++) {
        Ref<AudioLibrary> library = audio_libraries[i];
        AudioStreamPlayer *child = create(library);
        if(!child) continue;
    }
}

AudioStreamPlayer *AudioPlayer::create(Ref<AudioLibrary> library) {
    if(audio_stream_players.has(library->get_audio_name())) {
        ERR_PRINT(vformat(String::utf8("AudioStreamPlayer 이름이 %s 인 라이브러리가 중복됩니다"), library->get_audio_name()));
        return nullptr;
    }
    AudioStreamPlayer *child = _create(library);
    audio_stream_players[library->get_audio_name()] = child;
    return child;
}

AudioStreamPlayer *AudioPlayer::dynamic(Ref<AudioLibrary> library) {
    AudioStreamPlayer *child = _create(library);
    child->connect("finished", Callable(child, "queue_free"));
    return child;
}

void AudioPlayer::play(String audio_name) {
    AudioStreamPlayer **player = audio_stream_players.getptr(audio_name);
    if(!player) {
        ERR_PRINT(vformat(String::utf8("%s 라고 하는 이름의 오디오 플레이어가 없습니다"), audio_name));
        return;
    }
    (*player)->play();
}

void AudioPlayer::loop_play(const String& audio_name) {
    AudioStreamPlayer **player = audio_stream_players.getptr(audio_name);
    if (!player) {
        ERR_PRINT(vformat(String::utf8("%s 라고 하는 이름의 오디오 플레이어가 없습니다"), audio_name));
        return;
    }
    (*player)->play();

    Callable call = Callable(this, "_loop_audio").bind(*player);
    if(!(*player)->is_connected("finished", call)) (*player)->connect("finished", call);
}

void AudioPlayer::stop(const String& audio_name) {
    AudioStreamPlayer **player = audio_stream_players.getptr(audio_name);
    if(!player) return;

    Callable call = Callable(this, "_loop_audio").bind(*player);
    if((*player)->is_connected("finished", call)) (*player)->disconnect("finished", call);
    
    (*player)->stop();
}

AudioStreamPlayer *AudioPlayer::get_audio_stream_player(const String& audio_name) {
    AudioStreamPlayer **player = audio_stream_players.getptr(audio_name);
    if(!player) return nullptr;
    return *player;
}

AudioStreamPlayer *AudioPlayer::_create(Ref<AudioLibrary> library) {
    AudioStreamPlayer *child = memnew(AudioStreamPlayer);
    child->set_name(library->get_audio_name());
    add_child(child);
    child->connect("tree_exited", Callable(this, "_tree_exited").bind(child));

    child->set_stream(library->get_audio_stream());
    child->set_bus(library->get_audio_bus());
    child->set_volume_db(library->get_volume_db());
    return child;
}

void AudioPlayer::_tree_exited(AudioStreamPlayer *audio) {
    audio_stream_players.erase(audio->get_name());
}

void AudioPlayer::_loop_audio(AudioStreamPlayer* audio) {
    audio->play();
}

void AudioPlayer::set_audio_libraries(Array value) {
    audio_libraries = value;
}

Array AudioPlayer::get_audio_libraries() {
    return audio_libraries;
}