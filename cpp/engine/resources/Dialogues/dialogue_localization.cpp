#include "dialogue_localization.h"
#include "dialogues.h"
#include<godot_cpp/classes/json.hpp>
#include<godot_cpp/classes/file_access.hpp>
#include<godot_cpp/classes/translation_server.hpp>
using namespace godot;

DialogueLocalization::DialogueLocalization() {}

DialogueLocalization::~DialogueLocalization() {}

void DialogueLocalization::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load", "path"), &DialogueLocalization::load);
    ClassDB::bind_method(D_METHOD("get_dialogues", "locale", "id"), &DialogueLocalization::get_dialogues);
    ClassDB::bind_method(D_METHOD("get_dialogues_default", "id"), &DialogueLocalization::get_dialogues_default);
}

Ref<DialogueLocalization> DialogueLocalization::load(String path) {
    Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);
    if(file.is_null() || !file->is_open()) {
        ERR_PRINT(vformat(String::utf8("파일을 열 수 없습니다: %s"), path));
        return nullptr;
    }
    String content = file->get_as_text();
    file->close();
    data = JSON::parse_string(content);
    return this;
}

Ref<Dialogues> DialogueLocalization::get_dialogues(String locale, String id) {
    Ref<Dialogues> dialogues = memnew(Dialogues);
    if(data.is_empty()) {
        ERR_PRINT(String::utf8("로케일 데이터가 로드되지 않았습니다."));
        return dialogues;
    }
    if(!data.has(id)) {
        ERR_PRINT(vformat(String::utf8("대화 ID를 찾을 수 없습니다: %s"), id));
        return dialogues;
    }
    if(locale.find("_") != -1) locale = locale.get_slice("_", 0);

    Dictionary id_data = data[id];
    if(!id_data.has(locale)) {
        WARN_PRINT(vformat(String::utf8("로케일 데이터를 찾을 수 없습니다, 기본 로케일로 대체합니다: %s, %s"), locale, id));
        if(!id_data.has("en")) {
            ERR_PRINT(vformat(String::utf8("기본 로케일 데이터(en)를 찾을 수 없습니다: %s"), id));
            return dialogues;
        }
        locale = "en";
    }
    Dictionary locale_data = id_data[locale];
    if(locale_data.is_empty()) {
        ERR_PRINT(vformat(String::utf8("로케일 데이터가 비어있습니다: %s, %s"), locale, id));
        return dialogues;
    }

    if(!locale_data.has("text")) {
        ERR_PRINT(vformat(String::utf8("텍스트 데이터를 찾을 수 없습니다: %s, %s"), locale, id));
        return dialogues;
    }else dialogues->from(locale_data["text"]);

    if(locale_data.has("speed")) {
        if(locale_data["speed"].get_type() != Variant::ARRAY) {
            ERR_PRINT(vformat(String::utf8("속도 데이터 형식이 올바르지 않습니다(Array여야 합니다): %s, %s"), locale, id));
        }else dialogues->set_speed(locale_data["speed"]);
    }
    if(locale_data.has("pauses")) {
        if(locale_data["pauses"].get_type() != Variant::ARRAY) {
            ERR_PRINT(vformat(String::utf8("일시정지 데이터 형식이 올바르지 않습니다(Array여야 합니다): %s, %s"), locale, id));
        }else dialogues->set_pauses(locale_data["pauses"]);
    }
    if(id_data.has("expressions")) {
        if(id_data["expressions"].get_type() != Variant::ARRAY) {
            ERR_PRINT(vformat(String::utf8("표정 데이터 형식이 올바르지 않습니다(Array여야 합니다): %s, %s"), locale, id));
        }else dialogues->set_expressions(id_data["expressions"]);
    }
    return dialogues;
}

Ref<Dialogues> DialogueLocalization::get_dialogues_default(String id) {
    String default_locale = TranslationServer::get_singleton()->get_locale();
    return get_dialogues(default_locale, id);
}