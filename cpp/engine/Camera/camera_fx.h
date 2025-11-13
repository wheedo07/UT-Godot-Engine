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
            ColorRect* blinder;
            ColorRect* glitcher;
            ColorRect* shaker;
            TypedArray<Node> VFX;
            Array tween;
            Ref<ShaderMaterial> shaker_shader;
            Ref<ShaderMaterial> transition_shader;
            bool vfx, isTransition;
            float transition_speed;
            Vector2 shake_strengthPx;

            void _on_timeout_transition(bool isblind, float blindtime);
            void _on_finished_blind();

        public:
            CameraFx();
            ~CameraFx();

            void _ready() override;
            void _process(double delta) override;

            // 사용함수
            void kill();
            void transition(String path, float duration=2, float speed=1, bool isblind=true, float blindtime=0.3f);
            void blind(float time = 0.1f, float targetopacity = 1, float duration = 0);
            void blinder_color(Color color = Color(0, 0, 0, 1));
            void add_shake(float amt = 1, float speed = 100, float time = 0.4f, float duration = 0.01f);
            void stop_shake();
            void default_shake_strength();
            void show_blinder();
            void hide_blinder();
            // VFX
            void glitch(float time = 0, float targetrate = 1);
            void rgbsplit(float time = 0, float targetrate = 1);

            void set_shake_strength(Vector2 value);
            Vector2 get_shake_strength() const;
    };
}

#endif