#include "interaction_trigger.h"
using namespace godot;

InteractionTrigger::InteractionTrigger() {}

InteractionTrigger::~InteractionTrigger() {}

void InteractionTrigger::_bind_methods() {
    ClassDB::bind_method(D_METHOD("enable"), &InteractionTrigger::enable);
    ClassDB::bind_method(D_METHOD("disable"), &InteractionTrigger::disable);
    
    ADD_SIGNAL(MethodInfo("interacted"));
}

void InteractionTrigger::enable() {
    set_monitorable(true);
    set_monitoring(true);
}

void InteractionTrigger::disable() {
    set_monitorable(false);
    set_monitoring(false);
}