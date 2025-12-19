#include "battle_box.h"
#include "env.h"
#include "box_behaviour.h"
#include "engine/Battle/battle_system.h"
#include<godot_cpp/classes/geometry2d.hpp>
#include<godot_cpp/classes/rectangle_shape2d.hpp>
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
    polygon_point_count = 120;
    isPolygonRest = false;
    
    win_text = "UT_VICTORY";
    
    TransType = Tween::TRANS_QUAD;
    EaseType = Tween::EASE_OUT;
    
    mercy_texts.push_back("UT_MERCY");
    mercy_texts.push_back("UT_ESCAPED");
    mercy_texts.push_back("UT_CANNOT_ESCAPE");
    
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
    ADD_SIGNAL(MethodInfo("blitter_end"));

    ClassDB::bind_method(D_METHOD("_set_targets", "show_hp_bar"), &BattleBox::_set_targets, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("_backout"), &BattleBox::_backout);
    ClassDB::bind_method(D_METHOD("_disable"), &BattleBox::_disable);
    ClassDB::bind_method(D_METHOD("_real_rotate_by", "args"), &BattleBox::_real_rotate_by);
    ClassDB::bind_method(D_METHOD("_on_use_button", "choice"), &BattleBox::_on_use_button);
    ClassDB::bind_method(D_METHOD("_soul_choice", "action"), &BattleBox::_soul_choice);
    ClassDB::bind_method(D_METHOD("_set_items"), &BattleBox::_set_items);
    ClassDB::bind_method(D_METHOD("_on_soul_move_cooldown"), &BattleBox::_on_soul_move_cooldown);
    ClassDB::bind_method(D_METHOD("_on_point_tween_step", "new_position", "vertex_index"), &BattleBox::_on_point_tween_step);
    ClassDB::bind_method(D_METHOD("_on_point_tween_finished", "vertex_index"), &BattleBox::_on_point_tween_finished);
    ClassDB::bind_method(D_METHOD("_reset_finished"), &BattleBox::_reset_finished);

    // 상자 크기
    ClassDB::bind_method(D_METHOD("get_size"), &BattleBox::get_size);
    // 상자 위치
    ClassDB::bind_method(D_METHOD("get_box_position", "relative_to"), &BattleBox::get_box_position, DEFVAL(RELATIVE_TOP_LEFT));
    // 상자 회전 각도
    ClassDB::bind_method(D_METHOD("get_box_rotation"), &BattleBox::get_box_rotation);
    ClassDB::bind_method(D_METHOD("reset_box", "duration"), &BattleBox::reset_box, DEFVAL(0.5f));
    ClassDB::bind_method(D_METHOD("change_size", "new_size", "relative", "duration"), &BattleBox::change_size, DEFVAL(false), DEFVAL(0.6f));
    ClassDB::bind_method(D_METHOD("change_position", "new_position", "relative", "duration"), &BattleBox::change_position, DEFVAL(false), DEFVAL(0.6f));
    ClassDB::bind_method(D_METHOD("advanced_change_size", "relative_to", "new_position", "new_size", "position_relative", "size_relative", "duration"), &BattleBox::advanced_change_size
    , DEFVAL(false), DEFVAL(false), DEFVAL(0.6f));
    ClassDB::bind_method(D_METHOD("rotate_by", "rot", "relative", "duration"), &BattleBox::rotate_by, DEFVAL(false), DEFVAL(0.6f));
    ClassDB::bind_method(D_METHOD("set_box_position", "new_position", "relative_to", "relative"), &BattleBox::set_box_position, DEFVAL(RELATIVE_TOP_LEFT), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("set_box_size", "new_size", "relative_to", "relative"), &BattleBox::set_box_size, DEFVAL(RELATIVE_CENTER), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("set_box_rotation", "rot", "relative"), &BattleBox::set_box_rotation, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("box_show"), &BattleBox::box_show);
    ClassDB::bind_method(D_METHOD("box_hide"), &BattleBox::box_hide);
    ClassDB::bind_method(D_METHOD("clear_webs"), &BattleBox::clear_webs);
    ClassDB::bind_method(D_METHOD("set_webs", "n", "separation", "margin"), &BattleBox::set_webs, DEFVAL(-1), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("get_web_y_pos", "id"), &BattleBox::get_web_y_pos);
    ClassDB::bind_method(D_METHOD("blitter_print", "texts"), &BattleBox::blitter_print);
    ClassDB::bind_method(D_METHOD("polygon_enable"), &BattleBox::polygon_enable);
    ClassDB::bind_method(D_METHOD("polygon_is_enabled"), &BattleBox::polygon_is_enabled);
    ClassDB::bind_method(D_METHOD("create_protrusion", "direction", "offset", "size", "duration"), &BattleBox::create_protrusion, DEFVAL(0.3f));
    ClassDB::bind_method(D_METHOD("get_polygon_points"), &BattleBox::get_polygon_points);
    ClassDB::bind_method(D_METHOD("get_vertex_position", "vertex_index"), &BattleBox::get_vertex_position);
    ClassDB::bind_method(D_METHOD("move_closest_point", "target_point", "duration"), &BattleBox::move_closest_point, DEFVAL(0.3f));
    ClassDB::bind_method(D_METHOD("move_point_by_index", "vertex_index", "target_point", "duration"), &BattleBox::move_point_by_index, DEFVAL(0.3f));
    ClassDB::bind_method(D_METHOD("move_point_by_offset", "from_point", "offset", "duration"), &BattleBox::move_point_by_offset, DEFVAL(0.3f));
    ClassDB::bind_method(D_METHOD("move_multiple_points", "vertex_indices", "target_points", "duration"), &BattleBox::move_multiple_points, DEFVAL(0.3f));
    ClassDB::bind_method(D_METHOD("stop_all_point_tweens"), &BattleBox::stop_all_point_tweens);

    ClassDB::bind_method(D_METHOD("set_wintext", "value"), &BattleBox::set_wintext);
    ClassDB::bind_method(D_METHOD("get_wintext"), &BattleBox::get_wintext);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "wintext", PROPERTY_HINT_MULTILINE_TEXT), "set_wintext", "get_wintext");

    ClassDB::bind_method(D_METHOD("set_mercy_texts", "value"), &BattleBox::set_mercy_texts);
    ClassDB::bind_method(D_METHOD("get_mercy_texts"), &BattleBox::get_mercy_texts);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "mercy_texts", PROPERTY_HINT_TYPE_STRING,
        String::num(Variant::STRING) + "/" + String::num(PROPERTY_HINT_MULTILINE_TEXT) + ":"),
    "set_mercy_texts", "get_mercy_texts");

    ClassDB::bind_method(D_METHOD("set_ease_type", "type"), &BattleBox::set_ease_type);
    ClassDB::bind_method(D_METHOD("get_ease_type"), &BattleBox::get_ease_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenEase", PROPERTY_HINT_ENUM, "In,Out,InOut,OutIn"), "set_ease_type", "get_ease_type");

    ClassDB::bind_method(D_METHOD("set_transition_type", "type"), &BattleBox::set_transition_type);
    ClassDB::bind_method(D_METHOD("get_transition_type"), &BattleBox::get_transition_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "TweenTrans", PROPERTY_HINT_ENUM, "Linear,Sine,Quint,Quart,Quad,Expo,Elastic,Cubic,Circ,Bounce,Back,Spring"), "set_transition_type", "get_transition_type");

    ADD_GROUP("Polygon Box Properties", "");
    ClassDB::bind_method(D_METHOD("set_morph_speed", "value"), &BattleBox::set_morph_speed);
    ClassDB::bind_method(D_METHOD("get_morph_speed"), &BattleBox::get_morph_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "morph_speed", PROPERTY_HINT_RANGE, "0.1,500,0.1"), "set_morph_speed", "get_morph_speed");

    ClassDB::bind_method(D_METHOD("set_polygon_point_count", "value"), &BattleBox::set_polygon_point_count);
    ClassDB::bind_method(D_METHOD("get_polygon_point_count"), &BattleBox::get_polygon_point_count);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "polygon_point_count", PROPERTY_HINT_RANGE, "100,300,1"), "set_polygon_point_count", "get_polygon_point_count");
}

void BattleBox::_ready() {
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
    target_label = get_node<RichTextLabel>("Target/Targets");
    
    ResourceLoader* loader = ResourceLoader::get_singleton();
    web_scene = loader->load("res://Battle/Soul/box_web.tscn");
    
    webs = get_node<Polygon2D>("BoxContainer/Rect/Webs");
    rect_no_clip = get_node<Node2D>("BoxContainer/Rect/RectNoClip");
    rect_clip = get_node<Polygon2D>("BoxContainer/Rect/Bullets");
    
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
    _init_box();
}

void BattleBox::_init_box() {
    _physics_process(0.0);
    blitter_text->set_text("");
    anchor_targets[0] = Vector2(rect_container->get_theme_constant("margin_left"), rect_container->get_theme_constant("margin_top"));
    anchor_targets[1] = Vector2(640, 480) - Vector2(rect_container->get_theme_constant("margin_right"), rect_container->get_theme_constant("margin_bottom"));
    def_anchors = anchor_targets.duplicate();

    Array effects = stagehand->get_global_effects();
    if(!effects.is_empty()) {
        column1->set_effects(effects);
        column2->set_effects(effects);
        items_label->set_effects(effects);
        mercy_choices->set_effects(effects);
        target_label->set_effects(effects);
        blitter_text->set_effects(effects);
    }
}

void BattleBox::_physics_process(double delta) {
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
    webs->set_polygon(border_pts);
    rect_clip->set_polygon(border_pts);
}

void BattleBox::_process(double delta) {
    Ref<BoxSet> box_set = main->get_encounter()->get_box_set();
    if(box_set.is_valid()) {
        background->set_color(box_set->get_board_color());
        border->set_default_color(box_set->get_board_border_color());
    }

    if(!isPolygonMode) return;
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

void BattleBox::set_wintext(String value) {
    win_text = value;
}

String BattleBox::get_wintext() const {
    return win_text;
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

void BattleBox::set_ease_type(Tween::EaseType value) {
    EaseType = value;
}

Tween::EaseType BattleBox::get_ease_type() const {
    return EaseType;
}

void BattleBox::set_transition_type(Tween::TransitionType value) {
    TransType = value;
}

Tween::TransitionType BattleBox::get_transition_type() const {
    return TransType;
}