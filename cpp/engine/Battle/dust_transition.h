#ifndef DUST_TRANSITION_H
#define DUST_TRANSITION_H

#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/sub_viewport.hpp>
namespace godot {
    class Enemy;

    class DustTransition : public Node2D {
        GDCLASS(DustTransition, Node2D);

        protected:
            static void _bind_methods();

        public:
            Enemy* enemy;
            Node* sprite;
            enum EASING_TYPE {
                LINEAR,
                EASE_OUT,
                EASE_IN_OUT,
                CUBIC
            };

        private:
            int sample_rate;
            float alpha_threshold;
            float dissolve_speed;
            float transition_speed;
            Color particle_color_tint;

            float gravity;
            float dust_lifetime;
            Vector2 velocity_range_x;
            Vector2 velocity_range_y;

            Vector2i viewport_size;
            float particle_size;
            float size_shrink_factor;
            float transition_size_multiplier;

            EASING_TYPE easing_type;
            float transition_movement_factor;

            bool is_debug;

            void set_viewport_size(const Vector2i& p_size);
            Vector2i get_viewport_size() const;

            void set_sample_rate(int p_rate);
            int get_sample_rate() const;

            void set_alpha_threshold(float p_threshold);
            float get_alpha_threshold() const;

            void set_gravity(float p_gravity);
            float get_gravity() const;

            void set_dust_lifetime(float p_lifetime);
            float get_dust_lifetime() const;

            void set_velocity_range_x(const Vector2& p_range);
            Vector2 get_velocity_range_x() const;

            void set_velocity_range_y(const Vector2& p_range);
            Vector2 get_velocity_range_y() const;

            void set_dissolve_speed(float p_speed);
            float get_dissolve_speed() const;

            void set_transition_speed(float p_speed);
            float get_transition_speed() const;

            void set_is_debug(bool p_debug);
            bool get_is_debug() const;

            void set_particle_color_tint(const Color& p_color);
            Color get_particle_color_tint() const;

            void set_particle_size(float p_size);
            float get_particle_size() const;

            void set_size_shrink_factor(float p_factor);
            float get_size_shrink_factor() const;

            void set_transition_size_multiplier(float p_multiplier);
            float get_transition_size_multiplier() const;

            void set_easing_type(EASING_TYPE p_type);
            EASING_TYPE get_easing_type() const;

            void set_transition_movement_factor(float p_factor);
            float get_transition_movement_factor() const;

        private:
            bool is_active, one_shot;
            SubViewport* viewport;
            Array dust;
            Vector2 sprite_pos;

            float dissolve_progress;
            float sprite_min_y;
            float sprite_max_y;
            float dissolve_height;

            bool _on_sort_custom(Dictionary a, Dictionary b);
            void _on_frame_waited(bool start);
            float apply_easing(float delta);

        public:
            DustTransition();
            ~DustTransition();

            void _draw() override;
            void _process(double delta) override;

            void start_transition();
    };
}
VARIANT_ENUM_CAST(godot::DustTransition::EASING_TYPE);

#endif