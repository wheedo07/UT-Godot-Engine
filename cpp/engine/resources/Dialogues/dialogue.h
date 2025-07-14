#ifndef DIALOGUE_H
#define DIALOGUE_H
#include<godot_cpp/godot.hpp>
#include<godot_cpp/variant/array.hpp>
#include<godot_cpp/variant/dictionary.hpp>
#include<godot_cpp/classes/resource.hpp>
#include "pause.h"
namespace godot {
	class Dialogue : public Resource {
		GDCLASS(Dialogue, Resource);

        protected:
            static void _bind_methods();

		private:
			String dialog_text;
			double dialog_speed;
			Array dialog_expressions;
			TypedArray<DialoguePause> pauses;
		
		public:
			enum PauseType {CHAR, INDEX_LIST};
			Dialogue();
			~Dialogue();

    		void set_dialog_text(const String& p_text);
    		String get_dialog_text() const;

    		void set_dialog_speed(float p_speed);
    		float get_dialog_speed() const;

    		void set_dialog_expressions(const TypedArray<int>& p_expressions);
    		TypedArray<int> get_dialog_expressions() const;

			void set_pauses(const TypedArray<DialoguePause>& p_pauses);
			TypedArray<DialoguePause> get_pauses() const;
	};
}
VARIANT_ENUM_CAST(godot::Dialogue::PauseType);

#endif