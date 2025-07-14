#ifndef DEATH_SCREEN_H
#define DEATH_SCREEN_H

#include "death_soul.h"
#include "engine/Camera/camera_fx.h"
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
namespace godot {
    class DeathScreen : public Node2D {
        GDCLASS(DeathScreen, Node2D)
        
        protected:
            static void _bind_methods();
        
        private:
            CameraFx* camera;
            DeathSoul* death_soul;
            RichTextLabel* fade_text;
            AudioStreamPlayer* music_player;
            Ref<Tween> tween;
            bool pressed;

            void _setup_death_animation();
            void _done();
        
        public:
            DeathScreen();
            ~DeathScreen();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;
    };
}

#endif