#ifndef SAVE_POINT_H
#define SAVE_POINT_H

#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/variant/packed_string_array.hpp>
namespace godot {
    class Dialogues;

    class SavePoint : public AnimatedSprite2D {
        GDCLASS(SavePoint, AnimatedSprite2D)
        
        protected:
            static void _bind_methods();
        
        private:
            Ref<PackedScene> save_menu;
            Ref<Dialogues> dialogues;
        
        public:
            SavePoint();
            ~SavePoint();

            void _ready() override;

            void _on_interact_save();

            void _on_dialogue_finished();
            void set_dialogue(Ref<Dialogues> value);
            Ref<Dialogues> get_dialogue() const;
    };
}

#endif