#include "soul_overwolrd.h"
#include "env.h"
#include "engine/Battle/Bullets/bullet.h"
#include "engine/Battle/Bullets/bullet_area.h"
#include "engine/resources/AudioLibrary/audio_library.h"
#include<godot_cpp/classes/project_settings.hpp>

SoulOverworld::SoulOverworld() {
    isOn = false;
    iframes = 0;
    hiframes = 0;
}

SoulOverworld::~SoulOverworld() {}

void SoulOverworld::_bind_methods() {
    ADD_SIGNAL(MethodInfo("hurt", PropertyInfo(Variant::INT, "damage"), PropertyInfo(Variant::BOOL, "heal")));
}

void SoulOverworld::_ready() {
    hurt_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("hurt"));
    heal_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("heal"));
    area = Object::cast_to<Area2D>(get_node_internal("Area2D"));
}

void SoulOverworld::_physics_process(double delta) {
    if(!isOn) return;

    TypedArray<Area2D> overlapping_areas = area->get_overlapping_areas();
    for(int i=0; i < overlapping_areas.size(); i++) {
        Area2D *area_node = Object::cast_to<Area2D>(overlapping_areas[i]);
        if(area_node) {
            check_bullet(area_node);
        }
    }
}

void SoulOverworld::_process(double delta) {
    float delta_frame = ProjectSettings::get_singleton()->get_setting("max_fps", Engine::get_singleton()->get_frames_per_second());
    
    iframes -= delta * delta_frame;
    hiframes -= delta * delta_frame;
    
    // 무적 상태일 때 깜빡임 효과
    if(iframes > 0) {
        if(int(iframes) % 8 == 0) {
            set_modulate(Color(get_modulate().r, get_modulate().g, get_modulate().b, 1.0));
        }else if (int(iframes) % 8 == 4 && global->get_settings()["vfx"]) {
            set_modulate(Color(get_modulate().r, get_modulate().g, get_modulate().b, 0.6));
        }
    }else {
        set_modulate(Color(get_modulate().r, get_modulate().g, get_modulate().b, 1.0));
    }
}

void SoulOverworld::check_bullet(Area2D *area_node) {
    if(!area_node->is_class("BulletArea")) return;
    BulletArea *bullet_area = Object::cast_to<BulletArea>(area_node);
    if(bullet_area) {
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
                    if(!inputs.is_zero_approx()) {
                        hurt(bullet_area);
                    }
                    break;
                case Bullet::MODE_ORANGE:
                    if(inputs.is_zero_approx()) {
                        hurt(bullet_area);
                    }
                    break;
            }
        }
    }
}

void SoulOverworld::hurt(BulletArea* bullet_area) {
    if(!bullet_area) return;

    iframes = bullet_area->iframes;

    Dictionary equipment = global->get_equipment();
    Ref<Item> weapon = global->get_item_list()[equipment["weapon"]];
    int defense = global->get_player_defense() + weapon->get_defense_amount() + global->get_temp_def();

    int damage_amount = Math::max(bullet_area->damage - defense, 1);
    global->set_player_hp(global->get_player_hp() - damage_amount);
    hurt_sound->play();
    emit_signal("hurt", damage_amount);

    if(global->get_player_hp() <= 0 && !global->get_debugmode()) {
        Overworld *overworld = Object::cast_to<Overworld>(global->get_scene_container()->get_current_scene());
        if(overworld->has_method("player_died")) { // C++ 이랑 GDscript 모두 호환되도록
            global->game_over["text"] = overworld->call("player_died");
        }else {
            global->game_over["text"] = overworld->player_died();
        }
        global->game_over["color"] = get_modulate();
        global->game_over["overworld"] = true;
        sys->clear_system();
        global->save_settings();
        global->set_player_position(get_global_position());
        global->get_scene_container()->change_scene_to_file("res://Core/Custom/GameOver.tscn");
    }
}

void SoulOverworld::heal(BulletArea *bullet_area) {
    if(!bullet_area) return;
    hiframes = 1;
    global->heal(bullet_area->damage);

    if(bullet_area->damage > 0) heal_sound->play();
    emit_signal("hurt", bullet_area->damage, true);
}

void SoulOverworld::start() {
    isOn = true;
    show();
}

void SoulOverworld::stop() {
    isOn = false;
    hide();
}

void SoulOverworld::set_input(Vector2 value) {
    inputs = value;
}