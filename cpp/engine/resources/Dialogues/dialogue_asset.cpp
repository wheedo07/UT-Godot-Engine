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
}

void DialogueAsset::_get_property_list(List<PropertyInfo> *p_list) const {
    if(isLocale) {
        p_list->push_back(PropertyInfo(Variant::STRING, "locale_path", PROPERTY_HINT_FILE, "*.json"));
        p_list->push_back(PropertyInfo(Variant::PACKED_STRING_ARRAY, "locale_keys"));
    }else {
        p_list->push_back(PropertyInfo(Variant::ARRAY, "dialogues", PROPERTY_HINT_TYPE_STRING, 
            String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":Dialogues"
        ));
    }
}

bool DialogueAsset::_set(const StringName& p_name, const Variant& p_value) {
    String name = p_name;
    if(name == "locale_path") {
        locale_path = String(p_value);
        return true;
    }else if(name == "locale_keys") {
        locale_keys = PackedStringArray(p_value);
        return true;
    }else if(name == "dialogues") {
        dialogues = TypedArray<Dialogues>(p_value);
        return true;
    }
    return false;
}

bool DialogueAsset::_get(const StringName& p_name, Variant& r_ret) {
    String name = p_name;
    if(name == "locale_path") {
        r_ret = locale_path;
        return true;
    }else if(name == "locale_keys") {
        r_ret = locale_keys;
        return true;
    }else if(name == "dialogues") {
        r_ret = dialogues;
        return true;
    }
    return false;
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
    notify_property_list_changed();
}

bool DialogueAsset::get_is_locale() const {
    return isLocale;
}