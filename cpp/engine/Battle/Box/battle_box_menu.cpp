#include "battle_box.h"
#include "env.h"
#include "box_behaviour.h"
#include "engine/Battle/battle_system.h"
#include<godot_cpp/variant/utility_functions.hpp>

// 사용 함수
void BattleBox::blitter_print(PackedStringArray texts) {
    if(!main->player_turn) {
        ERR_PRINT("플레이어 턴이 아닐 때는 박스의 대사를 출력할 수 없습니다.");
        return;
    }
    global->_set_battle_text_box(true);
    change_state(BattleState::State_Blittering);
    blitter_text->type_text(texts);
    blitter_text->connect("finished_all_texts", Callable(this, "emit_signal").bind("blitter_end"), CONNECT_ONE_SHOT);
    blitter_text->connect("finished_all_texts", Callable(global, "_set_battle_text_box").bind(false), CONNECT_ONE_SHOT);
}
// * * //


// 내부 함수 //
void BattleBox::set_enemies(const Array p_enemies) {
    enemies = p_enemies;
    if(enemies_hp.size() == 0) {
        enemies_hp.resize(enemies.size());
        for(int i=0; i < enemies.size(); i++) {
            Enemy *enemie = Object::cast_to<Enemy>(enemies[i]);
            enemies_hp[i] = enemie->get_stats()["hp"];
        }
    }
    _set_targets();
}

void BattleBox::_set_targets(bool show_hp_bar) {
    float max_name_width = 0;
    String targets = "";
    for(int i=0; i < enemies.size(); i++) {
        Enemy *enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy) {
            Ref<EnemyAct> state = enemy->get_enemy_acts()[enemy->get_current_act()];
            bool sparable = state->get_sparable(); 
            String enemy_name = "* " + tr(enemy->get_enemy_name());
            targets += vformat("[color=%s]%s[/color]\n", sparable ? "yellow" : "white", enemy_name);
            
            Ref<Font> font = target_label->get_theme_font("normal_font");
            int font_size = target_label->get_theme_font_size("normal_font_size");
            if(font.is_valid()) {
                float text_width = font->get_string_size(enemy_name, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size).x;
                max_name_width = MAX(max_name_width, text_width);
            }
        }else {
            targets += "[color=white][/color]\n";
        }
    }
    target_label->set_text(targets);
    
    if(max_name_width > 0) {
        int new_margin = MAX(220, int(74 + max_name_width + 20));
        new_margin = MIN(new_margin, 380);
        hp_bar_container->add_theme_constant_override("margin_left", new_margin);
    }
    
    for(int i=0; i < 3; i++) {
        ProgressBar *bar = Object::cast_to<ProgressBar>(hp_bars[i]);
        if(i < enemies.size() && enemies[i] && hp_bars[i]) {
            Enemy *enemy = Object::cast_to<Enemy>(enemies[i]);
            if(enemy) {
                Dictionary stats = enemy->get_stats();
                bar->set_visible(show_hp_bar);
                bar->set_max(stats["max_hp"]);
                bar->set_value(enemies_hp[i]);
            }else {
                bar->set_visible(false);
            }
        }else if(hp_bars[i]) {
            bar->set_visible(false);
        }
    }
}

void BattleBox::set_mercy_options() {
    String txt = "";
    
    String spare_color = "white";
    for(int i=0; i < enemies.size(); i++) {
        Enemy *enemy = Object::cast_to<Enemy>(enemies[i]);
        if(enemy) {
            Ref<EnemyAct> state = enemy->get_enemy_acts()[enemy->get_current_act()];
            bool sparable = state->get_sparable();
            if (sparable) {
                spare_color = "yellow";
                break;
            }
        }
    }
    
    Ref<Encounter> encounter = main->get_encounter();
    PackedStringArray mercy_options = encounter->get_mercy_options();
    
    for(int i=0; i < mercy_options.size(); i++) {
        txt += vformat("[color=%s]%s[/color]\n", i == 0 ? spare_color : "white", tr(mercy_options[i]));
    }
    mercy_choices->set_text(txt);
    
    choices_extends.resize(mercy_options.size());
    choices_extends.fill(1);
}

int BattleBox::soul_pos_to_id(const Vector2& soul_pos, bool is, int x_limit) {
    if(is) { // 아이템 메뉴인 경우
        return current_page + soul_pos.y;
    }
    return soul_pos.y * x_limit + soul_pos.x;
}

Array BattleBox::id_to_soul_pos(int id, int x_limit) {
    Array result;
    
    while (id > 0) {
        if (id - x_limit > 0) {
            id -= x_limit;
            result.append(x_limit);
        } else {
            result.append(id);
            id = 0;
        }
    }
    
    return result;
}

void BattleBox::set_options() {
    Array acts;
    
    Enemy* enemy = Object::cast_to<Enemy>(enemies[current_target_id]);
    
    for(int i=0; i < 6; i++) {
        Ref<ActInfo> act = enemy->_get_act_info(i);
        if(!act.is_null()) acts.append(act->get_act_tr());
    }
    choices_extends = id_to_soul_pos(acts.size());
    
    String acts_p1 = "";
    String acts_p2 = "";
    for(int i=0; i < acts.size(); i++) {
        if(i == 0) acts_p1 = "";
        if(i == 1) acts_p2 = "";
        if (i % 2 == 0) {
            acts_p1 += String(acts[i]) + "\n";
        }else {
            acts_p2 += String(acts[i]) + "\n";
        }
    }
    
    column1->set_text(acts_p1);
    column2->set_text(acts_p2);
}

void BattleBox::_set_items() {
    PackedStringArray items;
    
    Array global_items = global->get_items();
    TypedArray<Item> item_list = global->get_item_list();
    
    int start_idx = current_page;
    int end_idx = Math::min<int>(start_idx + items_per_page, global_items.size());
    
    for (int i = start_idx; i < end_idx; i++) {
        Ref<Item> item = item_list[global_items[i]];
        items.push_back(item->get_item_name_tr() + "\n");
    }
    int size = global_items.size() - 1;
    int x = soul_pos_to_id(soul_position, true);
    float y = 8.0f / size;
    slider->set_value(x * y + 1);
    choices_extends.resize(Math::min(items_per_page, end_idx - start_idx));
    choices_extends.fill(1);

    items_label->set_text("* " + String("* ").join(items));
}

void BattleBox::blitter_flavour() {
    blitter_text->blitter(main->turn_number);
    action_memory.clear();
    change_state(BattleState::State_BlitteringCasual);
    blitter->show();
}

void BattleBox::blitter_act() {
    int target_id = soul_pos_to_id(soul_position, false, 2);
    Enemy* enemy = Object::cast_to<Enemy>(enemies[current_target_id]);
    
    if (enemy && blitter_text) {
        Ref<ActInfo> act_info = enemy->_get_act_info(target_id);
        PackedStringArray description = act_info->get_description();

        if(description.size() == 0) {
            emit_signal("act", current_target_id, target_id);
        }else {
            blitter_text->type_text(description);
            blitter_text->connect("finished_all_texts", Callable(this, "emit_signal").bind("act", current_target_id, target_id), CONNECT_ONE_SHOT);
        }
    }
}

void BattleBox::blitter_item() {
    Ref<Item> item = global->get_item_list()[used_item];
    PackedStringArray texts;
    
    // 장비 아이템인 경우
    if (item->get_item_type() == Item::WEAPON || item->get_item_type() == Item::ARMOR) {
        texts = global->equip_item(used_item);
    }else if(item->get_item_type() == Item::CONSUMABLE) {
        // 소비 아이템인 경우
        texts = global->item_use_text(used_item);
        if(item->get_heal_amount() > 0) stagehand->audio_player->play("heal");
    }else if(item->get_item_type() == Item::MISC) {
        // 기타 아이템인 경우
        texts = item->get_use_message();
    }

    if(item->get_item_type() != Item::MISC) {
        Array arr = global->get_items();
        for(int i=0; i < arr.size(); i++) {
            int item = arr[i];
            if(item == used_item) {
                arr.remove_at(i);
                break;
            }
        }
        global->set_items(arr);
    }
    
    blitter_text->type_text(texts);
    blitter_text->connect("finished_all_texts", Callable(this, "emit_signal").bind("item", used_item), CONNECT_ONE_SHOT);
}

void BattleBox::blitter_mercy() {
    mercy_choice = soul_pos_to_id(soul_position, false, 1);
    
    if (mercy_choice == 1) {
        float random = UtilityFunctions::randf();
        Ref<Encounter> encounter = main->get_encounter();
        
        if (random >= encounter->get_flee_chance()) {
            mercy_choice = -1;
        }
    }
    
    change_state(BattleState::State_Blittering);
    
    blitter_text->connect("finished_all_texts", Callable(this, "emit_signal").bind("mercy", mercy_choice), CONNECT_ONE_SHOT);
    blitter_text->type_text(mercy_texts[mercy_choice == -1 ? 2 : mercy_choice]);
}

void BattleBox::_soul_choice(const Vector2i& action) {
    if(int(action_memory.back()) == BattleState::State_Blittering || !can_move) return;
    Vector2i new_position = soul_position + action;
    
    if(int(action_memory.back()) == BattleState::State_Iteming) {
        can_move = false;
        emit_signal("move_soul", options_pos_base + options_pos_step * Vector2(0, new_position.y));
        soul_position = new_position;
        
        if(action != Vector2i(0, 0)) choice_sound->play();
        
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1, false);
        timer->connect("timeout", Callable(this, "_on_soul_move_cooldown"), CONNECT_ONE_SHOT);
    }else {
        if(new_position.y >= choices_extends.size() || new_position.y < 0) return;
        if(new_position.x >= int(choices_extends[new_position.y]) || new_position.x < 0) return;
        emit_signal("move_soul", options_pos_base + options_pos_step * new_position);
        if(action != Vector2i(0, 0)) choice_sound->play();
        soul_position = new_position;
    }
}

void BattleBox::_on_soul_move_cooldown() {
    can_move = true;
}

void BattleBox::_on_use_button(int choice) {
    soul_position = Vector2i(0, 0);
    button_choice = choice;
    int next_state;
    
    switch (choice) {
        case OPTION_FIGHT:
            next_state = BattleState::State_TargetEnemy;
            break;
        case OPTION_ACT:
            next_state = BattleState::State_TargetEnemy;
            break;
        case OPTION_ITEM:
            next_state = BattleState::State_Iteming;
            break;
        case OPTION_MERCY:
            next_state = BattleState::State_Mercying;
            break;
        default:
            next_state = BattleState::State_TargetEnemy;
            break;
    }
    
    change_state(next_state);
    refresh_options();
}

void BattleBox::_backout() {
    action_memory.resize(action_memory.size() - 1);
    refresh_nodes();
    soul_position = Vector2(0,0);
}

void BattleBox::change_state(int new_state) {
    if(!action_memory.is_empty()) {
        if(new_state == int(action_memory.back())) return;
    }
    
    if(new_state == BattleState::State_Disabled) {
        _disable();
        return;
    }
    
    action_memory.append(new_state);
    refresh_nodes();
}

void BattleBox::refresh_nodes() {
    refresh_options();
    current_state_node->lose_control();
    current_state_node = Object::cast_to<BattleBoxBehaviour>(current_state_nodes[action_memory.back()]);
    current_state_node->gain_control();
}

void BattleBox::refresh_options() {
    if (choices_extends.size() == 0) {
        soul_position = Vector2i(0, 0);
        return;
    }
    
    bool will_refresh = false;
    
    if (soul_position.y >= choices_extends.size()) {
        will_refresh = true;
    }else {
        int clamped_y = UtilityFunctions::clamp(soul_position.y, 0, 
            UtilityFunctions::max(choices_extends.size() - 1, 0));
        
        if (soul_position.x >= choices_extends[clamped_y].operator int()) {
            will_refresh = true;
        }
    }
    
    if(will_refresh) {
        while(soul_position.y > choices_extends.size() - 1) {
            soul_position.y -= 1;
        }
        
        if (soul_position.y >= 0 && soul_position.y < choices_extends.size()) {
            int min_idx = UtilityFunctions::min(soul_position.y, choices_extends.size() - 1);
            
            while(soul_position.x > int(choices_extends[min_idx]) - 1) {
                soul_position.x -= 1;
            }
        }
    }
}

void BattleBox::_disable() {
    Array screens_array = screens.values();
    
    for(int i=0; i < screens_array.size(); i++) {
        CanvasItem *screen = Object::cast_to<CanvasItem>(screens_array[i]);
        if(screen) screen->hide();
    }
    
    action_memory.resize(1);
    action_memory[0] = (int)BattleState::State_Disabled;
    
    if (button_choice != 0) button_choice = 0;
}

Blitter *BattleBox::get_blitter_text() const {
    return blitter_text;
}

RemoteTransform2D *BattleBox::get_tl() const {
    return tl;
}

RemoteTransform2D *BattleBox::get_br() const {
    return br;
}

void BattleBox::set_used_item(int value) {
    used_item = value;
}
// * * //