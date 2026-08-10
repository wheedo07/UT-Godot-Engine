#include "camera.h"
#include "undertale-engine/Global/ut.h"
using namespace godot;

UTGECamera::UTGECamera() {
    connect("ready", Callable(this, "_utge_ready"));
}

void UTGECamera::_bind_methods() {
    /* 내부 메서드 */
    ClassDB::bind_method(D_METHOD("_utge_ready"), &UTGECamera::_utge_ready);

    ClassDB::bind_method(D_METHOD("set_camera", "value"), &UTGECamera::set_camera);
    ClassDB::bind_method(D_METHOD("get_camera"), &UTGECamera::get_camera);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera2D", PROPERTY_USAGE_SCRIPT_VARIABLE | PROPERTY_USAGE_READ_ONLY), "set_camera", "get_camera");
}

void UTGECamera::_utge_ready() {
    camera = UT::tree()->get_root()->get_camera();
    set_remote_node(camera->get_path());
}

void UTGECamera::set_camera(Camera2D *value) {
    camera = value;
}

Camera2D *UTGECamera::get_camera() {
    return camera;
}