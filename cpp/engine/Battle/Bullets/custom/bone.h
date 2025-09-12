#ifndef BONE_H
#define BONE_H

#include "../defaultbullet.h"
#include "../bullet_area.h"
#include<godot_cpp/classes/nine_patch_rect.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/shader_material.hpp>
namespace godot {
    class Bone : public DefaultBullet {
        GDCLASS(Bone, DefaultBullet);

        public:
            Ref<Tween> h_tween;
            Ref<Tween> w_tween;

        protected:
            static void _bind_methods();
            
        private:
            float shader_time;
            Vector2 scroll_offset;
            
            NinePatchRect* sprite_rect;
            CollisionShape2D* collision;
            BulletArea* area;
            ResourceLoader* loader;
           
            Ref<ShaderMaterial> scroll_shader;

        public:
            Bone();
            ~Bone();
            
            void ready() override;
            void _process(double delta) override;
           
            // 사용 함수
            void scroll_texture(Vector2 speed=Vector2(0,0));
            Ref<PropertyTweener> tween_height(float new_height, float time);
            Ref<PropertyTweener> tween_width(float new_width, float time);

            Ref<Tween> get_h_tween();
            void set_h_tween(Ref<Tween> value);
            Ref<Tween> get_w_tween();
            void set_w_tween(Ref<Tween> value);
    };
}

#endif