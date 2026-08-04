#pragma once

#include<godot_cpp/classes/editor_plugin.hpp>
namespace godot {
    class UTEditorPlugin : public EditorPlugin {
        GDCLASS(UTEditorPlugin, EditorPlugin);

        protected:
            static void _bind_methods();

        public:
            void _enter_tree() override;
            void _exit_tree() override;
    };
}