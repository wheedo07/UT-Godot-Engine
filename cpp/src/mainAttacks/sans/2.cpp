#include "attack_sans.h"
#include "env.h"

void AttackSans::turn_2() {
    box->change_size(Vector2(220, 160));
    soul->set_mode();
    sys->sequence({
        {[this]() {
            emit_signal("throws", Vector2(1, 0));
        }, 5.0},
        {[this]() {
            emit_signal("throws", Vector2(0, -1));
        }, 0.3},
        {[this]() {
            emit_signal("throws", Vector2(-1, 0));
        }, 0.3},
        {[this]() {
            emit_signal("throws", Vector2(0, 1));
        }, 0.3},
        {[this]() {
            box->set_webs(5);
            soul->set_mode(SoulBattle::PURPLE);
        }, 1.3},
        {[this]() {
            box->clear_webs();
            soul->set_mode(SoulBattle::GREEN);
        }, 5.0},
        {[this]() {
            soul->set_mode(SoulBattle::ORANGE);
        }, 5.0},
        {[this]() {
            soul->set_mode(SoulBattle::CYAN);
            sys->time_loop(Array(), [this](double delta, TimeAccumPtr acc) {
                double* time = acc[0];
                if(*time > 1) {
                    Bone* b = create_bone(Masking::RELATIVE_BOX_CLIP);
                    b->tween_height(10, 0);
                    b->set_rotation_degrees(90);
                    int y = UFus::randi_range(10, 150);
                    b->set_position(Vector2(600, y));
                    b->fire(Vector2(100, y), Bullet::MOVEMENT_VELOCITY, 150);
                    *time = 0;
                }else *time += delta;
            }, 25);
        }, 10.0},
        {[this]() {
            soul->set_mode(SoulBattle::YELLOW);
        }, 30.0},
        {[this]() {
            main->toggle_transparent();
            soul->set_mode();
            Vector2 center = Vector2(320, 165);
            float radius = 130.0f;
            float angle_increment = 90.0f;
            sys->time_loop(Array(),[this, center, radius, angle_increment](double delta, TimeAccumPtr acc) {
                double* time = acc[0];
                double* current_angle = acc[1];
                
                *current_angle += UFus::deg_to_rad(angle_increment) * delta;
                *current_angle = UFus::fmod(*current_angle, 2.0 * Math_PI);
                if (*time >= 0.2) {
                    Vector2 spawn_position = Vector2(center.x + radius * cos(*current_angle), center.y + radius * sin(*current_angle));
                    Blaster* bl = create_blaster(Masking::RELATIVE_BOX);
                    bl->set_position(Vector2(spawn_position.x, spawn_position.y-100));
                    bl->set_rotation_degrees(UFus::rad_to_deg(*current_angle) + 90);
                    bl->fire(spawn_position, 1.5, 0.3, 0.4);
                    *time = 0;
                }else *time += delta;
            }, 15);
        }, 5.0},
        {[this]() {
            end_attack();
        }, 16.0f}
    });
}