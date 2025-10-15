#ifndef SETTINGS_H
#define SETTINGS_H
#include<godot_cpp/classes/canvas_layer.hpp>
#include<godot_cpp/classes/panel.hpp>
#include<godot_cpp/classes/h_box_container.hpp>
#include<godot_cpp/classes/animation_player.hpp>
#include<godot_cpp/classes/v_box_container.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/property_tweener.hpp>
#include<godot_cpp/classes/input.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/spin_box.hpp>
namespace godot {
    class Settings : public CanvasLayer {
        GDCLASS(Settings, CanvasLayer);

        protected:
            static void _bind_methods();
        
        private:
            Ref<Tween> tw;
            const Tween::TransitionType TRANSTYPE = Tween::TRANS_EXPO;
            Panel* Darken;
            CanvasItem* Blur;
            HBoxContainer* BusContainer;
            AnimationPlayer* AnimPlayer;
            VBoxContainer* Options;
            SpinBox* process_edit;
            bool enabled = false;

            void _scene_input(String text);
            void _change_process(double value);
        
        public:
            Settings();
            ~Settings();

            void _ready() override;
			void _unhandled_input(const Ref<InputEvent>& event) override;

            void toggle();
            void on_setting_changed(Node* btn);
    };
}

#endif