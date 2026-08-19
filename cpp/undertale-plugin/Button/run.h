#pragma once

#include<godot_cpp/classes/button.hpp>
#include<godot_cpp/classes/accept_dialog.hpp>
namespace godot {
    class UTEditorPlugin;
    class UTRunSceneDialog;
    class UTEditorRunBtn : public Button {
        GDCLASS(UTEditorRunBtn, Button);

        protected:
            static void _bind_methods();

        private:
            UTRunSceneDialog *layer_dialog = nullptr;
            AcceptDialog *alert_dialog = nullptr;
            UTEditorPlugin *plugin = nullptr;
            String current_scene_path;

            void _setup_dialogs();
            void _show_alert(String message);
            void _on_layer_dialog_confirmed();

        public:
            void _ready() override;
            void _pressed() override;

            void set_plugin(UTEditorPlugin *value);
    };
}