#ifndef OPTION_SELECTABLE_H
#define OPTION_SELECTABLE_H

#include<godot_cpp/classes/rich_text_label.hpp>
#include<godot_cpp/variant/color.hpp>
namespace godot {
    class OptionSelectable : public RichTextLabel {
        GDCLASS(OptionSelectable, RichTextLabel)

        protected:
            static void _bind_methods();
        
        private:
            Color default_color;
            Color selected_color;

            bool Selected; 
            bool selected;
        
        public:
            OptionSelectable();
            ~OptionSelectable();

            virtual void _ready() override;

            virtual void set_selected(bool new_val);
            bool get_selected() const;

            void set_initial_selected(bool p_selected);
            bool get_initial_selected() const;

            void set_selected_color(const Color& p_color);
            Color get_selected_color() const;
            
            Color get_default_color() const;

            virtual void reset();
    };
}

#endif