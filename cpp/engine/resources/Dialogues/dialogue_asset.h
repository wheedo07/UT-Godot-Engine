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

            void _get_property_list(List<PropertyInfo> *p_list) const;
            bool _set(const StringName& p_name, const Variant& p_value);
            bool _get(const StringName& p_name, Variant& r_ret);

            // 사용 함수
            Ref<DialogueAsset> load_locale_data();
            Ref<Dialogues> get_data(int index);
            bool has_data();

            void set_is_locale(bool p_isLocale);
            bool get_is_locale() const;
    };
}

#endif