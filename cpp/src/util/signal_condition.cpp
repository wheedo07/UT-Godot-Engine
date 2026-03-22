#include "signal_condition.h"
using namespace godot;

SignalCondition::SignalCondition() {
    is_any = false;
}

SignalCondition::~SignalCondition() {}

void SignalCondition::_bind_methods() {
    ClassDB::bind_static_method("SignalCondition", D_METHOD("all", "signals"), &SignalCondition::all);
    ClassDB::bind_static_method("SignalCondition", D_METHOD("any", "signals"), &SignalCondition::any);
    ADD_SIGNAL(MethodInfo("done"));
}

Ref<SignalCondition> SignalCondition::any(Array signals) {
    Ref<SignalCondition> cond = memnew(SignalCondition);
    for(int i=0; i < signals.size(); i++) {
        Signal signal = signals[i];
        StringName signal_name = signal.get_name();
        signal.connect(callable_mp(cond.ptr(), &SignalCondition::_any_signal_emitted), CONNECT_ONE_SHOT);
        cond->pending_signals.push_back(signal_name);
    }
    return cond;
}

Ref<SignalCondition> SignalCondition::all(Array signals) {
    Ref<SignalCondition> cond = memnew(SignalCondition);
    for(int i=0; i < signals.size(); i++) {
        Signal signal = signals[i];
        StringName signal_name = signal.get_name();
        signal.connect(callable_mp(cond.ptr(), &SignalCondition::_all_signal_emitted).bind(signal_name), CONNECT_ONE_SHOT);
        cond->pending_signals.push_back(signal_name);
    }
    return cond;
}

void SignalCondition::_all_signal_emitted(StringName signal_name) {
    pending_signals.erase(signal_name);
    if(pending_signals.size() == 0) {
        emit_signal("done");
    }
}

void SignalCondition::_any_signal_emitted() {
    if(is_any) return;
    is_any = true;
    emit_signal("done");
}