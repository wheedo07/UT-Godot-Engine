#ifndef DAMAGE_INDICATOR_H
#define DAMAGE_INDICATOR_H

#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/classes/progress_bar.hpp>
#include<godot_cpp/classes/tween.hpp>
#include<godot_cpp/classes/interval_tweener.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
namespace godot {
    class DamageIndicator : public Control {
        GDCLASS(DamageIndicator, Control);
        
        protected:
            static void _bind_methods();
        
        private:
            RichTextLabel* text_label;
            ProgressBar* health_bar;
            AudioStreamPlayer* hit_sound;

            float time;
            int hp;
            int max_hp;
            int damage_amount;
            bool miss;

            Ref<Tween> flash_tween;
            Ref<Tween> animation_tween;
            Ref<Tween> value_tween;

            void _on_frame_processed();
        
        public:
            DamageIndicator();
            ~DamageIndicator();

            void _ready() override;

            // 게터/세터
            void set_hp(int p_hp);
            int get_hp() const;

            void set_max_hp(int p_max_hp);
            int get_max_hp() const;

            void set_damage(int p_damage);
            int get_damage() const;

            void set_miss(bool p_miss);
            bool get_miss() const;

            void set_time(float p_time);
            float get_time() const;
    };
}

#endif