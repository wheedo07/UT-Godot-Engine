#ifndef CHARACTER_SPRITE_H
#define CHARACTER_SPRITE_H

#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/sprite_frames.hpp>
namespace godot {
    class CharacterSprite : public AnimatedSprite2D {
        GDCLASS(CharacterSprite, AnimatedSprite2D)

        protected:
            static void _bind_methods();

        private:
            PackedStringArray animation_names;

            Vector2 alert_offset;

        public:
            CharacterSprite();
            ~CharacterSprite();

            void _process(double delta) override;

            void set_alert_offset(Vector2 p_offset);
            Vector2 get_alert_offset() const;
    };
}

#endif