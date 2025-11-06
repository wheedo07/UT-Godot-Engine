#ifndef ENGINE_EDITOR_PLUGIN_H
#define ENGINE_EDITOR_PLUGIN_H

#include<godot_cpp/classes/editor_plugin.hpp>
namespace godot {
    class EngineEditorPlugin : public EditorPlugin {
        GDCLASS(EngineEditorPlugin, EditorPlugin)

        protected:
            static void _bind_methods();

        public:
            EngineEditorPlugin();
            ~EngineEditorPlugin();

            void _enter_tree() override;
            void _exit_tree() override;
    };
}

#endif