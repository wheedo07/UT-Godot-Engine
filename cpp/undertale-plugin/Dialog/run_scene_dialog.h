#pragma once

#include<godot_cpp/classes/confirmation_dialog.hpp>
#include<godot_cpp/classes/item_list.hpp>
namespace godot {
    class UTRunSceneDialog : public ConfirmationDialog {
        GDCLASS(UTRunSceneDialog, ConfirmationDialog);

        protected:
            static void _bind_methods();

        private:
            ItemList *layer_list = nullptr;
            void _ensure_ui();

        public:
            void _ready() override;

            void set_layers(PackedStringArray layers);
            StringName get_selected_layer();
    };
}