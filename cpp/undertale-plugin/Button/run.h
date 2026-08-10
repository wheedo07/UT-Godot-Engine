#pragma once

#include<godot_cpp/classes/button.hpp>
namespace godot {
    class UTEditorRunBtn : public Button {
        GDCLASS(UTEditorRunBtn, Button);

        protected:
            static void _bind_methods();

        public:
            void _ready() override;
            void _pressed() override;
    };
}