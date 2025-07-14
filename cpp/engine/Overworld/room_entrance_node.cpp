#include "room_entrance_node.h"
#include<godot_cpp/classes/resource_loader.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/classes/engine.hpp>
using namespace godot;

RoomEntranceNode::RoomEntranceNode() {
    Area = nullptr;
    facing_direction = Vector2();
    door_id = 0;
    door_margin = 16;
    new_room = "";
    special_room = false;
    new_room_entrance_id = 0;
}

RoomEntranceNode::~RoomEntranceNode() {}

void RoomEntranceNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("force_enter"), &RoomEntranceNode::force_enter);
    
    ClassDB::bind_method(D_METHOD("set_facing_direction", "direction"), &RoomEntranceNode::set_facing_direction);
    ClassDB::bind_method(D_METHOD("get_facing_direction"), &RoomEntranceNode::get_facing_direction);
    
    ClassDB::bind_method(D_METHOD("set_door_id", "id"), &RoomEntranceNode::set_door_id);
    ClassDB::bind_method(D_METHOD("get_door_id"), &RoomEntranceNode::get_door_id);
    
    ClassDB::bind_method(D_METHOD("set_door_margin", "margin"), &RoomEntranceNode::set_door_margin);
    ClassDB::bind_method(D_METHOD("get_door_margin"), &RoomEntranceNode::get_door_margin);
    
    ClassDB::bind_method(D_METHOD("set_new_room", "room"), &RoomEntranceNode::set_new_room);
    ClassDB::bind_method(D_METHOD("get_new_room"), &RoomEntranceNode::get_new_room);
    
    ClassDB::bind_method(D_METHOD("set_special_room", "special"), &RoomEntranceNode::set_special_room);
    ClassDB::bind_method(D_METHOD("get_special_room"), &RoomEntranceNode::get_special_room);
    
    ClassDB::bind_method(D_METHOD("set_new_room_entrance_id", "id"), &RoomEntranceNode::set_new_room_entrance_id);
    ClassDB::bind_method(D_METHOD("get_new_room_entrance_id"), &RoomEntranceNode::get_new_room_entrance_id);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "facing_direction"), "set_facing_direction", "get_facing_direction");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "door_id"), "set_door_id", "get_door_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "door_margin"), "set_door_margin", "get_door_margin");
    
    ADD_GROUP("Entered Room", "");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "new_room", PROPERTY_HINT_FILE, "*.tscn"), "set_new_room", "get_new_room");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "special_room"), "set_special_room", "get_special_room");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "new_room_entrance_id"), "set_new_room_entrance_id", "get_new_room_entrance_id");
}

void RoomEntranceNode::force_enter() {
    Area->_successful_enter();
}

void RoomEntranceNode::_ready() {
    bool isEditor = Engine::get_singleton()->is_editor_hint();
    if(isEditor) return;
    facing_direction = facing_direction.normalized();
    
    Ref<PackedScene> area_scene = ResourceLoader::get_singleton()->load("res://Overworld/Interactions/overworld_area_trigger.tscn");
    Area = Object::cast_to<OverworldAreaTrigger>(area_scene->instantiate());
    
    Area->set_action(special_room ? 19 : 3);
    Area->set_new_room(new_room);
    Area->set_new_room_entrance(new_room_entrance_id);
    add_child(Area, true, Node::INTERNAL_MODE_FRONT);
    Area->set_scale(get_size() / 20.0);
    Area->set_position(get_size() / 2.0);
}

void RoomEntranceNode::set_facing_direction(const Vector2& p_direction) {
    facing_direction = p_direction;
}

Vector2 RoomEntranceNode::get_facing_direction() const {
    return facing_direction;
}

void RoomEntranceNode::set_door_id(int p_id) {
    door_id = p_id;
}

int RoomEntranceNode::get_door_id() const {
    return door_id;
}

void RoomEntranceNode::set_door_margin(int p_margin) {
    door_margin = p_margin;
}

int RoomEntranceNode::get_door_margin() const {
    return door_margin;
}

void RoomEntranceNode::set_new_room(const String& p_room) {
    new_room = p_room;
    if(Area) {
        Area->set_new_room(p_room);
    }
}

String RoomEntranceNode::get_new_room() const {
    return new_room;
}

void RoomEntranceNode::set_special_room(bool p_special) {
    special_room = p_special;
}

bool RoomEntranceNode::get_special_room() const {
    return special_room;
}

void RoomEntranceNode::set_new_room_entrance_id(int p_id) {
    new_room_entrance_id = p_id;
    if(Area) {
        Area->set_new_room_entrance(p_id);
    }
}

int RoomEntranceNode::get_new_room_entrance_id() const {
    return new_room_entrance_id;
}