#ifndef ArgsHolder_H
#define ArgsHolder_H
#include<godot_cpp/classes/ref_counted.hpp>
#include<godot_cpp/variant/array.hpp>
namespace godot {    
    class ArgsHolder : public RefCounted {
        GDCLASS(ArgsHolder, RefCounted);

        protected:
            static void _bind_methods();
        
        public:
            Array args;
            float duration = 0.6f;
        
            void set_duration(float p_duration);
            float get_duration() const;
    };
}

#endif