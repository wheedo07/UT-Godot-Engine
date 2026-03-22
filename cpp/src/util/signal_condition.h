#ifndef __SIGNAL_CONDITION_H__
#define __SIGNAL_CONDITION_H__

#include<godot_cpp/classes/ref_counted.hpp>
#include<godot_cpp/variant/signal.hpp>
namespace godot {
    class SignalCondition : public RefCounted {
        GDCLASS(SignalCondition, RefCounted);

        protected:
            static void _bind_methods();

        public:
            bool is_any;
            Array pending_signals;

            void _all_signal_emitted(StringName signal_name);
            void _any_signal_emitted();
        
        public:
            SignalCondition();
            ~SignalCondition();

            static Ref<SignalCondition> all(Array signals);
            static Ref<SignalCondition> any(Array signals);
    };
}

#endif