#ifndef SAVE_POINT_H
#define SAVE_POINT_H

#include<godot_cpp/classes/animated_sprite2d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>
#include<godot_cpp/variant/packed_string_array.hpp>
namespace godot {
    class SavePoint : public AnimatedSprite2D {
        GDCLASS(SavePoint, AnimatedSprite2D)
        
        protected:
            static void _bind_methods();
        
        private:
            Ref<PackedScene> txt_box;
            Ref<PackedScene> save_menu;
            PackedStringArray save_text;
        
        public:
            SavePoint();
            ~SavePoint();

            void _ready() override;

            void _on_interact_save();

            void on_dialogue_finished();
            void set_save_text(const PackedStringArray& p_text);
            PackedStringArray get_save_text() const;
    };
}

#endif