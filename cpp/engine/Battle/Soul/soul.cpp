#include "soul.h"
#include "env.h"
#include "engine/Battle/battle_system.h"
#include "engine/Battle/Bullets/bullet_area.h"
#include "engine/Battle/Soul/yellow_soul_bullet.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/input_event_action.hpp>
#include<godot_cpp/classes/project_settings.hpp>
#include<godot_cpp/classes/engine.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/particle_process_material.hpp>
#include<godot_cpp/variant/callable.hpp>

SoulBattle::SoulBattle() {
    gravity_multiplier = 1.0f;
    speed = 160.0f;
    gravity = 3.25f;
    soul_type = SOUL_HUMAN;
    mode = RED;
    inputs = Vector2(0, 0);
    slow_down = 1;
    gravity_direction = Vector2(0, 1);
    special_bullet_mode = ARROW_KEYS_AND_MOVING;
    sprite = nullptr;
    ghost = nullptr;
    shoot = nullptr;
    mode_change_sound = nullptr;
    main = nullptr;
    area = nullptr;
    collision = nullptr;
    wallhit = nullptr;
    green_shield = nullptr;
    cyan_detector = nullptr;
    hiframes = 0;
    iframes = 0;
    invulnerable = false;
    motion = Vector2(0, 0);
    changed_direction_time = 0;
    purple_pos = 0;
    
    jump.resize(4);
    jump[0] = 8.0f;
    jump[1] = 5.0f;
    jump[2] = 2.5f;
    jump[3] = 170.0f;
}

SoulBattle::~SoulBattle() {}

void SoulBattle::_bind_methods() {
    ADD_SIGNAL(MethodInfo("shake_camera", PropertyInfo(Variant::FLOAT, "amt")));
    
    BIND_ENUM_CONSTANT(RED);
    BIND_ENUM_CONSTANT(BLUE);
    BIND_ENUM_CONSTANT(ORANGE);
    BIND_ENUM_CONSTANT(YELLOW);
    BIND_ENUM_CONSTANT(GREEN);
    BIND_ENUM_CONSTANT(CYAN);
    BIND_ENUM_CONSTANT(PURPLE);
    BIND_ENUM_CONSTANT(DISABLE_MOVEMENT);
    
    BIND_ENUM_CONSTANT(SOUL_HUMAN);
    BIND_ENUM_CONSTANT(SOUL_MONSTER);
    
    BIND_ENUM_CONSTANT(ARROW_KEYS);
    BIND_ENUM_CONSTANT(VELOCITY);
    BIND_ENUM_CONSTANT(ARROW_KEYS_AND_MOVING);
    
    ClassDB::bind_method(D_METHOD("set_mode", "new_mode"), &SoulBattle::set_mode, DEFVAL(RED));
    ClassDB::bind_method(D_METHOD("get_mode"), &SoulBattle::get_mode);
    ClassDB::bind_method(D_METHOD("set_gravity_direction", "new_direction", "force_blue_mode"), 
                         &SoulBattle::set_gravity_direction, DEFVAL(true));
    ClassDB::bind_method(D_METHOD("set_gravity_multiplier", "value"), &SoulBattle::set_gravity_multiplier);
    ClassDB::bind_method(D_METHOD("toggle_hpText"), &SoulBattle::toggle_hpText);

    ClassDB::bind_method(D_METHOD("fade_tw_calle", "node_ref", "parent_ref"), &SoulBattle::fade_tw_calle);
    ClassDB::bind_method(D_METHOD("disable"), &SoulBattle::disable);
    ClassDB::bind_method(D_METHOD("enable"), &SoulBattle::enable);
    ClassDB::bind_method(D_METHOD("menu_enable"), &SoulBattle::menu_enable);
    ClassDB::bind_method(D_METHOD("_on_death"), &SoulBattle::_on_death);
    ClassDB::bind_method(D_METHOD("_on_move_soul", "newpos"), &SoulBattle::_on_move_soul);
    
    ClassDB::bind_method(D_METHOD("set_soul_type", "soul_type"), &SoulBattle::set_soul_type);
    ClassDB::bind_method(D_METHOD("get_soul_type"), &SoulBattle::get_soul_type);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "soul_type", PROPERTY_HINT_ENUM, "SOUL_HUMAN,SOUL_MONSTER"), "set_soul_type", "get_soul_type");
    
    ClassDB::bind_method(D_METHOD("set_special_bullet_mode", "mode"), &SoulBattle::set_special_bullet_mode);
    ClassDB::bind_method(D_METHOD("get_special_bullet_mode"), &SoulBattle::get_special_bullet_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "special_bullet_mode", PROPERTY_HINT_ENUM, "ARROW_KEYS,VELOCITY,ARROW_KEYS_AND_MOVING"), 
                "set_special_bullet_mode", "get_special_bullet_mode");
}

void SoulBattle::_ready() {
    if(isEditor) return;
    sprite = Object::cast_to<Node2D>(get_node_internal("Sprite"));
    ghost = Object::cast_to<GPUParticles2D>(get_node_internal("Sprite/Ghost"));
    afterimage = Object::cast_to<GPUParticles2D>(get_node_internal("Sprite/afterimage"));
    shoot = Object::cast_to<AudioStreamPlayer>(get_node_internal("Shoot"));
    mode_change_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("Ding"));
    area = Object::cast_to<Area2D>(get_node_internal("Area2D"));
    collision = Object::cast_to<CollisionShape2D>(get_node_internal("CollisionShape2D"));
    wallhit = Object::cast_to<AudioStreamPlayer>(get_node_internal("Wallhit"));
    hp_label = Object::cast_to<RichTextLabel>(get_node_internal("hp_label"));
    input = Input::get_singleton();
    
    ResourceLoader* loader = ResourceLoader::get_singleton();
    green_mode_scene = loader->load("res://Battle/Soul/green_soul.tscn");
    yellow_bullet_scene = loader->load("res://Battle/Soul/yellow_soul_bullet.tscn");
    cyan_detect_scene = loader->load("res://Battle/Soul/cyan_detection.tscn");
    
    green_shield = Object::cast_to<GreenShielding>(green_mode_scene->instantiate());
    cyan_detector = Object::cast_to<CyanDetection>(cyan_detect_scene->instantiate());
    
    if (green_shield) {
        mode_nodes[GREEN] = green_shield;
    }
    
    if (cyan_detector) {
        mode_nodes[CYAN] = cyan_detector;
    }
    
    main = Object::cast_to<BattleMain>(global->get_scene_container()->get_current_scene());
  
    hp_label->hide();
    set_gravity_direction(Vector2(0, 1), false);
    set_modulate(Color(1, 1, 1, 0));
    red();
    menu_enable();
}

void SoulBattle::_physics_process(double delta) {
    if(isEditor) return;
    if (changed_direction_time > 0) {
        changed_direction_time -= delta;
    }
    
    sprite->set_scale(Vector2(1, 1));
    
    if (gravity_direction.x != 0) {
        motion.x = get_velocity().y;
        motion.y = get_velocity().x * gravity_direction.x;
    } else {
        motion.y = get_velocity().y * gravity_direction.y;
        motion.x = get_velocity().x;
    }

    if(!gravity_direction.is_zero_approx()) set_up_direction(gravity_direction * -1);
    
    switch(mode) {
        case RED:
            red();
            break;
        case BLUE:
            blue();
            break;
        case GREEN:
            green();
            break;
        case YELLOW:
            yellow();
            break;
        case PURPLE:
            purple();
            break;
        case ORANGE:
            orange();
            break;
        case CYAN:
            cyan();
            break;
        default:
            break;
    }
    if(mode != ORANGE) afterimage->set_emitting(false);
    
    _motion_align_gravity_direction();
    
    overlapping_areas = area->get_overlapping_areas();
    for (int i = 0; i < overlapping_areas.size(); i++) {
        Area2D* area_node = Object::cast_to<Area2D>(overlapping_areas[i]);
        if (area_node) {
            check_bullet(area_node);
        }
    }

    hp_label->set_text(vformat("[color=%s]%d/%d", global->get_player_kr() ? "ff00ff" : "ffffff", global->get_player_hp(), global->get_player_max_hp()));
}

void SoulBattle::_process(double delta) {
    if(isEditor) return;
    float delta_frame = ProjectSettings::get_singleton()->get_setting("max_fps", Engine::get_singleton()->get_frames_per_second());
    
    iframes -= delta * delta_frame;
    hiframes -= delta * delta_frame;
    
    // 무적 상태일 때 깜빡임 효과
    if(iframes > 0) {
        if(int(iframes) % 8 == 0) {
            sprite->set_modulate(Color(sprite->get_modulate().r, sprite->get_modulate().g, sprite->get_modulate().b, 1.0));
        }else if (int(iframes) % 8 == 4 && global->get_settings()["vfx"]) {
            sprite->set_modulate(Color(sprite->get_modulate().r, sprite->get_modulate().g, sprite->get_modulate().b, 0.6));
        }
    }else {
        sprite->set_modulate(Color(sprite->get_modulate().r, sprite->get_modulate().g, sprite->get_modulate().b, 1.0));
    }
}

void SoulBattle::_unhandled_input(const Ref<InputEvent>& event) {
    if(isEditor) return;
    if (mode == ORANGE) {
        Vector2 input_list_pressed = Vector2(
            event->is_action_pressed("ui_right") ? 1 : (event->is_action_pressed("ui_left") ? -1 : 0),
            event->is_action_pressed("ui_down") ? 1 : (event->is_action_pressed("ui_up") ? -1 : 0)
        );
        
        if (!input_list_pressed.is_zero_approx()) {
            inputs = input_list_pressed;
        }
    }
}

void SoulBattle::check_bullet(Area2D* area) {
    if(!area->is_class("BulletArea")) return;
    BulletArea* bullet_area = Object::cast_to<BulletArea>(area);
    
    if (!invulnerable && bullet_area) {
        // 치유 영역 확인
        if (hiframes <= 0) {
            if (bullet_area->damage_mode == Bullet::MODE_GREEN) {
                heal(bullet_area);
            }
        }
        
        // 데미지 영역 확인
        if (iframes <= 0) {
            switch (bullet_area->damage_mode) {
                case Bullet::MODE_WHITE:
                    hurt(bullet_area);
                    break;
                case Bullet::MODE_BLUE:
                    if ((special_bullet_mode == ARROW_KEYS && !inputs.is_zero_approx()) || 
                        (special_bullet_mode == VELOCITY && !get_velocity().is_zero_approx()) || 
                        (special_bullet_mode == ARROW_KEYS_AND_MOVING && (!inputs.is_zero_approx() || !get_velocity().is_zero_approx()))) {
                        hurt(bullet_area);
                    }
                    break;
                case Bullet::MODE_ORANGE:
                    if ((special_bullet_mode == ARROW_KEYS && inputs.is_zero_approx()) || 
                        (special_bullet_mode == VELOCITY && get_velocity().is_zero_approx()) || 
                        (special_bullet_mode == ARROW_KEYS_AND_MOVING && (inputs.is_zero_approx() || get_velocity().is_zero_approx()))) {
                        hurt(bullet_area);
                    }
                    break;
            }
        }
    }
}

void SoulBattle::hurt(BulletArea* area) {
    if(!area) return;
    
    iframes = area->iframes;
    
    Dictionary equipment = global->get_equipment();
    Ref<Item> weapon = global->get_item_list()[equipment["weapon"]];
    int defense = global->get_player_defense() + weapon->get_defense_amount() + global->get_temp_def();

    int damage_amount = Math::max(area->damage - defense, 1);
    global->set_player_hp(global->get_player_hp() - damage_amount);
    
    // KR(Karma) 데미지 처리
    if (main->is_kr()) {
        global->set_player_kr(global->get_player_kr() + area->kr);
    }
    
    if(global->get_player_kr() >= global->get_player_hp()) {
        global->set_player_kr(Math::max(global->get_player_hp() - 1, 0));
    }
    
    // 사망 처리
    if(global->get_player_hp() <= 0 && !global->get_debugmode()) {
        if(main->transparent) {
            main->toggle_transparent();
            Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.5);
            timer->connect("timeout", Callable(this, "_on_death"));
        }else _on_death();
        return;
    }
    audio_player->play("hurt");
}

void SoulBattle::_on_death() {
    global->game_over["text"] = main->_on_death_player();
    global->game_over["color"] = sprite->get_modulate();
    sys->clear_system();
    global->save_settings();
    global->set_player_position(get_global_transform_with_canvas().get_origin());
    main->attacks->force_end_attacks();
    global->get_scene_container()->change_scene_to_file("res://Battle/Death/death_screen.tscn");
}

void SoulBattle::heal(BulletArea* area) {
    if(!area) return;
    
    hiframes = 1;
    
    global->heal(area->damage);
    global->set_player_kr(global->get_player_kr() + Math::min<float>(area->kr, area->damage) + 1);
    
    if (global->get_player_kr() >= global->get_player_hp()) {
        global->set_player_kr(Math::max(global->get_player_hp() - 1, 0));
    }
    
    global->heal_sound->play();
}

void SoulBattle::set_mode(SoulMode new_mode) {
    set_mode_silent(new_mode);
    
    mode_change_sound->play();
    
    ghost->restart();
    ghost->set_emitting(true);
}

void SoulBattle::set_mode_silent(int new_mode) {
    mode = SoulMode(new_mode);
    if(!is_node_ready()) return;
    
    if (new_mode == DISABLE_MOVEMENT) {
        inputs = Vector2(0, 0);
    }
    
    Array keys = mode_nodes.keys();
    for (int i = 0; i < keys.size(); i++) {
        Variant key = keys[i];
        CanvasItem* mode_node = Object::cast_to<CanvasItem>(mode_nodes[key]);
        
        if(!mode_node) continue;
        
        if(int(key) != new_mode) {
            if (mode_node->is_inside_tree()) {
                fade_tw = get_tree()->create_tween();
                fade_tw->tween_property(mode_node, "modulate:a", 0.0, FADE_TIME);
                
                Node* node_ref = mode_node;
                Node* parent_ref = this;
                fade_tw->tween_callback(Callable(this, "fade_tw_calle").bind(node_ref, parent_ref));
            }
        }else if(!mode_node->is_inside_tree()) {
            fade_tw = get_tree()->create_tween();
            mode_node->set_modulate(Color(mode_node->get_modulate().r, mode_node->get_modulate().g, mode_node->get_modulate().b, 0.0));
            fade_tw->tween_property(mode_node, "modulate:a", 1.0, FADE_TIME);
            add_child(mode_node);
        }
    }
    if(new_mode != BLUE) set_gravity_direction_silent(Vector2(0, 1));

    if(new_mode == PURPLE) {
        purple_pos = 0;
        gravity_direction = Vector2(0,0);
        motion = Vector2(0, 0);
        update_purple_pos();
    }
}

void SoulBattle::fade_tw_calle(Node* node_ref, Node* parent_ref) {
    if (node_ref->get_parent() == parent_ref) {
        parent_ref->remove_child(node_ref);
    }
}

void SoulBattle::set_gravity_direction(const Vector2& new_direction, bool force_blue_mode) {
    set_up_direction(gravity_direction * -1);
    gravity_direction = new_direction;
    set_velocity(Vector2(0, 0));

    float angle = 0;
    if (new_direction == Vector2(0, 1)) { // 아래쪽
        angle = 0;
    } else if (new_direction == Vector2(0, -1)) { // 위쪽
        angle = Math_PI;
    } else if (new_direction == Vector2(1, 0)) { // 오른쪽
        angle = -Math_PI / 2;
    } else if (new_direction == Vector2(-1, 0)) { // 왼쪽
        angle = Math_PI / 2;
    } else {
        angle = Vector2(0, 1).angle_to(new_direction);
    }
    
    if (soul_type == SOUL_MONSTER) {
        angle += Math_PI;
    }
    
    Ref<Tween> rotation_tween = create_tween();
    rotation_tween->tween_property(sprite, "rotation", angle, 0.2);
    if(force_blue_mode) set_mode(BLUE);
}

void SoulBattle::toggle_hpText() {
    hp_label->set_visible(!hp_label->is_visible());
}

void SoulBattle::set_gravity_direction_silent(const Vector2& new_direction) {
    gravity_direction = new_direction;
    set_up_direction(gravity_direction * -1);
    changed_direction_time = TIME_GRANT;

    set_rotation_degrees(
        (gravity_direction == Vector2(0, 1) ? 0 :
         gravity_direction == Vector2(0, -1) ? 180 :
         gravity_direction == Vector2(1, 0) ? -90 : 90)
    );
}

void SoulBattle::red() {
    if (sprite) {
        if (soul_type == SOUL_MONSTER) {
            sprite->set_modulate(Color(1, 1, 1, 1));
            sprite->set_rotation_degrees(180);
        } else {
            sprite->set_modulate(Color(1, 0, 0, 1));
            sprite->set_rotation_degrees(0);
        }
    }
    four_dir_movement();
}

void SoulBattle::four_dir_movement() {
    slow_down = (input->is_action_pressed("ui_cancel") ? 1 : 0) + 1;
    
    inputs = Vector2(
        input->get_action_raw_strength("ui_right") - input->get_action_raw_strength("ui_left"),
        input->get_action_raw_strength("ui_down") - input->get_action_raw_strength("ui_up")
    );
    
    motion = speed * inputs / slow_down;
}

void SoulBattle::blue() {
    if (sprite) {
        sprite->set_modulate(Color(0, 0, 1, 1));
    }
    
    slow_down = (input->is_action_pressed("ui_cancel") ? 1 : 0) + 1;
    
    if (gravity_direction == Vector2(0, 1)) { // DOWN
        inputs = Vector2(
            input->get_action_strength("ui_right") - input->get_action_strength("ui_left"),
            input->is_action_pressed("ui_up") ? 1 : 0
        );
    } else if (gravity_direction == Vector2(-1, 0)) { // LEFT
        inputs = Vector2(
            input->get_action_strength("ui_down") - input->get_action_strength("ui_up"),
            input->is_action_pressed("ui_right") ? 1 : 0
        );
    } else if (gravity_direction == Vector2(1, 0)) { // RIGHT
        inputs = Vector2(
            input->get_action_strength("ui_down") - input->get_action_strength("ui_up"),
            input->is_action_pressed("ui_left") ? 1 : 0
        );
    } else if (gravity_direction == Vector2(0, -1)) { // UP
        inputs = Vector2(
            input->get_action_strength("ui_right") - input->get_action_strength("ui_left"),
            input->is_action_pressed("ui_down") ? 1 : 0
        );
    }
    
    if (!is_on_floor()) {
        motion.y += gravity * gravity_multiplier;
    }
    
    motion.x = speed * inputs.x / slow_down;
    
    if (is_on_floor()) {
        if (motion.y > 0) {
            motion.y = 0;
        }
        
        if (gravity_multiplier > 1.0f && changed_direction_time <= 0) {
            gravity_multiplier = 1.0f;
            
            wallhit->play();
            
            emit_signal("shake_camera", 0.05f);
        }
        
        if (inputs.y != 0) {
            motion.y -= float(jump[3]);
        }
    } else {
        if (motion.y > 0) {
            motion.y += gravity * (float(jump[2]) - 1);
        } else if (inputs.y == 0) {
            if (motion.y < 20) {
                motion.y = Math::lerp(motion.y, 0, (float(jump[1]) - 1) / 20);
            } else {
                motion.y = Math::lerp(motion.y, 20, (float(jump[0]) - 1) / 20);
            }
        }
    }
}

void SoulBattle::_motion_align_gravity_direction() {
    if (gravity_direction.x != 0) {
        set_velocity(Vector2(motion.y * gravity_direction.x, motion.x));
    } else {
        set_velocity(Vector2(motion.x, motion.y * gravity_direction.y));
    }
    
    move_and_slide();
}

void SoulBattle::green() {
    motion = Vector2(0, 0);
    
    if (sprite) {
        sprite->set_modulate(Color(0, 0.5, 0)); // WEB_GREEN
    }
}

void SoulBattle::yellow() {
    if (sprite) {
        sprite->set_modulate(Color(1, 1, 0)); // YELLOW
        sprite->set_scale(Vector2(1, soul_type == SOUL_HUMAN ? -1 : 1));
    }
    
    if(input->is_action_just_pressed("ui_accept")) {
        if (shoot) {
            shoot->play();
        }
        
        if (yellow_bullet_scene.is_valid() && main) {
            YellowBullet* bullet = Object::cast_to<YellowBullet>(yellow_bullet_scene->instantiate());
            main->box->add_child(bullet);
            bullet->set_global_position(sprite->get_global_position() + Vector2(0, -1).rotated(sprite->get_global_rotation()) * 6);
            bullet->set_rotation(sprite->get_global_rotation());
            bullet->set_velocity(Vector2(0, -1).rotated(sprite->get_global_rotation()) * bullet->get_speed());
        }
    }
    
    four_dir_movement();
}

void SoulBattle::purple() {
    if (sprite) {
        sprite->set_modulate(Color(0.5, 0, 0.5)); // PURPLE
    }
    
    slow_down = (input->is_action_pressed("ui_cancel") ? 1 : 0) + 1;
    
    inputs = input->get_vector("ui_left", "ui_right", "ui_up", "ui_down");
    motion.x = speed * inputs.x / slow_down;
    
    if (input->is_action_just_pressed("ui_down") || 
        input->is_action_just_pressed("ui_up")) {
        purple_pos += int(round(inputs.y));
        
        Array webs_array = main->box->webs_array;
        purple_pos = Math::clamp<int>(purple_pos, 0, webs_array.size() - 1);
        
        update_purple_pos();
    }
}

void SoulBattle::update_purple_pos() {
    if (purple_pos < -1) {
        purple_pos = 0;
        return;
    }
    
    if (p_tween.is_valid() && p_tween->is_valid()) {
        p_tween->kill();
    }
    
    p_tween = get_tree()->create_tween();
    
    float web_y_pos = main->box->get_web_y_pos(purple_pos);
    p_tween->tween_property(this, "global_position:y", web_y_pos, 0.1);
}

void SoulBattle::orange() {
    if (sprite) {
        sprite->set_modulate(Color(1, 0.65, 0)); // ORANGE
    }
    motion = speed * inputs;

    Ref<ParticleProcessMaterial> material = afterimage->get_process_material();
    if(material->get_gravity() != Vector3(motion.x * -1, motion.y * -1, 0)) afterimage->restart();
    material->set_gravity(Vector3(motion.x * -1, motion.y * -1, 0));
    afterimage->set_emitting(true);

    if(get_velocity() == Vector2(0,0)) afterimage->restart();
}

void SoulBattle::cyan() {
    if (sprite) {
        sprite->set_modulate(Color(0, 1, 1)); // CYAN
    }
    
    slow_down = (input->is_action_pressed("ui_cancel") ? 1 : 0) + 1;
    
    inputs = Vector2(
        input->get_action_raw_strength("ui_right") - input->get_action_raw_strength("ui_left"),
        input->get_action_raw_strength("ui_down") - input->get_action_raw_strength("ui_up")
    );
    
    if(cyan_detector && cyan_detector->get_can_move()) {
        motion = speed * inputs / slow_down;
    } else {
        motion = Vector2(0, 0);
    }
}

void SoulBattle::_kill_able_tween() {
    if (_able_tween.is_valid() && _able_tween->is_valid()) {
        _able_tween->kill();
    }
}

void SoulBattle::disable() {
    _kill_able_tween();
    _able_tween = get_tree()->create_tween();
    _able_tween->tween_property(this, "modulate:a", 0.0, 0.2);
}

void SoulBattle::enable() {
    set_physics_process(true);
    set_process_unhandled_input(true);
    inputs = Vector2(0, 0);
    set_z_index(1);
    collision->set_disabled(false);
    
    set_position(Vector2(320, 320));
    enable_tween();
}

void SoulBattle::_on_move_soul(const Vector2& newpos) {
    if (move_tween.is_valid() && move_tween->is_valid()) {
        move_tween->kill();
    }
    
    move_tween = get_tree()->create_tween();
    move_tween->set_trans(Tween::TRANS_EXPO);
    move_tween->set_ease(Tween::EASE_OUT);
    move_tween->set_parallel(true);
    move_tween->tween_property(this, "position", newpos, TIME);
}

void SoulBattle::menu_enable() {
    mode = DISABLE_MOVEMENT;
    set_physics_process(false);
    set_process_unhandled_input(false);
    set_z_index(0);
    collision->set_disabled(true);
    
    enable_tween();
}

void SoulBattle::enable_tween() {
    _kill_able_tween();
    _able_tween = get_tree()->create_tween();
    _able_tween->tween_property(this, "modulate:a", 1.0, 0.2);
}

void SoulBattle::set_soul_type(int p_soul_type) {
    soul_type = p_soul_type;
}

int SoulBattle::get_soul_type() const {
    return soul_type;
}

void SoulBattle::set_special_bullet_mode(int p_mode) {
    special_bullet_mode = p_mode;
}

int SoulBattle::get_special_bullet_mode() const {
    return special_bullet_mode;
}

SoulBattle::SoulMode SoulBattle::get_mode() const {
    return mode;
}

void SoulBattle::set_gravity_multiplier(float value) {
    gravity_multiplier = value;
}