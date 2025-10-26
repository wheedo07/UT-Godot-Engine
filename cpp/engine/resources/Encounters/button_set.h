#ifndef BUTTON_SET_H
#define BUTTON_SET_H

#include<godot_cpp/classes/resource.hpp>
#include<godot_cpp/classes/sprite_frames.hpp>
namespace godot {
    class ButtonSet : public Resource {
        GDCLASS(ButtonSet, Resource);

        protected:
            static void _bind_methods();

        private:
            Ref<SpriteFrames> fight_button;
            Ref<SpriteFrames> act_button;
            Ref<SpriteFrames> item_button;
            Ref<SpriteFrames> mercy_button;
            Vector2 soul_offset;
        
        public:
            ButtonSet();
            ~ButtonSet();

            void set_fight_button(const Ref<SpriteFrames>& p_frames);
            Ref<SpriteFrames> get_fight_button() const;

            void set_act_button(const Ref<SpriteFrames>& p_frames);
            Ref<SpriteFrames> get_act_button() const;

            void set_item_button(const Ref<SpriteFrames>& p_frames);
            Ref<SpriteFrames> get_item_button() const;

            void set_mercy_button(const Ref<SpriteFrames>& p_frames);
            Ref<SpriteFrames> get_mercy_button() const;

            void set_soul_offset(const Vector2& p_offset);
            Vector2 get_soul_offset() const;
    };
}

#endif