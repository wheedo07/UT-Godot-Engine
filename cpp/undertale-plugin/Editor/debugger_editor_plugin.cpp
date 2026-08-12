#include "debugger_editor_plugin.h"
#define DEBUG_NAMESPACE "ut_debugger"
using namespace godot;

void UTEditorDebuggerPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_session_started"), &UTEditorDebuggerPlugin::_on_session_started);
}

void UTEditorDebuggerPlugin::_setup_session(int32_t p_session_id) {
    session = get_session(p_session_id);
    session->connect("started", Callable(this, "_on_session_started"), CONNECT_ONE_SHOT);
}

bool UTEditorDebuggerPlugin::_has_capture(const String& p_capture) const {
    return p_capture == DEBUG_NAMESPACE;
}

bool UTEditorDebuggerPlugin::_capture(const String& p_message, const Array& p_data, int32_t p_session_id) {
    String message = p_message.replace(DEBUG_NAMESPACE, "").replace(":", "");
    if(message == "call_result") {
        if(p_data.size() < 2) return true;
        const int64_t request_id = p_data[0];
        const Variant result = p_data[1];
        return true;
    }else if(message == "tree_call_result") {
        if(p_data.size() < 2) return true;
        const int64_t request_id = p_data[0];
        const Variant result = p_data[1];
        return true;
    }
    return false;
}

void UTEditorDebuggerPlugin::_on_session_started() {
    remote_tree_call("change_scene_to_file", Array::make("main", "res://test.tscn"));
}

void UTEditorDebuggerPlugin::remote_call(NodePath path, StringName method, Array args) {
    if(session.is_null() || !session->is_active()) {
        print_error("Game debugger session is not active.");
        return;
    }
    const int64_t request_id = next_request_id++;

    Array data = {
        request_id,
        path,
        method,
        args
    };

    session->send_message(
        "ut_debugger:call",
        data
    );
}

void UTEditorDebuggerPlugin::remote_tree_call(StringName method, Array args) {
    if(session.is_null() || !session->is_active()) {
        print_error("Game debugger session is not active.");
        return;
    }
    const int64_t request_id = next_request_id++;

    Array data = {
        request_id,
        method,
        args
    };

    session->send_message(
        "ut_debugger:tree_call",
        data
    );
}