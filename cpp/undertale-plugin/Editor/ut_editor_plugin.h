#pragma once

#include<godot_cpp/classes/editor_plugin.hpp>
namespace godot {
    class UTEditorDebuggerPlugin;
    class UTEditorPlugin : public EditorPlugin {
        GDCLASS(UTEditorPlugin, EditorPlugin);

        protected:
            static void _bind_methods();

        private:
            Ref<UTEditorDebuggerPlugin> debugger;

        public:
            void _enter_tree() override;
            void _exit_tree() override;

            void current_play(StringName layer_id, String scene_path);
    };
}