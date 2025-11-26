#ifndef DIALOGUE_LOCALIZATION_H
#define DIALOGUE_LOCALIZATION_H

#include<godot_cpp/classes/resource.hpp>
namespace godot {
    class Dialogues;

    class DialogueLocalization : public Resource {
        GDCLASS(DialogueLocalization, Resource);

        protected:
            static void _bind_methods();

        private:
            Dictionary data;

        public:
            DialogueLocalization();
            ~DialogueLocalization();

            Ref<DialogueLocalization> load(String path);
            Ref<Dialogues> get_localized_dialogues(String locale, String id);
            Ref<Dialogues> get_localized_dialogues_default(String id);
    };
}

#endif