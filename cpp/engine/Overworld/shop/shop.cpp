#include "shop.h"
#include "env.h"

SHOP::SHOP() {
    soul_position = 0;
    current_state = States::SELECTING_ACTIONS;
    can_be_sold_to = true;
    
    soul_positions = Dictionary();
    Array pos_array1;
    pos_array1.push_back(Vector2(38, 38));
    pos_array1.push_back(Vector2(0, 44));
    soul_positions[0] = pos_array1;
    
    Array pos_array2;
    pos_array2.push_back(Vector2(52, 43));
    pos_array2.push_back(Vector2(0, 43));
    soul_positions[1] = pos_array2;

    info_panel = Dictionary();
    info_panel[true] = Vector2(422, 40);
    info_panel[false] = Vector2(422, 250);
    info_panel["time"] = 0.6;

    keeper_box = Dictionary();
    keeper_box[false] = Vector2(419, 239);
    keeper_box[true] = Vector2(642, 239);
    keeper_box["time"] = 0.2;
}

SHOP::~SHOP() {}

void SHOP::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_set_item_panel"), &SHOP::_set_item_panel);
    ClassDB::bind_method(D_METHOD("_set_keeper_box"), &SHOP::_set_keeper_box);
    ClassDB::bind_method(D_METHOD("_refresh_item_panel"), &SHOP::_refresh_item_panel);
    ClassDB::bind_method(D_METHOD("_refresh_g_info", "red"), &SHOP::_refresh_g_info, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("_set_soul_pos"), &SHOP::_set_soul_pos);
    ClassDB::bind_method(D_METHOD("_exit"), &SHOP::_exit);
    ClassDB::bind_method(D_METHOD("_in_state", "new_state"), &SHOP::_in_state);
    ClassDB::bind_method(D_METHOD("_get_sell_items_count"), &SHOP::_get_sell_items_count);
    ClassDB::bind_method(D_METHOD("_get_sell_items", "id", "size"), &SHOP::_get_sell_items, DEFVAL(4));
    ClassDB::bind_method(D_METHOD("_write_sell_items", "id"), &SHOP::_write_sell_items);
    ClassDB::bind_method(D_METHOD("_write_shop_items"), &SHOP::_write_shop_items);
    ClassDB::bind_method(D_METHOD("_keeper_dialogue", "dialogues"), &SHOP::_keeper_dialogue);
    ClassDB::bind_method(D_METHOD("_keeper_dialogue_temp", "dialogues", "return_state"), &SHOP::_keeper_dialogue_temp);
    ClassDB::bind_method(D_METHOD("_on_keeper_dialogue_started_typing", "index"), &SHOP::_on_keeper_dialogue_started_typing);
    ClassDB::bind_method(D_METHOD("_on_keeper_dialogue_finished_all_texts"), &SHOP::_on_keeper_dialogue_finished_all_texts);
    
    ADD_SIGNAL(MethodInfo("keeper_expression", PropertyInfo(Variant::ARRAY, "exp")));
}

void SHOP::_ready() {
    if(isEditor) return;
    Array rects_array;
    rects_array.push_back(get_node_internal("Control/Main"));
    rects_array.push_back(get_node_internal("Control/TextBox"));
    rects = rects_array;
    
    items_info_box = Object::cast_to<NinePatchRect>(get_node_internal("Control/Info"));
    items_info = Object::cast_to<RichTextLabel>(get_node_internal("Control/Info/ItemInfo"));
    items = Object::cast_to<RichTextLabel>(get_node_internal("Control/TextBox/Items"));
    
    Array gold_inv_array;
    gold_inv_array.push_back(get_node_internal("Control/Main/Gold"));
    gold_inv_array.push_back(get_node_internal("Control/Main/Space"));
    gold_inv_info = gold_inv_array;
    
    sell_items = Object::cast_to<RichTextLabel>(get_node_internal("Control/TextBox/SellItems"));
    dialogue_options = Object::cast_to<RichTextLabel>(get_node_internal("Control/TextBox/Dialogues"));
    keeper_dialogue = Object::cast_to<AdvancedTextTyper>(get_node_internal("Control/TextBox/MarginContainer/Dialogue"));
    item_slider_thing = Object::cast_to<ItemSlider>(get_node_internal("Control/TextBox/Slider"));
    soul = Object::cast_to<MenuSoul>(get_node_internal("Control/Main/Soul"));
    camera = global->get_scene_container()->get_camera();
}

void SHOP::init() {
    for(String str : dialogue_option) {
        if (str.is_empty()) continue;
        dialogue_options->add_text(str + "\n");
    }
    option_numbers.push_back(3);
    option_numbers.push_back(dialogue_option.size() - 1);
    option_numbers.push_back(offerings.size() - 1);
    option_numbers.push_back(global->get_items().size() - 1);
    _in_state(SELECTING_ACTIONS);
    _refresh_g_info();
}

void SHOP::_unhandled_input(const Ref<InputEvent>& event) {
    if (!(current_state < VIEWING_DIALOGUE) || isEditor) return;

    if (event->is_action_pressed("ui_down") && soul_position < option_numbers[current_state]) {
        soul_position += 1;
        Object::cast_to<AudioStreamPlayer>(get_node_internal("select"))->play();
        _set_soul_pos();
    }
    
    if (event->is_action_pressed("ui_up") && soul_position > 0) {
        soul_position -= 1;
        Object::cast_to<AudioStreamPlayer>(get_node_internal("select"))->play();
        _set_soul_pos();
    }
    
    if (event->is_action_pressed("ui_cancel")) {
        switch (current_state) {
            case BUYING_ITEMS:
                soul_position = 0;
                _in_state(SELECTING_ACTIONS);
                _refresh_g_info();
                break;
            case SELECTING_DIALOGUE:
                soul_position = 2;
                _in_state(SELECTING_ACTIONS);
                break;
            case SELLING_ITEMS:
                soul_position = 1;
                _in_state(SELECTING_ACTIONS);
                break;
        }
    }
    
    if (event->is_action_pressed("ui_accept")) {
        switch (current_state) {
            case SELECTING_ACTIONS:
                keeper_dialogue->kill_tweens(true);
                switch (soul_position) {
                    case 0:
                        soul_position = 0;
                        _in_state(BUYING_ITEMS);
                        break;
                    case 1:
                        if (can_be_sold_to && global->get_items().size() > 0 && _get_sell_items_count() > 0) {
                            soul_position = 0;
                            option_numbers.set(3, _get_sell_items_count() - 1);
                            _in_state(SELLING_ITEMS);
                        } else {
                            _keeper_dialogue_temp(keeper_cannot_sell_dialogues, SELECTING_ACTIONS);
                        }
                        break;
                    case 2:
                        soul_position = 0;
                        _in_state(SELECTING_DIALOGUE);
                        break;
                    case 3:
                        _exit();
                        break;
                }
                break;
            case SELECTING_DIALOGUE:
                _keeper_dialogue_temp(keeper_dialogues[soul_position], SELECTING_DIALOGUE);
                break;
            case BUYING_ITEMS: {
                int player_gold = global->get_player_gold();
                Array items = global->get_items();

                Ref<ShopItem> offering = offerings[soul_position];
                if (player_gold >= offering->get_cost() && items.size() < 8) {
                    global->set_player_gold(player_gold - offering->get_cost());
                    items.push_back(offering->get_item());
                    global->set_items(items);
                    Object::cast_to<AudioStreamPlayer>(get_node_internal("bought"))->play();
                    _refresh_g_info();
                } else {
                    Object::cast_to<AudioStreamPlayer>(get_node_internal("insufficient"))->play();
                    if (items.size() >= 8) {
                        _refresh_g_info(true);
                    }
                }
                break;
            }
            case SELLING_ITEMS: {
                Array items = global->get_items();
                if (items.size() > soul_position && items[soul_position].operator int() >= 0) {
                    items.remove_at(soul_position);
                    global->set_items(items);
                    _refresh_g_info();
                    Object::cast_to<AudioStreamPlayer>(get_node_internal("bought"))->play();
                    option_numbers.set(3, _get_sell_items_count() - 1);
                    
                    if (soul_position > option_numbers[3]) {
                        soul_position = option_numbers[3];
                    }
                    _set_soul_pos();
                    _write_sell_items(soul_position);
                    
                    if (!_get_sell_items_count()) {
                        soul_position = 1;
                        _in_state(SELECTING_ACTIONS);
                    }
                }
                break;
            }
        }
    }
}

void SHOP::_in_state(int new_state) {
    current_state = States(new_state);
    dialogue_options->hide();
    keeper_dialogue->hide();
    sell_items->hide();
    items_info->hide();
    soul->show();
    items->hide();
    
    Node* soul_parent = soul->get_parent();
    if (soul_parent) {
        soul_parent->remove_child(soul);
    }
    
    Object::cast_to<Node>(rects[MIN(new_state, 1)])->add_child(soul);
    _set_item_panel();
    
    switch (current_state) {
        case SELECTING_ACTIONS:
            keeper_dialogue->show();
            _keeper_dialogue(keeper_def_dialogue);
            _set_soul_pos();
            break;
        case BUYING_ITEMS:
            _write_shop_items();
            items->show();
            _set_soul_pos();
            break;
        case SELECTING_DIALOGUE:
            dialogue_options->show();
            _set_soul_pos();
            break;
        case VIEWING_DIALOGUE:
            _set_keeper_box();
            soul->hide();
            keeper_dialogue->show();
            break;
        case SELLING_ITEMS:
            sell_items->show();
            _write_sell_items(0);
            _set_soul_pos();
            break;
    }
}

void SHOP::_set_item_panel() {
    if (tw.is_valid()) {
        tw->kill();
    }
    tw = create_tween()->set_trans(Tween::TRANS_QUART);
    
    if (current_state == BUYING_ITEMS) {
        items_info_box->set_visible(true);
    }
    
    tw->tween_property(items_info_box, "position", info_panel[current_state == BUYING_ITEMS], info_panel["time"]);
    
    if (current_state != BUYING_ITEMS) {
        tw->tween_callback(Callable(items_info_box, "set_visible").bind(false));
    }
    
    _refresh_item_panel();
}

void SHOP::_set_keeper_box() {
    if (tw2.is_valid()) {
        tw2->kill();
    }
    
    tw2 = create_tween()->set_trans(Tween::TRANS_CIRC)->set_parallel();
    
    if (current_state == VIEWING_DIALOGUE) {
        tw2->tween_property(rects[0], "modulate:a", float(current_state != VIEWING_DIALOGUE), keeper_box["time"]);
    }
    
    tw2->tween_property(rects[1], "size", keeper_box[current_state == VIEWING_DIALOGUE], keeper_box["time"]);
    
    if (current_state != VIEWING_DIALOGUE) {
        tw2->tween_property(rects[0], "modulate:a", float(current_state != VIEWING_DIALOGUE), keeper_box["time"]);
    }
}

void SHOP::_refresh_item_panel() {
    items_info->set_visible(current_state == BUYING_ITEMS);
    
    if (items_info->is_visible()) {
        Ref<ShopItem> offering = offerings[soul_position];

        TypedArray<Item> item_list = global->get_item_list();
        Ref<Item> item_data = item_list[offering->get_item()];
        PackedStringArray item_info = item_data->get_item_information();
        items_info->set_text(item_info[0]);
    }
}

void SHOP::_refresh_g_info(bool red) {
    int player_gold = global->get_player_gold();
    Array items = global->get_items();
    
    Object::cast_to<RichTextLabel>(gold_inv_info[0])->set_text(vformat("%sG", player_gold));
    Object::cast_to<RichTextLabel>(gold_inv_info[1])->set_text(vformat("%s%s/8", red ? "[color=red]" : "",  items.size()));
}

void SHOP::_keeper_dialogue(Ref<Dialogues> dialogues) {
    dialogue_expressions = dialogues->get_dialogues_single(Dialogues::DIALOGUE_EXPRESSIONS);

    global->call_deferred("set_player_text_box", true);
    keeper_dialogue->call_deferred("type_text_advanced", dialogues);
}

void SHOP::_keeper_dialogue_temp(Ref<Dialogues> dialogues, int return_state) {
    temp_return_state = return_state;
    _in_state(VIEWING_DIALOGUE);
    _keeper_dialogue(dialogues);
}

void SHOP::_on_keeper_dialogue_started_typing(int index) {
    if (dialogue_expressions.size() > index) {
        emit_signal("keeper_expression", dialogue_expressions[index]);
    }
}

void SHOP::_on_keeper_dialogue_finished_all_texts() {
    global->set_player_text_box(false);
    if(current_state == VIEWING_DIALOGUE) {
        get_viewport()->set_input_as_handled();
        _in_state(temp_return_state);
        _set_keeper_box();
    }
}

void SHOP::_set_soul_pos() {
    if (current_state == SELLING_ITEMS) {
        _write_sell_items(soul_position);

        int size = _get_sell_items_count() - 1;
        int x = soul_position; 
        float y = 16 / size;
        if(size == soul_position) {
            item_slider_thing->set_value(16);
        }else item_slider_thing->set_value(x * y + 1);
    }
    
    item_slider_thing->set_visible(current_state == SELLING_ITEMS);
    
    if (current_state == BUYING_ITEMS) {
        _refresh_item_panel();
    }
    
    Array pos_info = soul_positions[MIN(current_state, 1)];
    Vector2 base_pos = pos_info[0];
    Vector2 offset = pos_info[1];
    
    Vector2 new_pos = base_pos + offset * (current_state != SELLING_ITEMS ? soul_position : 0);
    soul->move(new_pos);
}

void SHOP::_exit() {
    RoomEntranceNode* exit_node = Object::cast_to<RoomEntranceNode>(get_node_internal("Control/room_exit"));
    exit_node->force_enter();
}

int SHOP::_get_sell_items_count() const {
    int count = 0;
    Array items = global->get_items();
    
    for (int i = 0; i < items.size(); i++) {
        for (int j = 0; j < sellferings.size(); j++) {
            Ref<ShopItem> shop_item = sellferings[j];
            if(int(items[i]) == shop_item->get_item()) {
                count++;
                break;
            }
        }
    }
    
    return count;
}

String SHOP::_get_sell_items(int id, int size) const {
    String txt = "";
    int count = 0;
    Array items = global->get_items();
    TypedArray<Item> item_list = global->get_item_list();
    
    for (int i = 0; i < items.size(); i++) {
        for (int j = 0; j < sellferings.size(); j++) {
            Ref<ShopItem> shop_item = sellferings[j];
            if (int(items[i]) == shop_item->get_item()) {
                count++;
                if (count < id) continue;
                
                Ref<Item> item_data = item_list[items[i]];
                txt += vformat("%s - %sG\n", item_data->get_item_name(), shop_item->get_cost());
                if (count >= id + size - 1) return txt;
                break;
            }
        }
    }
    return txt;
}

void SHOP::_write_sell_items(int id) {
    sell_items->set_text(_get_sell_items(id + 1));
}

void SHOP::_write_shop_items() {
    String txt = "";
    TypedArray<Item> item_list = global->get_item_list();
    
    for (int i = 0; i < offerings.size(); i++) {
        Ref<ShopItem> shop_item = offerings[i];
        Ref<Item> item_data = item_list[shop_item->get_item()];
        txt += vformat("%sG - %s\n", shop_item->get_cost(), item_data->get_item_name());
    }
    items->set_text(txt);
}

void SHOP::set_offerings(const Array& p_offerings) {
    offerings = p_offerings;
}

void SHOP::set_sellferings(const Array& p_sellferings) {
    sellferings = p_sellferings;
}

void SHOP::set_keeper_dialogues(const Array& p_keeper_dialogues) {
    keeper_dialogues = p_keeper_dialogues;
}

void SHOP::set_keeper_def_dialogue(const Ref<Dialogues>& p_keeper_def_dialogue) {
    keeper_def_dialogue = p_keeper_def_dialogue;
}

void SHOP::set_can_be_sold_to(bool p_can_be_sold_to) {
    can_be_sold_to = p_can_be_sold_to;
}

void SHOP::set_keeper_cannot_sell_dialogues(const Ref<Dialogues>& p_keeper_cannot_sell_dialogues) {
    keeper_cannot_sell_dialogues = p_keeper_cannot_sell_dialogues;
}

void SHOP::set_dialogue_options(const PackedStringArray& p_dialogue_options) {
    dialogue_option = p_dialogue_options;
}