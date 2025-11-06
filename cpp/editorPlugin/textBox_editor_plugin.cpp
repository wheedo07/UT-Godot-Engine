#include "textBox_editor_plugin.h"
#include "env.h"

TextBoxEditorPlugin::TextBoxEditorPlugin() {}

TextBoxEditorPlugin::~TextBoxEditorPlugin() {}

void TextBoxEditorPlugin::_bind_methods() {}

bool TextBoxEditorPlugin::_can_handle(Object *p_object) const {
    return p_object->is_class("TextBox");
}

bool TextBoxEditorPlugin::_parse_property(Object *p_object, Variant::Type p_type, const String &p_name, PropertyHint p_hint_type, const String &p_hint_string, BitField<PropertyUsageFlags> p_usage_flags, bool p_wide) 
{
    print_line(p_name);
    return false;
}