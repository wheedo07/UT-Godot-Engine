#ifndef DIALOGUE_ASSET_H
#define DIALOGUE_ASSET_H

#include<godot_cpp/classes/resource.hpp>
namespace godot {
    class Dialogues;
    class DialogueLocalization;

    class DialogueAsset : public Resource {
        GDCLASS(DialogueAsset, Resource);

        protected:
            static void _bind_methods();

        private:
            bool isLocale;
            String locale_path;
            PackedStringArray locale_keys;
            TypedArray<Dialogues> dialogues;

            Ref<DialogueLocalization> data;

        public:
            DialogueAsset();
            ~DialogueAsset();

            // 사용 함수
            Ref<DialogueAsset> load_locale_data();
            Ref<Dialogues> get_data(int index);
            bool has_data();

            void set_is_locale(bool p_isLocale);
            bool get_is_locale() const;

            void set_locale_path(String path);
            String get_locale_path() const;

            void set_locale_keys(PackedStringArray keys);
            PackedStringArray get_locale_keys() const;

            void set_dialogues(TypedArray<Dialogues> p_dialogues);
            TypedArray<Dialogues> get_dialogues() const;
    };
}

#endif