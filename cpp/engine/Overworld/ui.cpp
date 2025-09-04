#include "ui.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/panel.hpp>
#include<godot_cpp/classes/viewport.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/label.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/classes/gpu_particles2d.hpp>

UI::UI() {
    sizethingys = Dictionary();
    sizethingys["PADDING"] = 40;
    sizethingys["SIZE_PER_OPTION"] = 40;
    
    soulposition = Vector2();
    soulposition_item = Vector2();
    
    optionsize = Dictionary();
    optionsize[OPTIONS] = Vector2(1, 3);
    optionsize[STATS] = Vector2();
    optionsize[ITEM] = Vector2(1, 1);
    optionsize[ITEM_ACTION] = Vector2(3, 1);
    optionsize[CELL] = Vector2(1, 1);
    optionsize[ITEM_USE_DISABLE_MOVEMENT] = Vector2();
    
    textbox = nullptr;
    current_state = OPTIONS;
    
    enabled_options.resize(3);
    enabled_options[0] = true;
    enabled_options[1] = true;
    enabled_options[2] = false;
    
    options_dict = Dictionary();
    options_dict[0] = String::utf8("아이템");
    options_dict[1] = String::utf8("스텟");
    options_dict[2] = String::utf8("전화");
    options_dict[3] = String::utf8("BOX");
    
    pos_history = Dictionary();
    pos_history[OPTIONS] = Variant();
    pos_history[STATS] = Variant();
    pos_history[ITEM] = Variant();
    pos_history[ITEM_ACTION] = Variant();
    pos_history[CELL] = Variant();
    pos_history[ITEM_USE_DISABLE_MOVEMENT] = Variant();
}

UI::~UI() {}

void UI::_bind_methods() {
    BIND_ENUM_CONSTANT(OPTIONS);
    BIND_ENUM_CONSTANT(STATS);
    BIND_ENUM_CONSTANT(ITEM);
    BIND_ENUM_CONSTANT(ITEM_ACTION);
    BIND_ENUM_CONSTANT(CELL);
    BIND_ENUM_CONSTANT(ITEM_USE_DISABLE_MOVEMENT);
    
    ClassDB::bind_method(D_METHOD("_in_state", "state"), &UI::_in_state);
    ClassDB::bind_method(D_METHOD("_set_enabled_options"), &UI::_set_enabled_options);
    ClassDB::bind_method(D_METHOD("_write_options"), &UI::_write_options);
    ClassDB::bind_method(D_METHOD("_set_overview"), &UI::_set_overview);
    ClassDB::bind_method(D_METHOD("_set_detailed"), &UI::_set_detailed);
    ClassDB::bind_method(D_METHOD("_set_items"), &UI::_set_items);
    ClassDB::bind_method(D_METHOD("_set_cells"), &UI::_set_cells);
    ClassDB::bind_method(D_METHOD("_close_menu"), &UI::_close_menu);
    ClassDB::bind_method(D_METHOD("soul_move", "action"), &UI::soul_move); 
    ClassDB::bind_method(D_METHOD("_on_animation_finished"), &UI::_on_animation_finished);
    ClassDB::bind_method(D_METHOD("_on_item_dialogue_finished"), &UI::_on_item_dialogue_finished);
    
    ClassDB::bind_method(D_METHOD("set_items_seperation", "seperation"), &UI::set_items_seperation);
    ClassDB::bind_method(D_METHOD("get_items_seperation"), &UI::get_items_seperation);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "items_seperation"), "set_items_seperation", "get_items_seperation");
    
    ClassDB::bind_method(D_METHOD("set_option_seperation", "seperation"), &UI::set_option_seperation);
    ClassDB::bind_method(D_METHOD("get_option_seperation"), &UI::get_option_seperation);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "option_seperation"), "set_option_seperation", "get_option_seperation");

    ClassDB::bind_method(D_METHOD("set_enabled_options", "options"), &UI::set_enabled_options);
    ClassDB::bind_method(D_METHOD("get_enabled_options"), &UI::get_enabled_options);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "enabled_options"), "set_enabled_options", "get_enabled_options");
    
    ClassDB::bind_method(D_METHOD("set_options_dict", "dict"), &UI::set_options_dict);
    ClassDB::bind_method(D_METHOD("get_options_dict"), &UI::get_options_dict);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "options_dict"), "set_options_dict", "get_options_dict");

    ClassDB::bind_method(D_METHOD("set_is_kill", "value"), &UI::set_is_kill);
}

void UI::_ready() {
    if(isEditor) return;
    soul = Object::cast_to<MenuSoul>(get_node_internal("Control/StatAndOptions/Soul"));
    stats = Object::cast_to<UI_Box>(get_node_internal("Control/StatAndOptions/Detailed"));
    items = Object::cast_to<UI_Box>(get_node_internal("Control/StatAndOptions/Items"));
    cells = Object::cast_to<UI_Box>(get_node_internal("Control/StatAndOptions/Cells"));
    
    item_actions = Dictionary();
    item_actions[0.0] = get_node_internal("Control/StatAndOptions/Items/Use");
    item_actions[1.0] = get_node_internal("Control/StatAndOptions/Items/Info");
    item_actions[2.0] = get_node_internal("Control/StatAndOptions/Items/Drop");
    
    textboxscene = ResourceLoader::get_singleton()->load("res://Overworld/text_box.tscn");
    soultarget = Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Options/Options"))->get_global_position();

    if(global->get_player_position().y >= 240) {
        get_node_internal("Control/StatAndOptions/Stats")->call("set_position", Vector2(0, 280));
    }
    
    global->set_player_in_menu(true);
    
    _set_overview();
    _write_options();
    _set_enabled_options();
    
    get_node_internal("Control/StatAndOptions/Options")->call("grow");
    get_node_internal("Control/StatAndOptions/Stats")->call("grow");
}

void UI::_in_state(States state) {
    pos_history[current_state] = soulposition;
    current_state = state;
    
    if (pos_history[state].get_type() != Variant::NIL) {
        soulposition = pos_history[state];
        Vector2 vec = optionsize[state];
        
        while (soulposition.x >= vec.x) {
            soulposition.x -= 1;
        }
        while (soulposition.y >= vec.y) {
            soulposition.y -= 1;
        }
    } else {
        soulposition = Vector2();
    }
    
    switch(state) {
        case OPTIONS:
            items->shrink();
            stats->shrink();
            cells->shrink();
            break;
        case ITEM:
            _set_items();
            items->grow();
            break;
        case STATS:
            _set_detailed();
            stats->grow();
            break;
        case CELL:
            _set_cells();
            cells->grow();
            break;
        case ITEM_USE_DISABLE_MOVEMENT:
            items->shrink();
            break;
        default:
            break;
    }
    
    soul_move(Vector2());
    Object::cast_to<GPUParticles2D>(get_node_internal("Control/StatAndOptions/Soul/Ghost"))->restart();
    Object::cast_to<GPUParticles2D>(get_node_internal("Control/StatAndOptions/Soul/Ghost"))->set_emitting(true);
}

void UI::_set_enabled_options() {
    TypedArray<bool> enabled_array;
    enabled_array.resize(enabled_options.size());
    
    for (int i = 0; i < enabled_options.size(); i++) {
        switch (i) {
            case 0:
                enabled_array[i] = global->get_items().size() > 0 && enabled_options[i];
                break;
            case 1:
                enabled_array[i] = enabled_options[i];
                break;
            case 2:
                enabled_array[i] = global->get_cells().size() > 0 && enabled_options[i];
                break;
            case 3:
                if (global->get_boxesinmenu()) {
                    enabled_array[i] = global->get_unlockedboxes() > 0;
                }
                break;
        }
    }
    
    enabled_options = enabled_array;
}

void UI::_write_options() {
    _set_enabled_options();
    String txt;
    
    for (int i = 0; i < enabled_options.size(); i++) {
        bool is_enabled = enabled_options[i];
        String color = is_enabled ? "white" : "gray";
        String option_text = options_dict[i];
        txt += vformat("[color=%s]%s[/color]\n", color, option_text);
    }
    
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Options/Options"))->set_text(txt);
}

void UI::_set_overview() {
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Stats/Name"))->set_text(global->get_player_name());
    
    String stats_text = vformat("LV %s\nHP %s/%s\nG   %s",
        global->get_player_lv(),
        global->get_player_hp(),
        global->get_player_max_hp(),
        global->get_player_gold()
    );
    
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Stats/Stats"))->set_text(stats_text);
}

void UI::_set_detailed() {
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Name"))->set_text(global->get_player_name());
    
    String hp_text = vformat("HP %s/%s",
        global->get_player_hp(),
        global->get_player_max_hp()
    );
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Hp"))->set_text(hp_text);
    
    Ref<Item> weapon_item = global->get_item_list()[global->get_equipment()["weapon"]];
    Ref<Item> armor_item = global->get_item_list()[global->get_equipment()["armor"]];
    
    int weapon_attack = weapon_item->get_attack_amount();
    int armor_attack = armor_item->get_attack_amount();
    int weapon_defense = weapon_item->get_defense_amount();
    int armor_defense = armor_item->get_defense_amount();
    
    String stats_text = vformat("AT %s(%s) \nDF %s(%s)",
        global->get_player_attack(),
        weapon_attack + armor_attack,
        global->get_player_defense(),
        weapon_defense + armor_defense
    );
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Stats"))->set_text(stats_text);
    
    String equipment_text = vformat(String::utf8("무기: %s \n갑옷: %s"),
        weapon_item->get_item_name(),
        armor_item->get_item_name()
    );
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Equipment"))->set_text(equipment_text);
    
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Gold"))->set_text(vformat("GOLD: %s", global->get_player_gold()));
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Lv"))->set_text(vformat("LV %s", global->get_player_lv()));
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Exp"))->set_text(vformat("EXP %s", global->get_player_exp()));
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Kills"))->set_text(vformat("KILLS: %s", global->get_player_kills()));
}

void UI::_set_items() {
    String txt;
    
    for (int i = 0; i < global->get_items().size(); i++) {
        Ref<Item> item = global->get_item_list()[global->get_items()[i]];
        txt += vformat("%s\n", item->get_item_name());
    }
    
    optionsize[ITEM] = Vector2(1, global->get_items().size());
    
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Items/Items"))->set_text(txt);
}

void UI::_set_cells() {}

void UI::_close_menu() {
    items->shrink();
    stats->shrink();
    cells->shrink();
    
    global->set_player_in_menu(false);
    set_process_unhandled_input(false);
    
    get_node_internal("Control/StatAndOptions/Stats")->call("shrink");
    get_node_internal("Control/StatAndOptions/Options")->call("shrink");
    
    Ref<Tween> tw = get_tree()->create_tween();
    tw->tween_property(soul, "modulate:a", 0.0, 0.2)->set_ease(Tween::EASE_OUT)->set_trans(Tween::TRANS_EXPO);
    UI_Box* tw_property = Object::cast_to<UI_Box>(get_node_internal("Control/StatAndOptions/Options"));
    tw_property->get_tw()->connect("finished", Callable(this, "_on_animation_finished"));
}

void UI::_unhandled_input(const Ref<InputEvent>& event) {
    // 텍스트 박스가 활성화되어 있으면 무시
    if(textbox || isEditor) return;
    
    if(event->is_action_pressed("ui_down")) {
        soul_move(Vector2(0, 1));
    }
    if(event->is_action_pressed("ui_up")) {
        soul_move(Vector2(0, -1));
    }
    if(event->is_action_pressed("ui_right")) {
        soul_move(Vector2(1, 0));
    }
    if(event->is_action_pressed("ui_left")) {
        soul_move(Vector2(-1, 0));
    }
    
    if(event->is_action_pressed("ui_accept")) {
        Object::cast_to<GPUParticles2D>(get_node_internal("Control/StatAndOptions/Soul/Ghost"))->restart();
        Object::cast_to<GPUParticles2D>(get_node_internal("Control/StatAndOptions/Soul/Ghost"))->set_emitting(true);
        
        Object::cast_to<AudioStreamPlayer>(get_node_internal("select"))->play();
        
        switch (current_state) {
            case OPTIONS: {
                if (bool(enabled_options[int(soulposition.y)])) {
                    switch (int(soulposition.y)) {
                        case 0:
                            _in_state(ITEM);
                            break;
                        case 1:
                            _in_state(STATS);
                            break;
                        case 2:
                            _in_state(CELL);
                            break;
                    }
                }
                break;
            }
            case ITEM: {
                soulposition_item.y = soulposition.y;
                _in_state(ITEM_ACTION);
                break;
            }
            case CELL: {
                _in_state(CELL);
                break;
            }
            case ITEM_ACTION: {
                soul->hide();
                switch (int(soulposition.x)) {
                    case 0: {
                        _in_state(ITEM_USE_DISABLE_MOVEMENT);
                        Ref<Item> item = global->get_item_list()[global->get_items()[soulposition_item.y]];
                        textbox = Object::cast_to<TextBox>(textboxscene->instantiate());
                        global->get_scene_container()->get_current_scene()->add_child(textbox);
                        textbox->connect("dialogue_finished", Callable(this, "_on_item_dialogue_finished"), CONNECT_ONE_SHOT);
                        Ref<Dialogues> dialogues = memnew(Dialogues);
                        
                        if (item->get_item_type() != Item::CONSUMABLE) {
                            // 장비 아이템인 경우
                            PackedStringArray txt = global->equip_item(global->get_items()[soulposition_item.y]);
                            Array items = global->get_items();
                            items.remove_at(soulposition_item.y);
                            global->set_items(items);
                            _set_items();
                            dialogues->from(txt);
                            textbox->generic(dialogues);
                        } else {
                            // 소비 아이템인 경우
                            PackedStringArray txt = global->item_use_text(global->get_items()[soulposition_item.y]);
                            Array items = global->get_items();
                            items.remove_at(soulposition_item.y);
                            global->set_items(items);
                            _set_items();
                            dialogues->from(txt);
                            textbox->generic(dialogues);
                        }
                        break;
                    }
                    case 1: {
                        _in_state(ITEM_USE_DISABLE_MOVEMENT);
                        textbox = Object::cast_to<TextBox>(textboxscene->instantiate());
                        global->get_scene_container()->get_current_scene()->add_child(textbox);
                        Ref<Item> item = global->get_item_list()[global->get_items()[soulposition_item.y]];
                        PackedStringArray info = item->get_item_information();
                        Ref<Dialogues> dialogues = memnew(Dialogues);
                        dialogues->from(info);
                        textbox->generic(dialogues);
                        textbox->connect("dialogue_finished", Callable(this, "_on_item_dialogue_finished"), CONNECT_ONE_SHOT);
                        break;
                    }
                    case 2: {
                        _in_state(ITEM_USE_DISABLE_MOVEMENT);
                        Ref<Item> item = global->get_item_list()[global->get_items()[soulposition_item.y]];
                        PackedStringArray txt = item->get_throw_message();
                        textbox = Object::cast_to<TextBox>(textboxscene->instantiate());
                        global->get_scene_container()->get_current_scene()->add_child(textbox);
                        _set_items();
                        Ref<Dialogues> dialogues = memnew(Dialogues);
                        dialogues->from(txt);
                        Array items = global->get_items();
                        items.remove_at(soulposition_item.y);
                        global->set_items(items);
                        textbox->generic(dialogues);
                        textbox->connect("dialogue_finished", Callable(this, "_on_item_dialogue_finished"), CONNECT_ONE_SHOT);
                        break;
                    }
                }
                _set_overview();
                break;
            }
            default:
                break;
        }
    }
    
    // 취소 버튼 처리
    if(event->is_action_pressed("ui_cancel")) {
        switch (current_state) {
            case ITEM:
            case CELL:
            case STATS:
                _in_state(OPTIONS);
                break;
            case ITEM_ACTION:
                _in_state(ITEM);
                break;
            case OPTIONS:
                _close_menu();
                break;
            default:
                break;
        }
    }else if(current_state == OPTIONS && event->is_action_pressed("ui_menu")) {
        call_deferred("_close_menu");
    }
}

bool UI::soul_move(const Vector2& action) {
    Object::cast_to<AudioStreamPlayer>(get_node_internal("choice"))->play();
    Vector2 vec = optionsize[current_state];

    if (soulposition.x + action.x > vec.x - 1) return false;
    if (soulposition.y + action.y > vec.y - 1) return false;
    if (soulposition.x + action.x < 0) return false;
    if (soulposition.y + action.y < 0) return false;
    
    soulposition += action;
    
    switch (current_state) {
        case OPTIONS: {
            RichTextLabel* options_node = Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Options/Options"));
            soultarget = options_node->get_global_position() + soulposition * option_seperation;
            break;
        }
        case ITEM: {
            RichTextLabel* items_node = Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Items/Items"));
            soultarget = items_node->get_global_position() + soulposition * items_seperation;
            break;
        }
        case ITEM_ACTION: {
            Node* action_node = Object::cast_to<Node>(item_actions[soulposition.x]);
            soultarget = action_node->call("get_global_position");
            break;
        }
        case CELL: {
            RichTextLabel* numbers_node = Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Cells/Numbers"));
            soultarget = numbers_node->get_global_position() + soulposition * items_seperation;
            break;
        }
        default:
            break;
    }

    soul->move_global(soultarget + Vector2(-12, 15));
    return true;
}

void UI::set_items_seperation(const Vector2& p_sep) {
    items_seperation = p_sep;
}

Vector2 UI::get_items_seperation() const {
    return items_seperation;
}

void UI::set_option_seperation(const Vector2& p_sep) {
    option_seperation = p_sep;
}

Vector2 UI::get_option_seperation() const {
    return option_seperation;
}

void UI::set_options_dict(const Dictionary& p_dict) {
    options_dict = p_dict;
}

Dictionary UI::get_options_dict() const {
    return options_dict;
}

void UI::set_is_kill(bool value) {
    Object::cast_to<RichTextLabel>(get_node_internal("Control/StatAndOptions/Detailed/Kills"))->set_visible(value);
}

void UI::_on_animation_finished() {
    queue_free();
}

void UI::_on_item_dialogue_finished() {
    textbox = nullptr;
    global->set_player_in_menu(true);
    _write_options();
    _in_state(OPTIONS);
    soul->show();
}

void UI::set_enabled_options(const Array& options) {
    enabled_options = options;
}

Array UI::get_enabled_options() const {
    return enabled_options;
}