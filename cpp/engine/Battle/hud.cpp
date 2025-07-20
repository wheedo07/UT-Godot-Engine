#include "hud.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>

BattleHUD::BattleHUD() {}

BattleHUD::~BattleHUD() {}

void BattleHUD::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_kr", "to"), &BattleHUD::set_kr, DEFVAL(true));
}

void BattleHUD::_ready() {
    if(isEditor) return;
    name_label = Object::cast_to<RichTextLabel>(get_node_internal("Name"));
    lv_label = Object::cast_to<RichTextLabel>(get_node_internal("Lv"));
    hp_bar = Object::cast_to<ProgressBar>(get_node_internal("MarginContainer/HpBar"));
    kr_bar = Object::cast_to<ProgressBar>(get_node_internal("MarginContainer/KrBar"));
    hp_bar_container = Object::cast_to<MarginContainer>(get_node_internal("MarginContainer"));
    kr_text = Object::cast_to<RichTextLabel>(get_node_internal("KrText/KR"));
    hp_label = Object::cast_to<RichTextLabel>(get_node_internal("Hp"));
    _process(0);
}

void BattleHUD::_process(double delta) {
    if (!global || isEditor) return;
    
    name_label->set_text(global->get_player_name());
    lv_label->set_text("Lv " + String::num_int64(global->get_player_lv()));
    
    Vector2 min_size = hp_bar_container->get_custom_minimum_size();
    min_size.x = Math::min(Math::max(global->get_player_max_hp() * 1.2f + 1.0f, 4.0f), 160.0f);
    hp_bar_container->set_custom_minimum_size(min_size);

    hp_bar->set_max(global->get_player_max_hp());
    kr_bar->set_max(global->get_player_max_hp());
    
    hp_bar->set_value(global->get_player_hp() - global->get_player_kr());
    kr_bar->set_value(global->get_player_hp());
    
    String hp_text = "";
    if(global->get_player_kr() > 0) {
        Color magenta = Color(1, 0, 1);
        hp_text = "[color=" + magenta.to_html() + "]";
    }
    
    hp_text += String::num_int64(global->get_player_hp()) + " / " + String::num_int64(global->get_player_max_hp());
    hp_label->set_text(hp_text);
}

void BattleHUD::set_kr(bool to) {
    kr_text->set_visible(to);
}