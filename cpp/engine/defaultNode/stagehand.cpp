#include "stagehand.h"
using namespace godot;

Stagehand::Stagehand() {
    audio_player = nullptr;
}

Stagehand::~Stagehand() {}

void Stagehand::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_character", "name"), &Stagehand::get_character);
    ClassDB::bind_method(D_METHOD("set_property", "value"), &Stagehand::set_property);
    ClassDB::bind_method(D_METHOD("get_audio_player"), &Stagehand::get_audio_player);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "audio_player", PROPERTY_HINT_NONE, "AudioPlayer", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_property", "get_audio_player");
}

void Stagehand::_ready() {
    audio_player = Object::cast_to<AudioPlayer>(get_node_internal("AudioPlayer"));

    if(!has_node("Character")) {
        ERR_PRINT("Character 노드가 없습니다");
        return;
    }
    TypedArray<Node> character_nodes = get_node_internal("Character")->get_children();
    for (int i = 0; i < character_nodes.size(); i++) {
        Node* child = Object::cast_to<Node>(character_nodes[i]);
        if (!child || !child->is_class("CharacterSetting")) continue;
        characters[child->get_name()] = Object::cast_to<CharacterSetting>(child);
    }
}

void Stagehand::set_property(Variant value) {
    ERR_PRINT("이 속성은 초기화 할수 없습니다");
}

AudioPlayer* Stagehand::get_audio_player() const {
    return audio_player;
}

CharacterSetting* Stagehand::get_character(String name) {
    CharacterSetting** character = characters.getptr(name);
    if (!character) {
        ERR_PRINT(vformat("%s 라고 하는 이름의 캐릭터가 없습니다", name));
        return nullptr;
    }
    return *character;
}