#pragma once

#include<godot_cpp/classes/editor_debugger_plugin.hpp>
#include<godot_cpp/classes/editor_debugger_session.hpp>
namespace godot {
    class UTEditorDebuggerPlugin : public EditorDebuggerPlugin {
        GDCLASS(UTEditorDebuggerPlugin, EditorDebuggerPlugin);

        protected:
            static void _bind_methods();

        private:
            Ref<EditorDebuggerSession> session;
            int64_t next_request_id = 0;

        public:
            void _setup_session(int32_t p_session_id) override;
            bool _has_capture(const String& p_capture) const override;
            bool _capture(const String& p_message, const Array& p_data, int32_t p_session_id) override;

            void _on_session_started();

            void remote_call(NodePath path, StringName method, Array args = Array());
            void remote_tree_call(StringName method, Array args = Array());
    };
}