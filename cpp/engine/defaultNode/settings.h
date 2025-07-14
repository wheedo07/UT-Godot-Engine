#ifndef SETTINGS_H
#define SETTINGS_H
#include "engine/Menus/bool_setting.h"
#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/panel.hpp>
#include<godot_cpp/classes/canvas_item.hpp>
#include<godot_cpp/classes/h_box_container.hpp>
#include<godot_cpp/classes/animation_player.hpp>
#include<godot_cpp/classes/v_box_container.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/input.hpp>
#include<godot_cpp/classes/input_event.hpp>
namespace godot {
    class Settings : public CanvasLayer {
        GDCLASS(Settings, CanvasLayer);

        protected:
            static void _bind_methods();
        
        private:
            Ref<Tween> tw;
            const int TRANSTYPE = Tween::TRANS_EXPO;
            const float TIME = 0.6f;
            Panel* Darken;
            CanvasItem* Blur;
            HBoxContainer* BusContainer;
            AnimationPlayer* AnimPlayer;
            VBoxContainer* BoolOptions;
            bool enabled = false;
        
        public:
            Settings();
            ~Settings();

            virtual void _ready() override;
            void toggle();
            void on_setting_changed(SettingBoolButton* name);
            virtual void _input(const Ref<InputEvent>& event) override;
    };
}

#endif