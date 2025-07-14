#ifndef DIALOGUE_PAUSE_H
#define DIALOGUE_PAUSE_H
#include<godot_cpp/classes/resource.hpp>
namespace godot {
    class DialoguePause : public Resource {
        GDCLASS(DialoguePause, Resource)

        protected:
            static void _bind_methods();
        
        private:
            int pause_index;
            float pause_duration;
        
        public:
            DialoguePause();
            ~DialoguePause();

            void set_pause_index(int p_index);
            int get_pause_index() const;

            void set_pause_duration(float p_duration);
            float get_pause_duration() const;
    };
}

#endif