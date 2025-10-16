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
#include<godot_cpp/classes/option_button.hpp>
#include<godot_cpp/classes/thread.hpp>
#include<godot_cpp/classes/os.hpp>
#include<godot_cpp/classes/line_edit.hpp>
namespace godot {
    class Settings : public CanvasLayer {
        GDCLASS(Settings, CanvasLayer);

        protected:
            static void _bind_methods();
        
        private:
            Ref<Thread> load_thread;
            Ref<Tween> tw;
            const Tween::TransitionType TRANSTYPE = Tween::TRANS_EXPO;
            OS* os;
            Panel* Darken;
            CanvasItem* Blur;
            HBoxContainer* BusContainer;
            AnimationPlayer* AnimPlayer;
            VBoxContainer* Options;
            SpinBox* process_edit;
            LineEdit* debug_edit;
            OptionButton* debug_edit2;
            bool enabled;
            PackedStringArray path_list;
            PackedStringArray class_exclude;

            void _scene_input(String text);
            void _change_process(double value);
            void _change_debug(int index);
            void _get_path_list();
            void _scan_directory(const String& path, PackedStringArray& paths);
            void _on_path_list_loaded();
        
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