#include "dialogue_asset.h"
#include "dialogues.h"
#include "dialogue_localization.h"
using namespace godot;

DialogueAsset::DialogueAsset() {
    isLocale = false;
    locale_path = "";
}

DialogueAsset::~DialogueAsset() {}

void DialogueAsset::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_is_locale", "isLocale"), &DialogueAsset::set_is_locale);
    ClassDB::bind_method(D_METHOD("get_is_locale"), &DialogueAsset::get_is_locale);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_locale"), "set_is_locale", "get_is_locale");

    ClassDB::bind_method(D_METHOD("set_locale_path", "path"), &DialogueAsset::set_locale_path);
    ClassDB::bind_method(D_METHOD("get_locale_path"), &DialogueAsset::get_locale_path);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "locale_path", PROPERTY_HINT_FILE, "*.json"), "set_locale_path", "get_locale_path");

    ADD_GROUP("datalogue_data", "");
    ClassDB::bind_method(D_METHOD("set_locale_keys", "keys"), &DialogueAsset::set_locale_keys);
    ClassDB::bind_method(D_METHOD("get_locale_keys"), &DialogueAsset::get_locale_keys);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "locale_keys"), "set_locale_keys", "get_locale_keys");

    ClassDB::bind_method(D_METHOD("set_dialogues", "dialogues"), &DialogueAsset::set_dialogues);
    ClassDB::bind_method(D_METHOD("get_dialogues"), &DialogueAsset::get_dialogues);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "dialogues", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":Dialogues"
    ), "set_dialogues", "get_dialogues");
}

Ref<DialogueAsset> DialogueAsset::load_locale_data() {
    if(locale_path.is_empty()) return this;
    data.instantiate();
    data->load(locale_path);
    return this;
}

Ref<Dialogues> DialogueAsset::get_data(int index) {
    if(isLocale) {
        if(index < 0 || index >= locale_keys.size()) {
            ERR_PRINT(String::utf8("로케일 키 인덱스가 범위를 벗어났습니다."));
            return nullptr;
        }
        String key = locale_keys[index];
        return data->get_dialogues_default(key);
    }else {
        if(index < 0 || index >= dialogues.size()) {
            ERR_PRINT(String::utf8("대화 데이터 인덱스가 범위를 벗어났습니다."));
            return nullptr;
        }
        return dialogues[index];
    }
}

bool DialogueAsset::has_data() {
    if(isLocale) {
        return !locale_keys.is_empty();
    }else {
        return !dialogues.is_empty();
    }
}

void DialogueAsset::set_is_locale(bool p_isLocale) {
    isLocale = p_isLocale;
}

bool DialogueAsset::get_is_locale() const {
    return isLocale;
}

void DialogueAsset::set_locale_path(String path) {
    locale_path = path;
}

String DialogueAsset::get_locale_path() const {
    return locale_path;
}

void DialogueAsset::set_locale_keys(PackedStringArray keys) {
    locale_keys = keys;
}

PackedStringArray DialogueAsset::get_locale_keys() const {
    return locale_keys;
}

void DialogueAsset::set_dialogues(TypedArray<Dialogues> p_dialogues) {
    dialogues = p_dialogues;
}

TypedArray<Dialogues> DialogueAsset::get_dialogues() const {
    return dialogues;
}