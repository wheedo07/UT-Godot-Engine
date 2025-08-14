#include "battle_box.h"
#include "env.h"
#include "box_behaviour.h"
#include "engine/Battle/battle_system.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/theme.hpp>
#include<godot_cpp/classes/scroll_container.hpp>
#include<godot_cpp/classes/slider.hpp>
#include<godot_cpp/classes/line2d.hpp>
#include<godot_cpp/classes/rectangle_shape2d.hpp>
#include<godot_cpp/core/property_info.hpp>

BattleBox::BattleBox() {
    items_per_page = 3;
    current_page = 0;
    scroll_counter = 0;
    options_pos_base = Vector2(76, 286);
    options_pos_step = Vector2(257, 30);
    mercy_choice = 0;
    current_target_id = 0;
    button_choice = 0;
    soul_position = Vector2i(0, 0);
    can_move = true;
    used_item = 0;
    current_web = 0;
    
    win_text = String::utf8("* 당신의 승리! \n* 당신은 %s EXP 와 %s Gold 를 얻었다");
    
    TransType = Tween::TRANS_QUAD;
    EaseType = Tween::EASE_OUT;
    
    mercy_texts.push_back(String::utf8("* 당신은 자비를 배풀었다"));
    mercy_texts.push_back(String::utf8("* 당신은 도망쳤다"));
    mercy_texts.push_back(String::utf8("* 당신은 도망치지 못했습니다."));
    
    anchor_targets.resize(2);
    anchor_targets[0] = Vector2(220, 140);
    anchor_targets[1] = Vector2(420, 340);
    
    action_memory.append((int)BattleState::State_Disabled);
    current_size = Vector2(0, 0);
    history = Array();
    for(int i=0; i < 4; i++) {
        Array arr;
        arr.resize(2);
        history.push_back(arr);
    }

    choices_extends = Array::make(1, 1, 1, 1, 1, 1);
}

BattleBox::~BattleBox() {}

void BattleBox::_bind_methods() {
    BIND_ENUM_CONSTANT(RELATIVE_TOP_LEFT);
    BIND_ENUM_CONSTANT(RELATIVE_TOP_RIGHT);
    BIND_ENUM_CONSTANT(RELATIVE_BOTTOM_LEFT);
    BIND_ENUM_CONSTANT(RELATIVE_BOTTOM_RIGHT);
    BIND_ENUM_CONSTANT(RELATIVE_CENTER);
    
    BIND_ENUM_CONSTANT(OPTION_FIGHT);
    BIND_ENUM_CONSTANT(OPTION_ACT);
    BIND_ENUM_CONSTANT(OPTION_ITEM);
    BIND_ENUM_CONSTANT(OPTION_MERCY);
    
    BIND_ENUM_CONSTANT(State_Disabled);
    BIND_ENUM_CONSTANT(State_BlitteringCasual);
    BIND_ENUM_CONSTANT(State_Blittering);
    BIND_ENUM_CONSTANT(State_TargetEnemy);
    BIND_ENUM_CONSTANT(State_Acting);
    BIND_ENUM_CONSTANT(State_Iteming);
    BIND_ENUM_CONSTANT(State_Mercying);
    BIND_ENUM_CONSTANT(State_Fighting);
    
    ADD_SIGNAL(MethodInfo("moved_to_buttons"));
    ADD_SIGNAL(MethodInfo("move_soul", PropertyInfo(Variant::VECTOR2, "newpos")));
    ADD_SIGNAL(MethodInfo("exit_menu"));
    ADD_SIGNAL(MethodInfo("act", PropertyInfo(Variant::INT, "target"), PropertyInfo(Variant::INT, "option")));
    ADD_SIGNAL(MethodInfo("fight", PropertyInfo(Variant::INT, "target")));
    ADD_SIGNAL(MethodInfo("item", PropertyInfo(Variant::INT, "item_choice")));
    ADD_SIGNAL(MethodInfo("mercy", PropertyInfo(Variant::INT, "target")));
    ADD_SIGNAL(MethodInfo("tweening_finished"));

    ClassDB::bind_method(D_METHOD("set_enemies", "p_enemies"), &BattleBox::set_enemies);
    ClassDB::bind_method(D_METHOD("set_targets", "show_hp_bar"), &BattleBox::set_targets, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("reset_box"), &BattleBox::reset_box);
    ClassDB::bind_method(D_METHOD("backout"), &BattleBox::backout);
    ClassDB::bind_method(D_METHOD("change_state", "new_state"), &BattleBox::change_state);
    ClassDB::bind_method(D_METHOD("disable"), &BattleBox::disable);
    ClassDB::bind_method(D_METHOD("blitter_flavour"), &BattleBox::blitter_flavour);
    ClassDB::bind_method(D_METHOD("blitter_act"), &BattleBox::blitter_act);
    ClassDB::bind_method(D_METHOD("blitter_item"), &BattleBox::blitter_item);
    ClassDB::bind_method(D_METHOD("blitter_mercy"), &BattleBox::blitter_mercy);
    ClassDB::bind_method(D_METHOD("blitter_print", "texts"), &BattleBox::blitter_print);
    
    ClassDB::bind_method(D_METHOD("change_size", "new_size", "relative"), &BattleBox::change_size, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("change_position", "new_position", "relative"), &BattleBox::change_position, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("advanced_change_size", "relative_to", "new_position", "new_size", "position_relative", "size_relative"), 
        &BattleBox::advanced_change_size, DEFVAL(Vector2()), DEFVAL(Vector2(100, 100)), DEFVAL(false), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("rotate_by", "rot", "relative"), &BattleBox::rotate_by, DEFVAL(false));
    
    ClassDB::bind_method(D_METHOD("clear_webs"), &BattleBox::clear_webs);
    ClassDB::bind_method(D_METHOD("set_webs", "n", "separation", "margin"), &BattleBox::set_webs, DEFVAL(-1), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("get_web_y_pos", "id"), &BattleBox::get_web_y_pos);
    
    ClassDB::bind_method(D_METHOD("tween_size", "args"), &BattleBox::tween_size);
    ClassDB::bind_method(D_METHOD("real_rotate_by", "args"), &BattleBox::real_rotate_by);
    ClassDB::bind_method(D_METHOD("_on_use_button", "choice"), &BattleBox::_on_use_button);
    ClassDB::bind_method(D_METHOD("soul_choice", "action"), &BattleBox::soul_choice);
    ClassDB::bind_method(D_METHOD("refresh_nodes"), &BattleBox::refresh_nodes);
    ClassDB::bind_method(D_METHOD("refresh_options"), &BattleBox::refresh_options);
    ClassDB::bind_method(D_METHOD("set_mercy_options"), &BattleBox::set_mercy_options);
    ClassDB::bind_method(D_METHOD("set_options"), &BattleBox::set_options);
    ClassDB::bind_method(D_METHOD("set_items"), &BattleBox::set_items);
    ClassDB::bind_method(D_METHOD("soul_pos_to_id", "soul_pos", "is", "x_limit"), &BattleBox::soul_pos_to_id, DEFVAL(false), DEFVAL(2));
    ClassDB::bind_method(D_METHOD("id_to_soul_pos", "id", "x_limit"), &BattleBox::id_to_soul_pos, DEFVAL(2));
    
    ClassDB::bind_method(D_METHOD("get_tl"), &BattleBox::get_tl);
    ClassDB::bind_method(D_METHOD("get_br"), &BattleBox::get_br);
    ClassDB::bind_method(D_METHOD("get_blitter_text"), &BattleBox::get_blitter_text);
    ClassDB::bind_method(D_METHOD("_on_soul_move_cooldown"), &BattleBox::_on_soul_move_cooldown);
    ClassDB::bind_method(D_METHOD("box_show"), &BattleBox::box_show);
    ClassDB::bind_method(D_METHOD("box_hide"), &BattleBox::box_hide);

    ClassDB::bind_method(D_METHOD("set_wintext", "value"), &BattleBox::set_wintext);
    ClassDB::bind_method(D_METHOD("get_wintext"), &BattleBox::get_wintext);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "wintext", PROPERTY_HINT_MULTILINE_TEXT), "set_wintext", "get_wintext");
}

void BattleBox::_ready() {
    isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;

    choice_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("Sounds/choice"));
    hp_bar_container = Object::cast_to<MarginContainer>(get_node_internal("Target/HpBars"));
    slider = Object::cast_to<ItemSlider>(get_node_internal("Items/ScrollContainer/Slider"));
    blitter_text = Object::cast_to<Blitter>(get_node_internal("Blitter/Text"));
    main = Object::cast_to<BattleMain>(global->get_scene_container()->get_current_scene());
    rect_container = Object::cast_to<MarginContainer>(get_node_internal("BoxContainer"));
    blitter = Object::cast_to<MarginContainer>(get_node_internal("Blitter"));
    rect = Object::cast_to<NinePatchRect>(get_node_internal("BoxContainer/NinePatchRect"));
    mercy_choices = Object::cast_to<RichTextLabel>(get_node_internal("Mercy/Choices"));
    column1 = Object::cast_to<RichTextLabel>(get_node_internal("Acts/Options/Column1"));
    column2 = Object::cast_to<RichTextLabel>(get_node_internal("Acts/Options/Column2"));
    items_label = Object::cast_to<RichTextLabel>(get_node_internal("Items/TextContainer/Items"));
    
    ResourceLoader* loader = ResourceLoader::get_singleton();
    web_scene = loader->load("res://Battle/Soul/box_web.tscn");
    
    webs = Object::cast_to<Control>(get_node_internal("BoxContainer/NinePatchRect/Webs"));
    rect_no_clip = Object::cast_to<Control>(get_node_internal("BoxContainer/NinePatchRect/RectNoClip"));
    rect_clip = Object::cast_to<Control>(get_node_internal("BoxContainer/NinePatchRect/Bullets"));
    
    collisions.resize(4);
    collisions[0] = Object::cast_to<CollisionShape2D>(get_node_internal("BoxContainer/Collisions/Top"));
    collisions[1] = Object::cast_to<CollisionShape2D>(get_node_internal("BoxContainer/Collisions/Bottom"));
    collisions[2] = Object::cast_to<CollisionShape2D>(get_node_internal("BoxContainer/Collisions/Left"));
    collisions[3] = Object::cast_to<CollisionShape2D>(get_node_internal("BoxContainer/Collisions/Right"));
    
    hp_bars.resize(3);
    hp_bars[0] = Object::cast_to<ProgressBar>(get_node_internal("Target/HpBars/Control/1"));
    hp_bars[1] = Object::cast_to<ProgressBar>(get_node_internal("Target/HpBars/Control/2"));
    hp_bars[2] = Object::cast_to<ProgressBar>(get_node_internal("Target/HpBars/Control/3"));
    
    tl = Object::cast_to<RemoteTransform2D>(get_node_internal("BoxContainer/TL"));
    br = Object::cast_to<RemoteTransform2D>(get_node_internal("BoxContainer/BR"));
    
    screens[BattleState::State_Blittering] = get_node_internal("Blitter");
    screens[BattleState::State_BlitteringCasual] = get_node_internal("Blitter");
    screens[BattleState::State_TargetEnemy] = get_node_internal("Target");
    screens[BattleState::State_Acting] = get_node_internal("Acts");
    screens[BattleState::State_Iteming] = get_node_internal("Items");
    screens[BattleState::State_Mercying] = get_node_internal("Mercy");
    
    current_state_nodes[BattleState::State_BlitteringCasual] = get_node_internal("Behaviours/BlitteringCasual");
    current_state_nodes[BattleState::State_Blittering] = get_node_internal("Behaviours/Blittering");
    current_state_nodes[BattleState::State_TargetEnemy] = get_node_internal("Behaviours/Targetting");
    current_state_nodes[BattleState::State_Acting] = get_node_internal("Behaviours/Acting");
    current_state_nodes[BattleState::State_Iteming] = get_node_internal("Behaviours/Iteming");
    current_state_nodes[BattleState::State_Mercying] = get_node_internal("Behaviours/Mercying");
    current_state_nodes[BattleState::State_Fighting] = get_node_internal("Behaviours/Fighting");
    
    current_state_node = Object::cast_to<BattleBoxBehaviour>(current_state_nodes[BattleState::State_Blittering]);
   
    _physics_process(0.0);
    blitter_text->set_text("");
    anchor_targets[0] = Vector2(rect_container->get_theme_constant("margin_left"), rect_container->get_theme_constant("margin_top"));
    anchor_targets[1] = Vector2(640, 480) - Vector2(rect_container->get_theme_constant("margin_right"), rect_container->get_theme_constant("margin_bottom"));
    def_anchors = anchor_targets.duplicate();
}

void BattleBox::_physics_process(double delta) {
    if(isEditor) return;
    current_size = Vector2(640, 480) - 
                  Vector2(rect_container->get_theme_constant("margin_right"), 
                         rect_container->get_theme_constant("margin_bottom")) - 
                  Vector2(rect_container->get_theme_constant("margin_left"), 
                         rect_container->get_theme_constant("margin_top"));
    
    corner_positions.clear();
    corner_positions.append(Vector2(rect_container->get_theme_constant("margin_left"), 
                                   rect_container->get_theme_constant("margin_top")));
    corner_positions.append(Vector2(640 - rect_container->get_theme_constant("margin_right"), 
                                   480 - rect_container->get_theme_constant("margin_bottom")));
    
    Ref<RectangleShape2D> shape_0 = Object::cast_to<CollisionShape2D>(collisions[0])->get_shape();
    Vector2 vec_0 = shape_0->get_size();
    vec_0.x = current_size.x + colsize;
    vec_0.y = colsize;
    shape_0->set_size(vec_0);

    Ref<RectangleShape2D> shape_2 = Object::cast_to<CollisionShape2D>(collisions[2])->get_shape();
    Vector2 vec_2 = shape_2->get_size();
    vec_2.x = current_size.y + colsize;
    vec_2.y = colsize;
    shape_2->set_size(vec_2);

    Vector2 corner_position_0 = corner_positions[0];
    Vector2 corner_position_1 = corner_positions[1];
    Vector2 center = corner_position_0 + current_size / 2.0;
    
    collisions[0].call("set_position", Vector2(center.x, corner_position_0.y - (colsize / 2.0 - 5.5)));
    collisions[1].call("set_position", Vector2(center.x, corner_position_1.y + (colsize / 2.0 - 5.5)));
    collisions[2].call("set_position", Vector2(corner_position_0.x - (colsize / 2.0 - 5.5), center.y));
    collisions[3].call("set_position", Vector2(corner_position_1.x + (colsize / 2.0 - 5.5), center.y));
    rect_container->set_pivot_offset(center);
    
    tl->set_position(corner_position_0 + Vector2(6, 6));
    br->set_position(corner_position_1 - Vector2(6, 6));
}

void BattleBox::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    int state = action_memory[0];
    if (state == BattleState::State_Disabled) return;
    
    if (action_memory.size() > 1) {
        if(event->is_action_pressed("ui_down")) {
            if (soul_position.y < choices_extends.size() - 1) {
                soul_choice(Vector2i(0, 1));
            }
        }
        if(event->is_action_pressed("ui_left")) {
            if (soul_position.x > 0) {
                soul_choice(Vector2i(-1, 0));
            }
        }
        if(event->is_action_pressed("ui_right")) {
            if (soul_position.x < int(choices_extends[soul_position.y]) - 1) {
                soul_choice(Vector2i(1, 0));
            }
        }
        if(event->is_action_pressed("ui_up")) {
            if (soul_position.y > 0) {
                soul_choice(Vector2i(0, -1));
            }
        }
    }
}

void BattleBox::set_enemies(const Array p_enemies) {
    enemies = p_enemies;
    if(enemies_hp.size() == 0) {
        enemies_hp.resize(enemies.size());
        for(int i=0; i < enemies.size(); i++) {
            Enemy* enemie = Object::cast_to<Enemy>(enemies[i]);
            enemies_hp[i] = enemie->get_stats()["hp"];
        }
    }
    set_targets();
}

void BattleBox::set_targets(bool show_hp_bar) {
    String targets = "";
    RichTextLabel* target_label = Object::cast_to<RichTextLabel>(get_node_internal("Target/Targets"));
    
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if (enemy) {
            Ref<EnemyState> state = enemy->get_enemy_states()[enemy->get_current_state()];
            bool sparable = state->get_sparable(); 
            targets += vformat("[color=%s]* %s[/color]\n", sparable ? "yellow" : "white", enemy->get_enemy_name());
        } else {
            targets += "[color=white][/color]\n";
        }
    }
    
    target_label->set_text(targets);
    
    for (int i = 0; i < 3; i++) {
        ProgressBar* bar = Object::cast_to<ProgressBar>(hp_bars[i]);
        if (i < enemies.size() && enemies[i] && hp_bars[i]) {
            Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
            if (enemy) {
                Dictionary stats = enemy->get_stats();
                bar->set_visible(show_hp_bar);
                bar->set_max(stats["max_hp"]);
                bar->set_value(enemies_hp[i]);
            } else {
                bar->set_visible(false);
            }
        } else if(hp_bars[i]) {
            bar->set_visible(false);
        }
    }
}

void BattleBox::set_mercy_options() {
    String txt = "";
    
    String spare_color = "white";
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* enemy = Object::cast_to<Enemy>(enemies[i]);
        if (enemy) {
            Ref<EnemyState> state = enemy->get_enemy_states()[enemy->get_current_state()];
            bool sparable = state->get_sparable();
            if (sparable) {
                spare_color = "yellow";
                break;
            }
        }
    }
    
    Ref<Encounter> encounter = main->get_encounter();
    PackedStringArray mercy_options = encounter->get_mercy_options();
    
    for (int i = 0; i < mercy_options.size(); i++) {
        txt += vformat("[color=%s]%s[/color]\n", i == 0 ? spare_color : "white", mercy_options[i]);
    }
    
    mercy_choices->set_text(txt);
    
    choices_extends.resize(mercy_options.size());
    choices_extends.fill(1);
}

int BattleBox::soul_pos_to_id(const Vector2& soul_pos, bool is, int x_limit) {
    if (is) { // 아이템 메뉴인 경우
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
    
    for (int i = 0; i < 6; i++) {
        Ref<ActInfo> act = enemy->get_act_info(i);
        if(!act.is_null()) acts.append(act->get_act());
    }
    
    choices_extends = id_to_soul_pos(acts.size());
    
    String acts_p1 = "";
    String acts_p2 = "";
    
    for (int i = 0; i < acts.size(); i++) {
        if(i == 0) acts_p1 = "";
        if(i == 1) acts_p2 = "";
        if (i % 2 == 0) {
            acts_p1 += String(acts[i]) + "\n";
        } else {
            acts_p2 += String(acts[i]) + "\n";
        }
    }
    
    column1->set_text(acts_p1);
    column2->set_text(acts_p2);
}

void BattleBox::set_items() {
    PackedStringArray items;
    
    Array global_items = global->get_items();
    TypedArray<Item> item_list = global->get_item_list();
    
    int start_idx = current_page;
    int end_idx = Math::min<int>(start_idx + items_per_page, global_items.size());
    
    for (int i = start_idx; i < end_idx; i++) {
        Ref<Item> item = item_list[global_items[i]];
        items.push_back(item->get_item_name() + "\n");
    }
    int size = global_items.size() - 1;
    int x = soul_pos_to_id(soul_position, true);
    float y = 8.0f / size;
    slider->set_value(x * y + 1);
    choices_extends.resize(Math::min(items_per_page, end_idx - start_idx));
    choices_extends.fill(1);

    items_label->set_text("* " + String("* ").join(items));
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

void BattleBox::backout() {
    action_memory.resize(action_memory.size() - 1);
    refresh_nodes();
    soul_position = Vector2(0,0);
}

void BattleBox::change_state(int new_state) {
    if(!action_memory.is_empty()) {
        if(new_state == int(action_memory.back())) return;
    }
    
    if(new_state == BattleState::State_Disabled) {
        disable();
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

void BattleBox::disable() {
    Array screens_array = screens.values();
    
    for (int i = 0; i < screens_array.size(); i++) {
        CanvasItem* screen = Object::cast_to<CanvasItem>(screens_array[i]);
        if(screen) screen->hide();
    }
    
    action_memory.resize(1);
    action_memory[0] = (int)BattleState::State_Disabled;
    
    if (button_choice != 0) button_choice = 0;
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
        Ref<ActInfo> act_info = enemy->get_act_info(target_id);
        
        PackedStringArray description = act_info->get_description();
        
        blitter_text->type_text(description);
        blitter_text->connect("finished_all_texts", Callable(this, "emit_signal").bind("act", current_target_id, target_id), CONNECT_ONE_SHOT);
    }
}

void BattleBox::blitter_item() {
    Ref<Item> item = global->get_item_list()[used_item];
    
    PackedStringArray texts;
    
    // 장비 아이템인 경우
    if (item->get_item_type() == Item::WEAPON || item->get_item_type() == Item::ARMOR) {
        texts = global->equip_item(used_item);
    } else {
        // 소비 아이템인 경우
        texts = global->item_use_text(used_item);
    }
    Array arr = global->get_items();
    for(int i=0; i < arr.size(); i++) {
        int item = arr[i];
        if(item == used_item) {
            arr.remove_at(i);
            break;
        }
    }
    global->set_items(arr);
    
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

void BattleBox::soul_choice(const Vector2i& action) {
    if(int(action_memory.back()) == BattleState::State_Blittering || !can_move) return;
    Vector2i new_position = soul_position + action;
    
    if(int(action_memory.back()) == BattleState::State_Iteming) {
        can_move = false;
        emit_signal("move_soul", options_pos_base + options_pos_step * Vector2(0, new_position.y));
        soul_position = new_position;
        
        if(action != Vector2i(0, 0)) choice_sound->play();
        
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.1);
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

void BattleBox::reset_box() {
    if (tw.is_valid()) {
        tw->kill();
    }
    
    float rotation = rect_container->get_rotation();
    if (!Math::is_zero_approx(rotation)) {
        rotation = Math::fmod(rotation, static_cast<float>(Math_PI));
        
        tw = create_tween();
        tw->set_ease(EaseType);
        tw->set_trans(TransType);
        tw->tween_property(rect_container, "rotation", 0.0, 0.6);
        tw->play();
    }
    
    anchor_targets = def_anchors.duplicate();
    
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    tween_size(args);
    
    clear_webs();
}

void BattleBox::clear_webs() {
    TypedArray<Node> children = webs->get_children();
    
    for (int i = 0; i < children.size(); i++) {
        Node* child = Object::cast_to<Node>(children[i]);
        if (child) {
            webs->remove_child(child);
            child->queue_free();
        }
    }
    
    webs_array.clear();
}

void BattleBox::set_webs(int n, float separation, int margin) {
    clear_webs();
    
    if (n < 1) {
        return;
    }
    
    float actual_separation = (separation == -1) ? 
        (current_size.y - 10 - margin * 2) / (n + 1) : separation;
    
    for (int i = 1; i <= n; i++) {
        Node* web_instance = web_scene->instantiate();
        if (web_instance) {
            Line2D* web_line = Object::cast_to<Line2D>(web_instance);
            if (web_line) {
                webs->add_child(web_line);
                web_line->set_position(Vector2(0, actual_separation * i));
                webs_array.append(web_line);
            }
        }
    }
}

float BattleBox::get_web_y_pos(int id) {
    if (webs_array.is_empty()) {
        ERR_PRINT("웹이 비어 있습니다. set_webs()를 사용하여 보라색 영혼이 생길경우 추가해 주세요!");
        return 0;
    }
    
    id = UtilityFunctions::clamp(id, 0, webs_array.size() - 1);
    Node* web = Object::cast_to<Node>(webs_array[id]);
    
    if (web) {
        Vector2 vec = web->call("get_global_position");
        return vec.y;
    }
    
    return 0;
}

Ref<ArgsHolder> BattleBox::change_size(const Vector2& new_size, bool relative) {
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];

    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    Vector2 current_center = anchor_targets_0 + intended_size / 2.0;
    Vector2 final_size = relative ? new_size + intended_size : new_size;
    
    Vector2 min_size = rect_container->get_custom_minimum_size();
    if (final_size.x < min_size.x) final_size.x = min_size.x;
    if (final_size.y < min_size.y) final_size.y = min_size.y;
    
    anchor_targets[0] = current_center - final_size / 2.0;
    anchor_targets[1] = current_center + final_size / 2.0;
    
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    tween_size(args);
    
    return args;
}

Ref<ArgsHolder> BattleBox::change_position(const Vector2& new_position, bool relative) {
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];

    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    Vector2 final_position = relative ? new_position + anchor_targets_0 : new_position;
    
    anchor_targets[0] = final_position;
    anchor_targets[1] = final_position + intended_size;
    
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    tween_size(args);
    
    return args;
}

Ref<ArgsHolder> BattleBox::advanced_change_size(int relative_to, const Vector2& new_position, 
                                               const Vector2& new_size, bool position_relative, 
                                               bool size_relative) {
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];

    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    Vector2 final_size = size_relative ? new_size + intended_size : new_size;
    
    Vector2 min_size = rect_container->get_custom_minimum_size();
    if (final_size.x < min_size.x) final_size.x = min_size.x;
    if (final_size.y < min_size.y) final_size.y = min_size.y;
    
    Vector2 final_position;
    
    switch (relative_to) {
        case RELATIVE_TOP_LEFT:
            final_position = position_relative ? new_position + anchor_targets[0] : new_position;
            anchor_targets[0] = final_position;
            break;
        case RELATIVE_TOP_RIGHT:
            final_position = position_relative ? new_position + anchor_targets[0] + intended_size.x * Vector2(1, 0) : new_position;
            anchor_targets[0] = final_position - final_size.x * Vector2(1, 0);
            break;
        case RELATIVE_BOTTOM_LEFT:
            final_position = position_relative ? new_position + anchor_targets[0] + intended_size.y * Vector2(0, 1) : new_position;
            anchor_targets[0] = final_position - final_size.y * Vector2(0, 1);
            break;
        case RELATIVE_BOTTOM_RIGHT:
            final_position = position_relative ? new_position + anchor_targets[1] : new_position;
            anchor_targets[0] = final_position - final_size;
            break;
        case RELATIVE_CENTER:
            final_position = position_relative ? new_position + anchor_targets[0] + intended_size / 2.0 : new_position;
            anchor_targets[0] = final_position - final_size / 2.0;
            break;
    }
    anchor_targets_0 = anchor_targets[0];
    
    anchor_targets[1] = anchor_targets_0 + final_size;
    
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    tween_size(args);
    
    return args;
}

Ref<ArgsHolder> BattleBox::rotate_by(float rot, bool relative) {
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->args.append(rot);
    args->args.append(relative);
    
    call_deferred("real_rotate_by", args);
    return args;
}

void BattleBox::blitter_print(PackedStringArray texts) {
    global->set_battle_text_box(true);
    change_state(BattleState::State_Blittering);
    blitter_text->type_text(texts);
    blitter_text->connect("finished_all_texts", Callable(global, "set_battle_text_box").bind(false), CONNECT_ONE_SHOT);
}

Vector2 BattleBox::get_size() {
    return rect->get_size();
}

void BattleBox::box_show() {
    if(box_texture.is_null()) return;
    get_node_internal("BoxContainer/Panel")->call("show");
    rect->set_texture(box_texture);
}

void BattleBox::box_hide() {
    box_texture = rect->get_texture()->duplicate();
    get_node_internal("BoxContainer/Panel")->call("hide");
    rect->set_texture(Ref<Texture>());
}

void BattleBox::real_rotate_by(Ref<ArgsHolder> args) {
    float target_rotation = args->args[0];
    bool is_relative = args->args[1];
    
    tw = create_tween();
    tw->set_ease(EaseType);
    tw->set_trans(TransType);
    
    if (is_relative) {
        float current_rotation = rect_container->get_rotation();
        tw->tween_property(rect_container, "rotation", current_rotation + target_rotation, args->get_duration());
    } else {
        tw->tween_property(rect_container, "rotation", target_rotation, args->get_duration());
    }
    
    tw->play();
}

void BattleBox::tween_size(Ref<ArgsHolder> args) {
    tw = create_tween();
    tw->set_parallel(true);
    tw->set_ease(EaseType);
    tw->set_trans(TransType);
    
    float margin_left = rect_container->get_theme_constant("margin_left");
    float margin_top = rect_container->get_theme_constant("margin_top");
    float margin_right = rect_container->get_theme_constant("margin_right");
    float margin_bottom = rect_container->get_theme_constant("margin_bottom");
   
    Vector2 current_corner0 = Vector2(margin_left, margin_top);
    Vector2 current_corner1 = Vector2(640 - margin_right, 480 - margin_bottom);

    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];
    
    tw->tween_property(rect_container, "theme_override_constants/margin_left", 
                      margin_left + (anchor_targets_0.x - current_corner0.x), args->get_duration());
                      
    tw->tween_property(rect_container, "theme_override_constants/margin_top", 
                      margin_top + (anchor_targets_0.y - current_corner0.y), args->get_duration());
                      
    tw->tween_property(rect_container, "theme_override_constants/margin_right", 
                      margin_right + (current_corner1.x - anchor_targets_1.x), args->get_duration());
                      
    tw->tween_property(rect_container, "theme_override_constants/margin_bottom", 
                      margin_bottom + (current_corner1.y - anchor_targets_1.y), args->get_duration());
    
    tw->play();
    tw->connect("finished", Callable(this, "emit_signal").bind("tweening_finished"));
}

RemoteTransform2D* BattleBox::get_tl() const {
    return tl;
}

RemoteTransform2D* BattleBox::get_br() const {
    return br;
}

Blitter* BattleBox::get_blitter_text() const {
    return blitter_text;
}

void BattleBox::set_wintext(String value) {
    win_text = value;
}

String BattleBox::get_wintext() const {
    return win_text;
}

void BattleBox::set_used_item(int value) {
    used_item = value;
}

int BattleBox::get_used_item() {
    return used_item;
}