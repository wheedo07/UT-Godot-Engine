#ifndef OPTION_SELECTABLE_SOLO_H
#define OPTION_SELECTABLE_SOLO_H

#include "option_selectable.h"
#include<godot_cpp/classes/input_event.hpp>
namespace godot {
    class OptionSelectableSolo : public OptionSelectable {
        GDCLASS(OptionSelectableSolo, OptionSelectable)

        protected:
            static void _bind_methods();
        
        private:
            // 활성화 상태
            bool enabled;

            // 소울 위치 오프셋
            Vector2 offset;

            // 주변 옵션 노드 참조
            NodePath node_up;
            NodePath node_down;
            NodePath node_left;
            NodePath node_right;
            NodePath node_accept;
        
        public:
            OptionSelectableSolo();
            ~OptionSelectableSolo();

            void _ready() override;
            void _unhandled_input(const Ref<InputEvent>& event) override;

            // 선택 상태 메서드 오버라이드
            void set_selected(bool new_val) override;
            void reset() override;

            // 활성화 관련 메서드
            void set_enabled(bool new_val);
            bool get_enabled() const;

            void enable();
            void disable();

            // 소울 이동 메서드
            void move_soul(OptionSelectable* node);

            // 노드 참조 게터/세터
            void set_node_up(NodePath p_node);
            NodePath get_node_up() const;

            void set_node_down(NodePath p_node);
            NodePath get_node_down() const;

            void set_node_left(NodePath p_node);
            NodePath get_node_left() const;

            void set_node_right(NodePath p_node);
            NodePath get_node_right() const;

            void set_node_accept(NodePath p_node);
            NodePath get_node_accept() const;

            // 오프셋 게터/세터
            void set_offset(const Vector2& p_offset);
            Vector2 get_offset() const;
    };
}

#endif