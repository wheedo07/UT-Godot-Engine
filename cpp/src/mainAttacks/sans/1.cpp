#include "attack_sans.h"
#include<godot_cpp/classes/window.hpp>
#include "env.h"

void AttackSans::turn_1() {
    box->change_size(Vector2(300, 140));
    emit_signal("throws", Vector2(0, 1));
    sys->time_loop(Array::make(0.8), [this](double delta, TimeAccumPtr acc) {
        double* time = acc[0];
        if(*time >= 1) {
            Bone* b = create_bone(Masking::ABSOLUTE);
            b->set_rotation_degrees(180);
            b->set_position(Vector2(120, 400));
            b->fire(Vector2(420, 400), Bullet::MOVEMENT_TWEEN, 160);
            b->tween_height(45, 1.1);
            Bone* b_2 = create_bone(Masking::ABSOLUTE);
            b_2->set_position(Vector2(120, 250));
            b_2->fire(Vector2(420, 250), Bullet::MOVEMENT_TWEEN, 160);
            b_2->tween_height(95, 1.1);
            
            Bone* b2 = create_bone(Masking::ABSOLUTE);
            b2->set_rotation_degrees(180);
            b2->set_position(Vector2(520, 400));
            b2->fire(Vector2(220, 400), Bullet::MOVEMENT_TWEEN, 160);
            b2->tween_height(45, 1.1);
            Bone* b2_2 = create_bone(Masking::ABSOLUTE);
            b2_2->set_position(Vector2(520, 250));
            b2_2->fire(Vector2(220, 250), Bullet::MOVEMENT_TWEEN, 160);
            b2_2->tween_height(95, 1.1);
            
            sys->sleep([this, b, b_2, b2, b2_2]() {
                b->queue_free(); b_2->queue_free();
                b2->queue_free(); b2_2->queue_free();
            }, 1.8);
            *time = 0;
        }else *time += delta;
    }, 10);
    sys->sequence({
        {[this]() {
            soul->set_mode();
            box->change_size(Vector2(140, 140));
            box->rotate_by(UFus::deg_to_rad(42));
        }, 10.2},
        {[this]() {
            Blaster* bl = create_blaster(Masking::ABSOLUTE);
            bl->set_rotation_degrees(42);
            bl->set_position(Vector2(328, 495));
            bl->fire(Vector2(328, 395), 1.3, 0.5, 0.8);

            Blaster* bl2 = create_blaster(Masking::ABSOLUTE);
            bl2->set_rotation_degrees(-50);
            bl2->set_position(Vector2(328, 495));
            bl2->fire(Vector2(328, 395), 1.3, 0.5, 0.8);

            Blaster* bl3 = create_blaster(Masking::ABSOLUTE);
            bl3->set_rotation_degrees(42);
            bl3->set_position(Vector2(328, 134));
            bl3->fire(Vector2(328, 234), 1.3, 0.5, 0.8);

            Blaster* bl4 = create_blaster(Masking::ABSOLUTE);
            bl4->set_rotation_degrees(-50);
            bl4->set_position(Vector2(328, 134));
            bl4->fire(Vector2(328, 234), 1.3, 0.5, 0.8);

            Blaster* bl5 = create_blaster(Masking::ABSOLUTE);
            bl5->set_position(Vector2(320, -100));
            bl5->fire(Vector2(320, 100), 0.5, 0.5, 0.8);
        }, 0.5},
        {[this]() {
            end_attack();
        }, 2.2f}
    });
}