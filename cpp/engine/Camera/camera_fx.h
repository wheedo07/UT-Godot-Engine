#ifndef CAMERA_FX_H
#define CAMERA_FX_H
#include<godot_cpp/classes/camera2d.hpp>
#include<godot_cpp/classes/color_rect.hpp>
#include<godot_cpp/classes/canvas_item.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/shader_material.hpp>
namespace godot {
    class CameraFx : public Camera2D {
        GDCLASS(CameraFx, Camera2D)

        protected:
            static void _bind_methods();
        
        private:
            ColorRect* glitcher;
            ColorRect* Fxcher;
            TypedArray<Node> VFX;
            Array tween;
            Ref<ShaderMaterial> Fxmaster;
            bool vfx;
            Vector2 origin_zoom;

            void fx_stop();

        public:
            ColorRect* blinder;
            CameraFx();
            ~CameraFx();

            void _ready() override;
            void _process(double delta) override;

            // 사용함수
            void kill();
            void blind(float time = 0, float targetopacity = 1, float duration = 0.1);
            void blinder_color(Color color = Color(0, 0, 0, 1));
            void add_shake(float amt = 0.1, float speed = 30, float time = 0.4, float duration = 0.15);
            void stop_shake();
            void tween_zoom(Vector2 amount, float time = 0.5, Vector2 offset = Vector2(0, 0));

            // VFX
            void glitch(float time = 0, float targetrate = 1);
            void rgbsplit(float time = 0, float targetrate = 1);
    };
}

#endif