#include "camera_fx.h"
#include "env.h"
#include<godot_cpp/classes/method_tweener.hpp>
#include<godot_cpp/variant/utility_functions.hpp>

CameraFx::CameraFx() {
    origin_zoom = Vector2(0,0);
}

CameraFx::~CameraFx() {}

void CameraFx::_bind_methods() {
    ADD_SIGNAL(MethodInfo("finished_tween"));

    ClassDB::bind_method(D_METHOD("kill"), &CameraFx::kill);
    ClassDB::bind_method(D_METHOD("blind", "time", "targetopacity", "duration"), &CameraFx::blind, DEFVAL(0.0f), DEFVAL(1.0f), DEFVAL(0.1));
    ClassDB::bind_method(D_METHOD("blinder_color", "color"), &CameraFx::blinder_color, DEFVAL(Color(0, 0, 0, 1)));
    ClassDB::bind_method(D_METHOD("add_shake", "amt", "speed", "time", "duration"), &CameraFx::add_shake, DEFVAL(0.1f), DEFVAL(30.0f), DEFVAL(0.4f), DEFVAL(0.15f));
    ClassDB::bind_method(D_METHOD("stop_shake"), &CameraFx::stop_shake);
    ClassDB::bind_method(D_METHOD("tween_zoom", "amount", "time", "offset"), &CameraFx::tween_zoom, DEFVAL(Vector2(1, 1)), DEFVAL(0.5f), DEFVAL(Vector2(0, 0)));    

    // VFX
    ClassDB::bind_method(D_METHOD("glitch", "time", "targetrate"), &CameraFx::glitch, DEFVAL(0.0f), DEFVAL(1.0f));
    ClassDB::bind_method(D_METHOD("rgbsplit", "time", "targetrate"), &CameraFx::rgbsplit, DEFVAL(0.0f), DEFVAL(1.0f));
}

void CameraFx::_ready() {
    if(isEditor || !global) return;

    blinder = Object::cast_to<ColorRect>(get_node_internal("CanvasLayer/Blinder"));
    Fxcher = Object::cast_to<ColorRect>(get_node_internal("FX/Fx"));
    glitcher = Object::cast_to<ColorRect>(get_node_internal("Glitch/Glitch"));
    VFX.push_back(glitcher);

    tween.resize(5);
    tween.fill(Ref<Tween>());
    Fxmaster = Fxcher->get_material();

    Dictionary settings = global->get_settings();
    vfx = settings["vfx"];
}

void CameraFx::_process(double delta) {
    if(isEditor || !global) return;
    Dictionary settings = global->get_settings();

    if(!settings["shake"]) Fxmaster->set_shader_parameter("shake_enable", false);

    bool tv = settings["vfx"];
    if (tv != vfx) {
        vfx = settings["vfx"];
        for (int i = 0; i < VFX.size(); i++) {
            Node* item = Object::cast_to<Node>(VFX[i]);
            item->call("set_visible", vfx);
        }
    }
}

void CameraFx::kill() {
    fx_stop();
    if(!origin_zoom.is_zero_approx()) set_zoom(origin_zoom);
    for(int i=0; i < tween.size(); i++) {
        Ref<Tween> tw = tween[i];
        if(tw.is_valid()) tw->kill();
    }
}

void CameraFx::blind(float time, float targetopacity, float duration) {
    int index = 0;
    Ref<Tween> blindertween = tween[index];
    blindertween.unref();
	blindertween = create_tween()->set_trans(Tween::TRANS_SINE);
	blindertween->tween_property(blinder, "modulate:a", targetopacity, time == 0 ? duration : time);
    blindertween->connect("finished", Callable(this, "emit_signal").bind("finished_tween"), CONNECT_ONE_SHOT);
    if(time != 0) blindertween->connect("finished", Callable(this, "blind").bind(0, 0, duration), CONNECT_ONE_SHOT);

    tween[index] = blindertween;
}

void CameraFx::blinder_color(Color color) {
    blinder->set_color(color);
}

void CameraFx::add_shake(float amt, float speed, float time, float duration) {
    int index = 1;
    Ref<Tween> Fxtween = tween[index];
    
    bool is_shake_active = Fxmaster->get_shader_parameter("shake_enable");
    float current_magnitude = 0.0f;
    float current_speed = 0.0f;
    
    if (is_shake_active) {
        current_magnitude = Fxmaster->get_shader_parameter("shake_magnitude");
        current_speed = Fxmaster->get_shader_parameter("shake_speed");
        
        if (amt <= current_magnitude && duration == 0) {
            return;
        }
        
        if (Fxtween.is_valid()) {
            Fxtween->kill();
        }
    }
    
    Fxmaster->set_shader_parameter("shake_enable", true);
    
    Fxtween.unref();
    Fxtween = create_tween()->set_parallel()->set_trans(Tween::TRANS_SINE);
    
    float ramp_up_time = time * 0.2;
    
    float target_magnitude = (is_shake_active) ? amt + current_magnitude * 0.5f : amt;
    target_magnitude = MIN(target_magnitude, amt * 2.0f);
    
    Fxtween->tween_property(Fxcher, "material:shader_parameter/shake_speed", speed, ramp_up_time);
    Fxmaster->set_shader_parameter("shake_magnitude", target_magnitude);
    Fxmaster->set_shader_parameter("shake_hspeed", target_magnitude);
    Fxmaster->set_shader_parameter("shake_vspeed", target_magnitude);
    
    if (duration != 0) {
        float fade_start = duration * 0.7;
        float fade_duration = duration * 0.3;
        
        Fxtween->chain();
        Fxtween->tween_property(Fxcher, "material:shader_parameter/shake_magnitude", 0.0f, fade_duration)->set_delay(fade_start);
        Fxtween->tween_property(Fxcher, "material:shader_parameter/shake_speed", 10.0f, fade_duration)->set_delay(fade_start);

        Fxtween->connect("finished", Callable(this, "stop_shake"), CONNECT_ONE_SHOT);
        Fxtween->connect("finished", Callable(this, "emit_signal").bind("finished_tween"), CONNECT_ONE_SHOT);
    }
    tween[index] = Fxtween;
}

void CameraFx::stop_shake() {
    Fxmaster->set_shader_parameter("shake_enable", false);
}

void CameraFx::tween_zoom(Vector2 amount, float time, Vector2 offset) {
    int index = 2;
    if(origin_zoom.is_zero_approx()) origin_zoom = get_zoom();
    Ref<Tween> zoomtween = tween[index];
    zoomtween.unref();
    zoomtween = create_tween()->set_parallel();
    zoomtween->tween_property(this, "zoom", amount, time);
    zoomtween->tween_property(this, "offset", offset, time);
    zoomtween->connect("finished", Callable(this, "emit_signal").bind("finished_tween"), CONNECT_ONE_SHOT);
    tween[index] = zoomtween;
}

void CameraFx::glitch(float time, float targetrate) {
    int index = 3;
    Ref<Tween> glitchtween = tween[index];
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
    int index = 4;
    Ref<Tween> glitchtween = tween[index];
    glitchtween.unref();
	glitchtween = create_tween()->set_parallel();
	glitchtween->tween_property(glitcher, "material:shader_parameter/shake_color_rate", targetrate * 0.01, time);
    glitchtween->connect("finished", Callable(this, "emit_signal").bind("finished_tween"), CONNECT_ONE_SHOT);
    tween[index] = glitchtween;
}

void CameraFx::fx_stop() {
    Fxmaster->set_shader_parameter("shake_enable", false);
    Fxmaster->set_shader_parameter("lens_enable", false);
    Fxmaster->set_shader_parameter("swirl_enable", false);
    Fxmaster->set_shader_parameter("sine_enable", false);
    Fxmaster->set_shader_parameter("inter_enable", false);
}