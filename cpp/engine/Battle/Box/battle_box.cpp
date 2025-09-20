#include "battle_box.h"
#include "env.h"
#include "box_behaviour.h"
#include "engine/Battle/battle_system.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/method_tweener.hpp>
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
#include<godot_cpp/classes/geometry2d.hpp>
#define reset_morphSpeed 800

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
    isPolygonMode = false;
    morph_speed = 200.0f;
    backup_color = Color();
    polygon_point_count = 120;
    tweening_vertex_index = -1;
    is_point_tweening = false;
    isPolygonRest = false;
    
    win_text = String("* ") + tr("UT_VICTORY") + String("\n * ") + tr("UT_GET_EXP_GOLD");
    
    TransType = Tween::TRANS_QUAD;
    EaseType = Tween::EASE_OUT;
    
    mercy_texts.push_back(String("* ")+tr("UT_MERCY"));
    mercy_texts.push_back(String("* ")+tr("UT_ESCAPED"));
    mercy_texts.push_back(String("* ")+tr("UT_CANNOT_ESCAPE"));
    
    anchor_targets.resize(2);
    anchor_targets[0] = Vector2(220, 140);
    anchor_targets[1] = Vector2(420, 340);
    
    action_memory.append(int(BattleState::State_Disabled));
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
    ADD_SIGNAL(MethodInfo("tween_finished"));

    ClassDB::bind_method(D_METHOD("_set_targets", "show_hp_bar"), &BattleBox::_set_targets, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("_backout"), &BattleBox::_backout);
    ClassDB::bind_method(D_METHOD("_disable"), &BattleBox::_disable);
    ClassDB::bind_method(D_METHOD("_real_rotate_by", "args"), &BattleBox::_real_rotate_by);
    ClassDB::bind_method(D_METHOD("_on_use_button", "choice"), &BattleBox::_on_use_button);
    ClassDB::bind_method(D_METHOD("_soul_choice", "action"), &BattleBox::_soul_choice);
    ClassDB::bind_method(D_METHOD("_set_items"), &BattleBox::_set_items);
    ClassDB::bind_method(D_METHOD("_on_soul_move_cooldown"), &BattleBox::_on_soul_move_cooldown);
    ClassDB::bind_method(D_METHOD("_on_point_tween_step", "new_position"), &BattleBox::_on_point_tween_step);
    ClassDB::bind_method(D_METHOD("_on_point_tween_finished"), &BattleBox::_on_point_tween_finished);
    ClassDB::bind_method(D_METHOD("_reset_finished"), &BattleBox::_reset_finished);

    // 상자 크기
    ClassDB::bind_method(D_METHOD("get_size"), &BattleBox::get_size);
    // 상자 왼쪽위 모서리 위치
    ClassDB::bind_method(D_METHOD("get_tl_anchor"), &BattleBox::get_tl_anchor);
    // 상자 오른쪽아래 모서리 위치
    ClassDB::bind_method(D_METHOD("get_br_anchor"), &BattleBox::get_br_anchor);
    ClassDB::bind_method(D_METHOD("reset_box", "duration"), &BattleBox::reset_box, DEFVAL(0.5f));
    ClassDB::bind_method(D_METHOD("change_size", "new_size", "relative", "duration"), &BattleBox::change_size, DEFVAL(false), DEFVAL(0.6f));
    ClassDB::bind_method(D_METHOD("change_position", "new_position", "relative", "duration"), &BattleBox::change_position, DEFVAL(false), DEFVAL(0.6f));
    ClassDB::bind_method(D_METHOD("advanced_change_size", "relative_to", "new_position", "new_size", "position_relative", "size_relative", "duration"), &BattleBox::advanced_change_size, 
        DEFVAL(Vector2()), DEFVAL(Vector2(100, 100)), DEFVAL(false), DEFVAL(false), DEFVAL(0.6f));
    ClassDB::bind_method(D_METHOD("rotate_by", "rot", "relative", "duration"), &BattleBox::rotate_by, DEFVAL(false), DEFVAL(0.6f));
    ClassDB::bind_method(D_METHOD("box_show"), &BattleBox::box_show);
    ClassDB::bind_method(D_METHOD("box_hide"), &BattleBox::box_hide);
    ClassDB::bind_method(D_METHOD("clear_webs"), &BattleBox::clear_webs);
    ClassDB::bind_method(D_METHOD("set_webs", "n", "separation", "margin"), &BattleBox::set_webs, DEFVAL(-1), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("get_web_y_pos", "id"), &BattleBox::get_web_y_pos);
    ClassDB::bind_method(D_METHOD("blitter_print", "texts"), &BattleBox::blitter_print);
    ClassDB::bind_method(D_METHOD("polygon_enable"), &BattleBox::polygon_enable);
    ClassDB::bind_method(D_METHOD("get_polygon_points"), &BattleBox::get_polygon_points);
    ClassDB::bind_method(D_METHOD("get_vertex_position", "vertex_index"), &BattleBox::get_vertex_position);
    ClassDB::bind_method(D_METHOD("move_closest_point", "target_point", "duration"), &BattleBox::move_closest_point, DEFVAL(0.3f));
    ClassDB::bind_method(D_METHOD("move_point_by_index", "vertex_index", "target_point", "duration"), &BattleBox::move_point_by_index, DEFVAL(0.3f));
    ClassDB::bind_method(D_METHOD("move_point_by_offset", "from_point", "offset", "duration"), &BattleBox::move_point_by_offset, DEFVAL(0.3f));


    ClassDB::bind_method(D_METHOD("set_wintext", "value"), &BattleBox::set_wintext);
    ClassDB::bind_method(D_METHOD("get_wintext"), &BattleBox::get_wintext);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "wintext", PROPERTY_HINT_MULTILINE_TEXT), "set_wintext", "get_wintext");

    ClassDB::bind_method(D_METHOD("set_mercy_texts", "value"), &BattleBox::set_mercy_texts);
    ClassDB::bind_method(D_METHOD("get_mercy_texts"), &BattleBox::get_mercy_texts);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "mercy_texts", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_mercy_texts", "get_mercy_texts");

    ClassDB::bind_method(D_METHOD("set_morph_speed", "value"), &BattleBox::set_morph_speed);
    ClassDB::bind_method(D_METHOD("get_morph_speed"), &BattleBox::get_morph_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "morph_speed", PROPERTY_HINT_RANGE, "0.1,500,0.1"), "set_morph_speed", "get_morph_speed");

    ClassDB::bind_method(D_METHOD("set_polygon_point_count", "value"), &BattleBox::set_polygon_point_count);
    ClassDB::bind_method(D_METHOD("get_polygon_point_count"), &BattleBox::get_polygon_point_count);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "polygon_point_count", PROPERTY_HINT_RANGE, "100,300,1"), "set_polygon_point_count", "get_polygon_point_count");
}

void BattleBox::_ready() {
    if(isEditor) return;

    choice_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("Sounds/choice"));
    hp_bar_container = Object::cast_to<MarginContainer>(get_node_internal("Target/HpBars"));
    slider = Object::cast_to<ItemSlider>(get_node_internal("Items/ScrollContainer/Slider"));
    blitter_text = Object::cast_to<Blitter>(get_node_internal("Blitter/Text"));
    main = Object::cast_to<BattleMain>(global->get_scene_container()->get_current_scene());
    rect_container = Object::cast_to<MarginContainer>(get_node_internal("BoxContainer"));
    blitter = Object::cast_to<MarginContainer>(get_node_internal("Blitter"));
    rect = Object::cast_to<ColorRect>(get_node_internal("BoxContainer/Rect"));
    border = Object::cast_to<Line2D>(get_node_internal("BoxContainer/Rect/border"));
    background = Object::cast_to<Polygon2D>(get_node_internal("BoxContainer/Rect/background"));
    mercy_choices = Object::cast_to<RichTextLabel>(get_node_internal("Mercy/Choices"));
    column1 = Object::cast_to<RichTextLabel>(get_node_internal("Acts/Options/Column1"));
    column2 = Object::cast_to<RichTextLabel>(get_node_internal("Acts/Options/Column2"));
    items_label = Object::cast_to<RichTextLabel>(get_node_internal("Items/TextContainer/Items"));
    
    ResourceLoader* loader = ResourceLoader::get_singleton();
    web_scene = loader->load("res://Battle/Soul/box_web.tscn");
    
    webs = Object::cast_to<Control>(get_node_internal("BoxContainer/Rect/Webs"));
    rect_no_clip = Object::cast_to<Control>(get_node_internal("BoxContainer/Rect/RectNoClip"));
    rect_clip = Object::cast_to<Control>(get_node_internal("BoxContainer/Rect/Bullets"));
    
    collisions.resize(4);
    collisions[0] = Object::cast_to<CollisionShape2D>(get_node_internal("BoxContainer/Collisions/Top"));
    collisions[1] = Object::cast_to<CollisionShape2D>(get_node_internal("BoxContainer/Collisions/Bottom"));
    collisions[2] = Object::cast_to<CollisionShape2D>(get_node_internal("BoxContainer/Collisions/Left"));
    collisions[3] = Object::cast_to<CollisionShape2D>(get_node_internal("BoxContainer/Collisions/Right"));
    polygon = Object::cast_to<CollisionPolygon2D>(get_node_internal("BoxContainer/Collisions/Polygon"));
    
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
    
    collisions[0].call("set_position", Vector2(center.x, corner_position_0.y - (colsize / 2.0 - 2.5)));
    collisions[1].call("set_position", Vector2(center.x, corner_position_1.y + (colsize / 2.0 - 2.5)));
    collisions[2].call("set_position", Vector2(corner_position_0.x - (colsize / 2.0 - 2.5), center.y));
    collisions[3].call("set_position", Vector2(corner_position_1.x + (colsize / 2.0 - 2.5), center.y));
    rect_container->set_pivot_offset(center);
    
    tl->set_position(corner_position_0 + Vector2(6, 6));
    br->set_position(corner_position_1 - Vector2(6, 6));

    PackedVector2Array points;
    if(isPolygonMode) {
        float margin_left = rect_container->get_theme_constant("margin_left");
        float margin_top = rect_container->get_theme_constant("margin_top");
        Vector2 margin_offset = Vector2(margin_left, margin_top);

        points = polygon->get_polygon();
        for(int i = 0; i < points.size(); i++) {
            points[i] = points[i] - margin_offset;
        }
    }else {
        points.push_back(Vector2(0, 0));
        points.push_back(Vector2(current_size.x, 0));
        points.push_back(current_size);
        points.push_back(Vector2(0, current_size.y));
    }
    Array off = Geometry2D::get_singleton()->offset_polygon(points, isPolygonMode ? 2.5 : 0);
    PackedVector2Array border_pts = off.size() > 0 ? (PackedVector2Array)off[0] : points;
    border->set_points(border_pts);
    background->set_polygon(border_pts);
}

void BattleBox::_process(double delta) {
    if(!isPolygonMode || isEditor) return;

    if(static_shape.size() == target_shape.size()) {
        bool all_reached = true;
        
        for(int i=0; i < target_shape.size(); i++) {
            Vector2 current = static_shape[i];
            Vector2 target = target_shape[i];
            float speed = isPolygonRest && morph_speed < reset_morphSpeed ? reset_morphSpeed : morph_speed;
            Vector2 new_pos = current.move_toward(target, speed * delta);
            static_shape.set(i, new_pos);

            if (current.distance_to(target) > 1.0f) {
                all_reached = false;
            }
        }
        polygon->set_polygon(static_shape);

        if (all_reached && isPolygonRest) {
            _polygon_reset_finished();
        }
    }
}

void BattleBox::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    int state = action_memory[0];
    if (state == BattleState::State_Disabled) return;
    
    if (action_memory.size() > 1) {
        if(event->is_action_pressed("ui_down")) {
            if (soul_position.y < choices_extends.size() - 1) {
                _soul_choice(Vector2i(0, 1));
            }
        }
        if(event->is_action_pressed("ui_left")) {
            if (soul_position.x > 0) {
                _soul_choice(Vector2i(-1, 0));
            }
        }
        if(event->is_action_pressed("ui_right")) {
            if (soul_position.x < int(choices_extends[soul_position.y]) - 1) {
                _soul_choice(Vector2i(1, 0));
            }
        }
        if(event->is_action_pressed("ui_up")) {
            if (soul_position.y > 0) {
                _soul_choice(Vector2i(0, -1));
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
    _set_targets();
}

void BattleBox::_set_targets(bool show_hp_bar) {
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
        Ref<ActInfo> act = enemy->_get_act_info(i);
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

void BattleBox::_set_items() {
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
        Ref<ActInfo> act_info = enemy->_get_act_info(target_id);
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

void BattleBox::_soul_choice(const Vector2i& action) {
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

int BattleBox::find_closest_edge_to_point(PackedVector2Array& poly, Vector2 point) {
    int best_edge = 0;
    float best_distance = Math_INF;
    for(int i=0; i < poly.size(); i++) {
        Vector2 a = poly[i];
        Vector2 b = poly[(i + 1) % poly.size()];
        Vector2 closest = Geometry2D::get_singleton()->get_closest_point_to_segment(point, a, b);
        float distance = (point - closest).length_squared();
        
        if (distance < best_distance) {
            best_distance = distance;
            best_edge = i;
        }
    }
    
    return best_edge;
}

int BattleBox::find_closest_vertex(const PackedVector2Array& poly, const Vector2& point) {
    if (poly.size() == 0) return -1;
    
    int closest_index = 0;
    float closest_distance = Math_INF;
    
    for (int i = 0; i < poly.size(); i++) {
        float distance = (poly[i] - point).length_squared();
        if (distance < closest_distance) {
            closest_distance = distance;
            closest_index = i;
        }
    }
    
    return closest_index;
}

bool BattleBox::is_polygon_valid(const PackedVector2Array& poly) {
    if (poly.size() < 3) return false;
    
    for(int i = 0; i < poly.size(); i++) {
        Vector2 a1 = poly[i];
        Vector2 a2 = poly[(i + 1) % poly.size()];
        
        for (int j = i + 2; j < poly.size(); j++) {
            if(j == poly.size() - 1 && i == 0) continue;
            
            Vector2 b1 = poly[j];
            Vector2 b2 = poly[(j + 1) % poly.size()];
            Variant intersection = Geometry2D::get_singleton()->segment_intersects_segment(a1, a2, b1, b2);
            
            if(intersection.get_type() == Variant::VECTOR2) return false;
        }
    }
    return true;
}

void BattleBox::_on_point_tween_step(Vector2 new_position) {
    if (tweening_vertex_index >= 0 && tweening_vertex_index < target_shape.size()) {
        target_shape.set(tweening_vertex_index, new_position);
    }
}

void BattleBox::_on_point_tween_finished() {
    is_point_tweening = false;
    tweening_vertex_index = -1;
}

void BattleBox::_reset_finished() {
    if(isPolygonMode) {
        isPolygonRest = true;
        is_point_tweening = false;
        tweening_vertex_index = -1;
        if(point_tween.is_valid()) point_tween->kill();

        Vector2 margin_offset = Vector2(rect_container->get_theme_constant("margin_left"), rect_container->get_theme_constant("margin_top"));
        PackedVector2Array rect_poly;
        
        float perimeter = (current_size.x + current_size.y) * 2.0f;
        float segment_length = perimeter / float(static_shape.size());
        for(int i = 0; i < static_shape.size(); i++) {
            float distance = segment_length * float(i);
            Vector2 point;

            if(distance < current_size.x) {
                point = margin_offset + Vector2(distance, 0);
            } else if (distance < current_size.x + current_size.y) {
                point = margin_offset + Vector2(current_size.x, distance - current_size.x);
            } else if (distance < current_size.x * 2.0f + current_size.y) {
                point = margin_offset + Vector2(current_size.x - (distance - current_size.x - current_size.y), current_size.y);
            } else {
                point = margin_offset + Vector2(0, current_size.y - (distance - current_size.x * 2.0f - current_size.y));
            }

            rect_poly.push_back(point);
        }
        target_shape = rect_poly;
    }
}

void BattleBox::_polygon_reset_finished() {
    isPolygonMode = false;
    isPolygonRest = false;
    polygon->set_disabled(true);
    for(int i=0; i < 4; i++) collisions[i].set("disabled", false);
    static_shape.clear();
    target_shape.clear();
}

void BattleBox::reset_box(float duration) {
    if(tw.is_valid()) tw->kill();
    
    float rotation = rect_container->get_rotation();
    if (!Math::is_zero_approx(rotation)) {
        rotation = Math::fmod(rotation, float(Math_PI));
        
        tw = create_tween();
        tw->set_ease(EaseType);
        tw->set_trans(TransType);
        tw->tween_property(rect_container, "rotation", 0, duration);
        tw->play();
    }
    anchor_targets = def_anchors.duplicate();
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->set_duration(duration);
    clear_webs();
    tween_size(args);
    tw->connect("finished", Callable(this, "_reset_finished"));
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

void BattleBox::change_size(const Vector2& new_size, bool relative, float duration) {
    if(isPolygonMode) {
        ERR_PRINT("다각형 모드에서는 사용할 수 없습니다. reset_box()를 후출하고 사용해주세요");
        return;
    }
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
    args->set_duration(duration);
    tween_size(args);
}

void BattleBox::change_position(const Vector2& new_position, bool relative, float duration) {
    if(isPolygonMode) {
        ERR_PRINT("다각형 모드에서는 사용할 수 없습니다. reset_box()를 후출하고 사용해주세요");
        return;
    }
    Vector2 anchor_targets_0 = anchor_targets[0];
    Vector2 anchor_targets_1 = anchor_targets[1];

    Vector2 intended_size = anchor_targets_1 - anchor_targets_0;
    Vector2 final_position = relative ? new_position + anchor_targets_0 : new_position;
    
    anchor_targets[0] = final_position;
    anchor_targets[1] = final_position + intended_size;
    
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->set_duration(duration);
    tween_size(args);
}

void BattleBox::advanced_change_size(RelativePosition relative_to, const Vector2& new_position, 
                                               const Vector2& new_size, bool position_relative, 
                                               bool size_relative, float duration) {
    if(isPolygonMode) {
        ERR_PRINT("다각형 모드에서는 사용할 수 없습니다. reset_box()를 후출하고 사용해주세요");
        return;
    }
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
    args->set_duration(duration);
    tween_size(args);
}

void BattleBox::rotate_by(float rot, bool relative, float duration) {
    Ref<ArgsHolder> args = memnew(ArgsHolder);
    args->set_duration(duration);
    args->args.append(rot);
    args->args.append(relative);
    
    call_deferred("_real_rotate_by", args);
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

Vector2 BattleBox::get_tl_anchor() {
    return anchor_targets[0];
}

Vector2 BattleBox::get_br_anchor() {
    return anchor_targets[1];
}

void BattleBox::polygon_enable() {
    isPolygonMode = true;
    
    Vector2 margin_offset = Vector2(rect_container->get_theme_constant("margin_left"), rect_container->get_theme_constant("margin_top"));
    PackedVector2Array rect_poly;
    
    float perimeter = (current_size.x + current_size.y) * 2.0f;
    float segment_length = perimeter / float(polygon_point_count);
    for(int i = 0; i < polygon_point_count; i++) {
        float distance = segment_length * float(i);
        Vector2 point;
        
        if(distance < current_size.x) {
            point = margin_offset + Vector2(distance, 0);
        } else if (distance < current_size.x + current_size.y) {
            point = margin_offset + Vector2(current_size.x, distance - current_size.x);
        } else if (distance < current_size.x * 2.0f + current_size.y) {
            point = margin_offset + Vector2(current_size.x - (distance - current_size.x - current_size.y), current_size.y);
        } else {
            point = margin_offset + Vector2(0, current_size.y - (distance - current_size.x * 2.0f - current_size.y));
        }
        
        rect_poly.push_back(point);
    }
    polygon->set_polygon(rect_poly);
    target_shape = rect_poly.duplicate();
    static_shape = rect_poly.duplicate();

    polygon->set_disabled(false);
    for (int i = 0; i < 4; i++) {
        collisions[i].set("disabled", true);
    }
}

void BattleBox::move_closest_point(Vector2 target_point, float duration) {
    if (!isPolygonMode || target_shape.size() < 3 || is_point_tweening) return;
    
    Vector2 local_target = to_local(target_point);
    int closest_vertex = find_closest_vertex(target_shape, local_target);
    
    if (closest_vertex >= 0 && closest_vertex < target_shape.size()) {
        move_point_by_index(closest_vertex, target_point, duration);
    }
}

void BattleBox::move_point_by_index(int vertex_index, Vector2 target_point, float duration) {
   if (!isPolygonMode || vertex_index < 0 || vertex_index >= target_shape.size() || is_point_tweening) return;
    Vector2 local_target = to_local(target_point);
    Vector2 constrained_point = local_target;
    Vector2 old_position = target_shape[vertex_index];

    target_shape.set(vertex_index, constrained_point);
    if(!is_polygon_valid(target_shape)) {
        target_shape.set(vertex_index, old_position);
        ERR_PRINT("유효하지 않은 다각형 모양입니다. 점을 이동할 수 없습니다.");
        return;
    }
    
    target_shape.set(vertex_index, old_position);
    
    tweening_vertex_index = vertex_index;
    is_point_tweening = true;
    if(point_tween.is_valid()) point_tween->kill();
    
    Vector2 current_position = target_shape[vertex_index];
    point_tween = create_tween()->set_ease(Tween::EASE_OUT)->set_trans(Tween::TRANS_QUAD);
    point_tween->tween_method(Callable(this, "_on_point_tween_step"), current_position, constrained_point, duration);
    point_tween->connect("finished", Callable(this, "_on_point_tween_finished"), CONNECT_ONE_SHOT);
    point_tween->play();
}

void BattleBox::move_point_by_offset(Vector2 from_point, Vector2 offset, float duration) {
    if (!isPolygonMode || target_shape.size() < 3 || is_point_tweening) return;
    
    Vector2 local_from = to_local(from_point);
    int closest_vertex = find_closest_vertex(target_shape, local_from);
    
    if (closest_vertex >= 0 && closest_vertex < target_shape.size()) {
        Vector2 current_pos = target_shape[closest_vertex];
        Vector2 target_pos = to_global(current_pos + offset);
        move_point_by_index(closest_vertex, target_pos, duration);
    }
}

PackedVector2Array BattleBox::get_polygon_points() {
    return target_shape.duplicate();
}

Vector2 BattleBox::get_vertex_position(int vertex_index) {
    if (vertex_index < 0 || vertex_index >= target_shape.size()) {
        return Vector2();
    }
    return target_shape[vertex_index];
}

void BattleBox::box_show() {
    if(backup_color == Color()) backup_color = background->get_color();
    get_node_internal("BoxContainer/Panel")->call("show");
    border->show();
    background->set_color(backup_color);
}

void BattleBox::box_hide() {
    if(backup_color == Color()) backup_color = background->get_color();
    get_node_internal("BoxContainer/Panel")->call("hide");
    border->hide();
    background->set_color(Color(0, 0, 0, 0));
}

void BattleBox::_real_rotate_by(Ref<ArgsHolder> args) {
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
    tw = create_tween()->set_parallel(true);
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
    tw->connect("finished", Callable(this, "emit_signal").bind("tween_finished"));
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

void BattleBox::set_mercy_texts(PackedStringArray value) {
    mercy_texts = value;
}

PackedStringArray BattleBox::get_mercy_texts() {
    return mercy_texts;
}

void BattleBox::set_morph_speed(float value) {
    morph_speed = value;
}

float BattleBox::get_morph_speed() const {
    return morph_speed;
}

void BattleBox::set_polygon_point_count(int value) {
    polygon_point_count = value;
}

int BattleBox::get_polygon_point_count() const {
    return polygon_point_count;
}