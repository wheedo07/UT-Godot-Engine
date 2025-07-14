#ifndef ACT_INFO_H
#define ACT_INFO_H
#include<godot_cpp/classes/resource.hpp>

namespace godot {
    class ActInfo : public Resource {
        GDCLASS(ActInfo, Resource)

        protected:
            static void _bind_methods();
        
        private:
            String act;
            PackedStringArray description;
        
        public:
            ActInfo();
            ~ActInfo();
            
            void set_act(const String& p_act);
            String get_act() const;
            
            void set_description(const PackedStringArray& p_description);
            PackedStringArray get_description() const;
    };
}

#endif