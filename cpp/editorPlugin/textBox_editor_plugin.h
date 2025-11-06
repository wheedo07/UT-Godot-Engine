#ifndef TEXT_BOX_EDITOR_PLUGIN_H
#define TEXT_BOX_EDITOR_PLUGIN_H

#include<godot_cpp/classes/editor_inspector_plugin.hpp>
#include<godot_cpp/classes/option_button.hpp>
namespace godot {
    class TextBoxEditorPlugin : public EditorInspectorPlugin {
        GDCLASS(TextBoxEditorPlugin, EditorInspectorPlugin)

        protected:
            static void _bind_methods();

        public:
            TextBoxEditorPlugin();
            ~TextBoxEditorPlugin();

            bool _can_handle(Object *p_object) const override;
            bool _parse_property(Object *p_object, Variant::Type p_type, const String &p_name, PropertyHint p_hint_type, 
                const String &p_hint_string, BitField<PropertyUsageFlags> p_usage_flags, bool p_wide) override;
    };
}

#endif