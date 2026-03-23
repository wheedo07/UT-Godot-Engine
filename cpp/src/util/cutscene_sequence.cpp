#include "cutscene_sequence.h"
#include "env.h"

CutsceneSequence::CutsceneSequence() {
    skip_requested = false;
}

CutsceneSequence::~CutsceneSequence() {}

void CutsceneSequence::_bind_methods() {
    ClassDB::bind_method(D_METHOD("skip"), &CutsceneSequence::skip);
    ClassDB::bind_method(D_METHOD("skip_callback", "callback"), &CutsceneSequence::skip_callback);
    ClassDB::bind_method(D_METHOD("create_timer", "time"), &CutsceneSequence::create_timer);
    ClassDB::bind_method(D_METHOD("create_tween"), &CutsceneSequence::create_tween);

    ClassDB::bind_method(D_METHOD("set_skip_requested", "value"), &CutsceneSequence::set_skip_requested);
    ClassDB::bind_method(D_METHOD("get_skip_requested"), &CutsceneSequence::get_skip_requested);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "skip_requested", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SCRIPT_VARIABLE), "set_skip_requested", "get_skip_requested");
}

void CutsceneSequence::skip() {
    skip_requested = true;
    for(int i=0; i < timers.size(); i++) {
        Ref<SceneTreeTimer> timer = timers[i];
        if(timer.is_valid()) timer->set_time_left(0);
    }
    for(int i=0; i < tweens.size(); i++) {
        Ref<Tween> tween = tweens[i];
        if(tween.is_valid()) tween->custom_step(10000.0);
    }
    timers.clear();
    tweens.clear();

    if(callback.is_valid()) callback.call();
}

Ref<SceneTreeTimer> CutsceneSequence::create_timer(float time) {
    Ref<SceneTreeTimer> timer;
    if(skip_requested) {
        timer = stagehand->get_tree()->create_timer(0, false);
    }else timer = stagehand->get_tree()->create_timer(time, false);

    timers.push_back(timer);
    timer->connect("timeout", callable_mp(this, &CutsceneSequence::_timer_timeout).bind(timer), CONNECT_ONE_SHOT);
    return timer;
}

Ref<Tween> CutsceneSequence::create_tween() {
    Ref<Tween> tween = stagehand->get_tree()->create_tween();
    if(skip_requested) {
        stagehand->get_tree()->connect("process_frame", 
            callable_mp(this, &CutsceneSequence::_tween_process_frame).bind(tween), CONNECT_DEFERRED);
    }else {
        tweens.push_back(tween);
        tween->connect("finished", callable_mp(this, &CutsceneSequence::_tween_finished).bind(tween), CONNECT_ONE_SHOT);
    }
    return tween;
}

void CutsceneSequence::skip_callback(Callable callback) {
    this->callback = callback;
}

void CutsceneSequence::_timer_timeout(Ref<SceneTreeTimer> timer) {
    timers.erase(timer);
}

void CutsceneSequence::_tween_finished(Ref<Tween> tween) {
    tweens.erase(tween);
}

void CutsceneSequence::_tween_process_frame(Ref<Tween> tween) {
    if(!tween.is_valid()) return;
    tween->custom_step(10000.0);
}

void CutsceneSequence::set_skip_requested(bool value) {
    skip_requested = value;
}

bool CutsceneSequence::get_skip_requested() {
    return skip_requested;
}