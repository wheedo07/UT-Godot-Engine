#include "audio_library.h"
#include<godot_cpp/classes/audio_server.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
using namespace godot;

AudioLibrary::AudioLibrary() {
    name = "";
    bus = "Master";
    volume_db = 0;
}

AudioLibrary::~AudioLibrary() {}

void AudioLibrary::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_audio_name", "value"), &AudioLibrary::set_audio_name);
    ClassDB::bind_method(D_METHOD("get_audio_name"), &AudioLibrary::get_audio_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "audio_name"), "set_audio_name", "get_audio_name");

    ClassDB::bind_method(D_METHOD("set_audio_stream", "value"), &AudioLibrary::set_audio_stream);
    ClassDB::bind_method(D_METHOD("get_audio_stream"), &AudioLibrary::get_audio_stream);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "audio_stream", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_audio_stream", "get_audio_stream");

    ClassDB::bind_method(D_METHOD("set_volume_db", "value"), &AudioLibrary::set_volume_db);
    ClassDB::bind_method(D_METHOD("get_volume_db"), &AudioLibrary::get_volume_db);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,0.1"), "set_volume_db", "get_volume_db");

    ClassDB::bind_static_method("AudioLibrary", 
        D_METHOD("create", "name", "stream", "bus", "volume_db"), &AudioLibrary::create, 
        DEFVAL("Master"), DEFVAL(0));
    ClassDB::bind_static_method("AudioLibrary", D_METHOD("load", "path"), &AudioLibrary::load);
}

void AudioLibrary::_get_property_list(List<PropertyInfo> *p_list) const {
    AudioServer *audio_server = AudioServer::get_singleton();
    String buses;
    for(int i=0; i < audio_server->get_bus_count(); i++) {
        if(i != 0) buses += ",";
        buses += audio_server->get_bus_name(i);
    }
    p_list->push_back(PropertyInfo(Variant::STRING_NAME, "audio_bus", PROPERTY_HINT_ENUM, buses));
}

bool AudioLibrary::_set(const StringName &p_name, const Variant &p_value) {
    String name = p_name;
    if(name == "audio_bus") {
        bus = p_value;
        return true;
    }
    return false;
}

bool AudioLibrary::_get(const StringName &p_name, Variant &r_ret) {
    String name = p_name;
    if(name == "audio_bus") {
        r_ret = bus;
        return true;
    }
    return false;
}

void AudioLibrary::set_audio_name(String value) {
    name = value;
}

String AudioLibrary::get_audio_name() {
    return name;
}

void AudioLibrary::set_audio_bus(StringName value) {
    bus = value;
}

StringName AudioLibrary::get_audio_bus() {
    return bus;
}

void AudioLibrary::set_volume_db(float value) {
    volume_db = value;
}

float AudioLibrary::get_volume_db() {
    return volume_db;
}

void AudioLibrary::set_audio_stream(Ref<AudioStream> value) {
    audio_stream = value;
}

Ref<AudioStream> AudioLibrary::get_audio_stream() {
    return audio_stream;
}

Ref<AudioLibrary> AudioLibrary::create(String name, Ref<AudioStream> stream, StringName bus, float volume_db) {
    Ref<AudioLibrary> library = memnew(AudioLibrary);
    library->set_audio_name(name);
    library->set_audio_stream(stream);
    library->set_audio_bus(bus);
    library->set_volume_db(volume_db);
    return library;
}

Ref<AudioLibrary> AudioLibrary::load(String path) {
    Ref<Resource> resource = ResourceLoader::get_singleton()->load(path);
    if(resource.is_null()) {
        ERR_PRINT(vformat(String::utf8("AudioLibrary 리소스 %s 를 불러올 수 없습니다"), path));
        return nullptr;
    }
    Ref<AudioLibrary> library = resource;
    if(library.is_null()) {
        ERR_PRINT(vformat(String::utf8("리소스 %s 는 AudioLibrary 타입이 아닙니다"), path));
        return nullptr;
    }
    return library;
}