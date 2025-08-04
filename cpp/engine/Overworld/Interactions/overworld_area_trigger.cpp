#include "overworld_area_trigger.h"
#include "env.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/core/math.hpp>
using namespace godot;

OverworldAreaTrigger::OverworldAreaTrigger() {
    required_collider_group = StringName();
    action = 0;
    new_room = "";
    new_room_entrance = -1;
}

OverworldAreaTrigger::~OverworldAreaTrigger() {}

void OverworldAreaTrigger::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_area_entered", "body"), &OverworldAreaTrigger::_on_area_entered);
    ClassDB::bind_method(D_METHOD("_on_body_entered", "body"), &OverworldAreaTrigger::_on_body_entered);
    ClassDB::bind_method(D_METHOD("_successful_enter"), &OverworldAreaTrigger::_successful_enter);
    ClassDB::bind_method(D_METHOD("disable"), &OverworldAreaTrigger::disable);

    ClassDB::bind_method(D_METHOD("set_required_collider_group", "group"), &OverworldAreaTrigger::set_required_collider_group);
    ClassDB::bind_method(D_METHOD("get_required_collider_group"), &OverworldAreaTrigger::get_required_collider_group);
    
    ClassDB::bind_method(D_METHOD("set_action", "action"), &OverworldAreaTrigger::set_action);
    ClassDB::bind_method(D_METHOD("get_action"), &OverworldAreaTrigger::get_action);
    
    ClassDB::bind_method(D_METHOD("set_new_room", "room"), &OverworldAreaTrigger::set_new_room);
    ClassDB::bind_method(D_METHOD("get_new_room"), &OverworldAreaTrigger::get_new_room);
    
    ClassDB::bind_method(D_METHOD("set_new_room_entrance", "entrance"), &OverworldAreaTrigger::set_new_room_entrance);
    ClassDB::bind_method(D_METHOD("get_new_room_entrance"), &OverworldAreaTrigger::get_new_room_entrance);

    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "required_collider_group"), "set_required_collider_group", "get_required_collider_group");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "action", PROPERTY_HINT_FLAGS, "ROOM_CHANGE,STOP_MOVEMENT,CUTSCENE,ONE_SHOT,NON_ROOM_SCENE_CHANGE"), "set_action", "get_action");
    
    ADD_GROUP("ROOM_CHANGE", "");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "new_room", PROPERTY_HINT_FILE, "*.tscn"), "set_new_room", "get_new_room");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "new_room_entrance"), "set_new_room_entrance", "get_new_room_entrance");

    ADD_SIGNAL(MethodInfo("cutscene"));
    ADD_SIGNAL(MethodInfo("stopped_obj"));
    ADD_SIGNAL(MethodInfo("finished_work"));

    BIND_ENUM_CONSTANT(ROOM_CHANGE);
    BIND_ENUM_CONSTANT(STOP_MOVEMENT);
    BIND_ENUM_CONSTANT(CUTSCENE);
    BIND_ENUM_CONSTANT(ONE_SHOT);
    BIND_ENUM_CONSTANT(NON_ROOM_SCENE_CHANGE);
}

void OverworldAreaTrigger::_ready() {
    if(isEditor) return;
    Collision = Object::cast_to<CollisionShape2D>(get_node_internal("Collision"));
    Color debug_color = Color(
        UtilityFunctions::randf(),
        UtilityFunctions::randf(),
        UtilityFunctions::randf(),
        0.35
    );
    Collision->set_debug_color(debug_color);
}

void OverworldAreaTrigger::_on_area_entered(Area2D* body) {
    if (body) {
        _on_body_entered(body->get_parent());
    }
}

void OverworldAreaTrigger::_on_body_entered(Node* body) {
    if (!body) return;

    if (!required_collider_group.is_empty()) {
        if (!body->is_in_group(required_collider_group)) {
            return;
        }
        emit_signal("stopped_obj");
        if (action & STOP_MOVEMENT) {
            if(body->has_method("set_canmove")) {
                body->call("set_canmove", false);
            }else WARN_PRINT("set_canmove 가 없는 오브젝트가 들어왔습니다.");
        }
        if (action & CUTSCENE) {
            emit_signal("cutscene");
            if (action & ONE_SHOT) {
                emit_signal("finished_work");
            }
        }
        return;
    }

    if (body->is_in_group("player")) {
        _successful_enter();
    }
}

void OverworldAreaTrigger::_successful_enter() {
    if (action & STOP_MOVEMENT) {
        global->set_player_can_move(false);
        if (action & ONE_SHOT) {
            emit_signal("finished_work");
        }
    }

    if (action & CUTSCENE) {
        emit_signal("cutscene");
        if (action & ONE_SHOT) {
            emit_signal("finished_work");
        }
    }

    if (action & (ROOM_CHANGE | NON_ROOM_SCENE_CHANGE)) {
        if (new_room.is_empty()) {
            ERR_PRINT("새로운 방이 설정되지 않았습니다.");
            return;
        }
        global->set_just_died(false);
        if (action & NON_ROOM_SCENE_CHANGE) {
            scene_changer->load_general_scene(new_room);
        } else if (action & ROOM_CHANGE) {
            Dictionary extra_data;
            extra_data["entrance"] = new_room_entrance;
            scene_changer->enter_room_path(new_room, extra_data);
        }
        if (action & ONE_SHOT) {
            emit_signal("finished_work");
        }
    }
}

void OverworldAreaTrigger::disable() {
    set_deferred("monitorable", false);
    set_deferred("monitoring", false);
    if (Collision) {
        Collision->set_deferred("disabled", true);
    }
}

void OverworldAreaTrigger::set_required_collider_group(const StringName& p_group) {
    required_collider_group = p_group;
}

StringName OverworldAreaTrigger::get_required_collider_group() const {
    return required_collider_group;
}

void OverworldAreaTrigger::set_action(int p_action) {
    action = p_action;
}

int OverworldAreaTrigger::get_action() const {
    return action;
}

void OverworldAreaTrigger::set_new_room(const String& p_room) {
    new_room = p_room;
}

String OverworldAreaTrigger::get_new_room() const {
    return new_room;
}

void OverworldAreaTrigger::set_new_room_entrance(int p_entrance) {
    new_room_entrance = p_entrance;
}

int OverworldAreaTrigger::get_new_room_entrance() const {
    return new_room_entrance;
}