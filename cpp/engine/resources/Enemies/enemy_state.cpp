#include "enemy_state.h"
#include<godot_cpp/core/class_db.hpp>
using namespace godot;

EnemyState::EnemyState() {
    sparable = false;

    Ref<ActInfo> check = memnew(ActInfo);
    check->set_act(String::utf8("* 살펴보기"));
    check->set_description(PackedStringArray({String::utf8("* 덤디덤 - ATK 120 DEF 40\n* 지하세계 왕이다")}));
    acts.push_back(check);

    Ref<ActInfo> insult = memnew(ActInfo);
    insult->set_act(String::utf8("* 모욕하기"));
    insult->set_description(PackedStringArray({String::utf8("* 넌.. 정말"), String::utf8("* 나쁜 아이야")}));
    acts.push_back(insult);

    Ref<ActInfo> compliment = memnew(ActInfo);
    compliment->set_act(String::utf8("* 장난치기"));
    compliment->set_description(PackedStringArray({String::utf8("* 사람을 다 일어나게 하는 숫자는?"), String::utf8("* 다섯")}));
    acts.push_back(compliment);

    Ref<ActInfo> inquire = memnew(ActInfo);
    inquire->set_act(String::utf8("* 한숨쉬기"));
    inquire->set_description(PackedStringArray({
        String::utf8("* 나도 몰루"),
        String::utf8("* 미래의 나야 써라")
    }));
    acts.push_back(inquire);
}

EnemyState::~EnemyState() {}

void EnemyState::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_acts", "acts"), &EnemyState::set_acts);
    ClassDB::bind_method(D_METHOD("get_acts"), &EnemyState::get_acts);
    ClassDB::bind_method(D_METHOD("set_sparable", "sparable"), &EnemyState::set_sparable);
    ClassDB::bind_method(D_METHOD("get_sparable"), &EnemyState::get_sparable);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "acts", PROPERTY_HINT_TYPE_STRING, 
        String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":ActInfo")
    ,"set_acts", "get_acts");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sparable"), "set_sparable", "get_sparable");
}

void EnemyState::set_acts(TypedArray<ActInfo> p_acts) {
    acts = p_acts;
}

TypedArray<ActInfo> EnemyState::get_acts() const {
    return acts;
}

void EnemyState::set_sparable(bool p_sparable) {
    sparable = p_sparable;
}

bool EnemyState::get_sparable() const {
    return sparable;
}