#ifndef __CUTSCENE_SEQUENCE_H__
#define __CUTSCENE_SEQUENCE_H__

#include<godot_cpp/classes/ref_counted.hpp>
#include<godot_cpp/classes/scene_tree_timer.hpp>
#include<godot_cpp/classes/tween.hpp>
namespace godot {
    class CutsceneSequence : public RefCounted {
        GDCLASS(CutsceneSequence, RefCounted);

        protected:
            static void _bind_methods();
        
        private:
            bool skip_requested;
            Callable callback;
            Array timers;
            Array tweens;

            void set_skip_requested(bool value);
            bool get_skip_requested();

            void _timer_timeout(Ref<SceneTreeTimer> timer);
            void _tween_finished(Ref<Tween> tween);
            void _tween_process_frame(Ref<Tween> tween);
        
        public:
            CutsceneSequence();
            ~CutsceneSequence();

            void skip();
            void skip_callback(Callable callback);
            Ref<SceneTreeTimer> create_timer(float time);
            Ref<Tween> create_tween();
    };
}

#endif