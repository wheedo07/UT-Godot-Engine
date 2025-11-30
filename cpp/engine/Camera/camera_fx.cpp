#include "camera_fx.h"
#include "env.h"
#include<godot_cpp/classes/method_tweener.hpp>
#include<godot_cpp/variant/utility_functions.hpp>

CameraFx::CameraFx() {
    isTransition = false;
    transition_speed = 1;
    shake_strengthPx = Vector2(2, 1.5);
}

CameraFx::~CameraFx() {}

void CameraFx::_bind_methods() {
    GDVIRTUAL_BIND(ready);

    ADD_SIGNAL(MethodInfo("finished_tween"));
    ADD_SIGNAL(MethodInfo("finished_transition"));
    ADD_SIGNAL(MethodInfo("killed"));
    ClassDB::bind_method(D_METHOD("_on_timeout_transition", "isblind", "blindtime"), &CameraFx::_on_timeout_transition);
    ClassDB::bind_method(D_METHOD("_on_finished_blind"), &CameraFx::_on_finished_blind);

    ClassDB::bind_method(D_METHOD("set_shake_strength", "value"), &CameraFx::set_shake_strength);
    ClassDB::bind_method(D_METHOD("get_shake_strength"), &CameraFx::get_shake_strength);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shake_strength"), "set_shake_strength", "get_shake_strength");

    ClassDB::bind_method(D_METHOD("kill"), &CameraFx::kill);
    ClassDB::bind_method(D_METHOD("transition", "path", "duration", "speed", "isblind", "blindtime"), &CameraFx::transition, DEFVAL(2), DEFVAL(1), DEFVAL(true), DEFVAL(0.3f));
    ClassDB::bind_method(D_METHOD("blind", "time", "targetopacity", "duration"), &CameraFx::blind, DEFVAL(0.1f), DEFVAL(1), DEFVAL(0));
    ClassDB::bind_method(D_METHOD("blinder_color", "color"), &CameraFx::blinder_color, DEFVAL(Color(0, 0, 0, 1)));
    ClassDB::bind_method(D_METHOD("add_shake", "amt", "speed", "time", "duration"), &CameraFx::add_shake, DEFVAL(1), DEFVAL(100), DEFVAL(0.1f), DEFVAL(0.01f));
    ClassDB::bind_method(D_METHOD("stop_shake"), &CameraFx::stop_shake);
    ClassDB::bind_method(D_METHOD("default_shake_strength"), &CameraFx::default_shake_strength);
    ClassDB::bind_method(D_METHOD("show_blinder"), &CameraFx::show_blinder);
    ClassDB::bind_method(D_METHOD("hide_blinder"), &CameraFx::hide_blinder);

    // VFX
    ClassDB::bind_method(D_METHOD("glitch", "time", "targetrate"), &CameraFx::glitch, DEFVAL(0), DEFVAL(1));
    ClassDB::bind_method(D_METHOD("rgbsplit", "time", "targetrate"), &CameraFx::rgbsplit, DEFVAL(0), DEFVAL(1));
    ClassDB::bind_method(D_METHOD("register_vfx", "vfx_path"), &CameraFx::register_vfx);
}

void CameraFx::_ready() {
    if(!global) return;

    blinder = Object::cast_to<ColorRect>(get_node_internal("CanvasLayer/Blinder"));
    shaker = Object::cast_to<ColorRect>(get_node_internal("CanvasLayer2/Shaker"));
    glitcher = Object::cast_to<ColorRect>(get_node_internal("Glitch/Glitch"));

    tween.resize(4);
    tween.fill(Ref<Tween>());
    shaker_shader = shaker->get_material();

    Dictionary settings = global->get_settings();
    vfx = settings["vfx"];

    register_vfx("Glitch/Glitch");
    if(has_method("ready")) { // C++ 이랑 GDscript 모두 호환되도록
        call("ready");
    }else {
        ready();
    }
}

void CameraFx::ready() {
    // Override 용
}

void CameraFx::_process(double delta) {
    if(!global) return;
    Dictionary settings = global->get_settings();

    shaker_shader->set_shader_parameter("enabled", settings["shake"]);

    bool tv = settings["vfx"];
    if(tv != vfx) {
        vfx = settings["vfx"];
        for (int i = 0; i < VFX.size(); i++) {
            Node* item = Object::cast_to<Node>(VFX[i]);
            item->call("set_visible", vfx);
        }
    }

    if(transition_shader.is_valid()) {
        if(isTransition) {
            transition_shader->set_shader_parameter("progress", 
                float(transition_shader->get_shader_parameter("progress")) + delta * transition_speed);
        }
    }
}

void CameraFx::register_vfx(NodePath vfx_path) {
    Node* vfx_node = get_node_internal(vfx_path);
    VFX.push_back(glitcher);
}

void CameraFx::kill() {
    for(int i=0; i < tween.size(); i++) {
        Ref<Tween> tw = tween[i];
        if(tw.is_valid()) tw->kill();
    }
    if(isTransition) {
        isTransition = false;
        transition_shader.unref();
    }
    blinder->set_material(memnew(Material));
    hide_blinder();
    stop_shake();
    emit_signal("killed");
}

void CameraFx::blind(float time, float targetopacity, float duration) {
    int index = 0;
    Ref<Tween> blindertween = tween[index];
    if(blindertween.is_valid()) blindertween->kill();
    blindertween.unref();
	blindertween = create_tween()->set_trans(Tween::TRANS_SINE);
	blindertween->tween_property(blinder, "modulate:a", targetopacity, time);
    if(duration != 0) {
        blindertween->tween_interval(duration);
        blindertween->connect("finished", Callable(this, "blind").bind(time, 0, 0), CONNECT_ONE_SHOT);
    }else blindertween->connect("finished", Callable(this, "_on_finished_blind"), CONNECT_ONE_SHOT);

    tween[index] = blindertween;
}

void CameraFx::transition(String path, float duration, float speed, bool isblind, float blindtime) {
    if(isTransition) {
        ERR_PRINT("현재 트랜지션이 진행중입니다.");
        return;
    }
    Ref<Tween> blindertween = tween[0];
    if(blindertween.is_valid()) blindertween->kill();
    Color mod = blinder->get_modulate();
    mod.a = 1;
    blinder->set_modulate(mod);
    transition_shader = ResourceLoader::get_singleton()->load(path);
    transition_speed = speed;
    blinder->set_material(transition_shader);

    Ref<SceneTreeTimer> timer = get_tree()->create_timer(duration, false);
    timer->connect("timeout", Callable(this, "_on_timeout_transition").bind(isblind, blindtime), CONNECT_ONE_SHOT);
    isTransition = true;
}

void CameraFx::blinder_color(Color color) {
    blinder->set_color(color);
}

void CameraFx::add_shake(float amt, float speed, float time, float duration) {
    int index = 1;
    Ref<Tween> shaker_tween = tween[index];
    if(shaker_tween.is_valid()) shaker_tween->kill();
    shaker_tween.unref();
   
    shaker_tween = create_tween()->set_parallel();
    shaker_tween->tween_property(shaker, "material:shader_parameter/strength_scale", amt, time);
    shaker_tween->tween_property(shaker, "material:shader_parameter/frequency", speed, time);
    
    if(duration != 0) {
        shaker_tween->chain();
        shaker_tween->tween_property(shaker, "material:shader_parameter/frequency", 0, time)->set_delay(duration);
        shaker_tween->tween_property(shaker, "material:shader_parameter/strength_scale", 0, time)->set_delay(duration);
    }
    shaker_tween->connect("finished", Callable(this, "emit_signal").bind("finished_tween"), CONNECT_ONE_SHOT);
    
    tween[index] = shaker_tween;
}

void CameraFx::stop_shake() {
    int index = 1;
    Ref<Tween> shaker_tween = tween[index];
    if(shaker_tween.is_valid()) {
        shaker_tween->kill();
        shaker_tween.unref();
    }
    
    shaker_shader->set_shader_parameter("strength_scale", 0);
}

void CameraFx::default_shake_strength() {
    set_shake_strength(Vector2(2, 1.5));
}

void CameraFx::show_blinder() {
    Color mod = blinder->get_modulate();
    mod.a = 1;
    blinder->set_modulate(mod);
}

void CameraFx::hide_blinder() {
    Color mod = blinder->get_modulate();
    mod.a = 0;
    blinder->set_modulate(mod);
}

void CameraFx::glitch(float time, float targetrate) {
    int index = 2;
    Ref<Tween> glitchtween = tween[index];
    if(glitchtween.is_valid()) glitchtween->kill();
    glitchtween.unref();
	glitchtween = create_tween()->set_parallel();
	glitchtween->tween_property(glitcher, "material:shader_parameter/shake_power", targetrate * 0.1, time);
	glitchtween->tween_property(glitcher, "material:shader_parameter/shake_color_rate", targetrate * 0.01, time);
    if(time != 0) {
        glitchtween->connect("finished", Callable(this, "glitch").bind(0, 0), CONNECT_ONE_SHOT);
    }else glitchtween->connect("finished", Callable(this, "emit_signal").bind("finished_tween"), CONNECT_ONE_SHOT);
    tween[index] = glitchtween;
}

void CameraFx::rgbsplit(float time, float targetrate) {
    int index = 3;
    Ref<Tween> glitchtween = tween[index];
    if(glitchtween.is_valid()) glitchtween->kill();
    glitchtween.unref();
	glitchtween = create_tween()->set_parallel();
	glitchtween->tween_property(glitcher, "material:shader_parameter/shake_color_rate", targetrate * 0.01, time);
    glitchtween->connect("finished", Callable(this, "emit_signal").bind("finished_tween"), CONNECT_ONE_SHOT);
    tween[index] = glitchtween;
}

void CameraFx::_on_finished_blind() {
    if(isTransition) {
        isTransition = false;
        emit_signal("finished_transition");
    }else emit_signal("finished_tween");
}

void CameraFx::_on_timeout_transition(bool isblind, float blindtime) {
    if(!transition_shader.is_valid()) return;
    transition_shader.unref();
    Color mod = blinder->get_modulate();
    if(isblind) {
        blinder->set_material(memnew(Material));
        blind(blindtime, mod.a ? 0 : 1);
    }else {
        isTransition = false;
        emit_signal("finished_transition");
    }
}

void CameraFx::set_shake_strength(Vector2 value) {
    shake_strengthPx = value;
    if(!isEditor) shaker_shader->set_shader_parameter("strength_px", shake_strengthPx);
}

Vector2 CameraFx::get_shake_strength() const {
    return shake_strengthPx;
}