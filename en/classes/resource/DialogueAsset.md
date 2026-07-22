# DialogueAsset

The `DialogueAsset` resource is used in the editor to define dialogue content. It can load dialogue content from either a [`DialogueLocalization`](DialogueLocalization.md) resource or a [`Dialogues`](Dialogues.md) resource.

---

## Properties

### `is_locale`
- Type: `bool`
- Default: `false`
- Description: Specifies whether to use a [`DialogueLocalization`](DialogueLocalization.md) resource when loading dialogue. If set to `false`, a [`Dialogues`](Dialogues.md) resource is used.

### `dialogues`
- Type: `Array[Dialogues]`
- Description: An array of dialogue resources to use when [`is_locale`](#is_locale) is set to `false`. General dialogue content is defined here.

### `locale_path`
- Type: `String`
- Description: The path to the JSON file to use when [`is_locale`](#is_locale) is set to `true`. The file format must follow the [`DialogueLocalization`](DialogueLocalization.md) resource format.

### `locale_keys`
- Type: `PackedStringArray`
- Description: An array of locale keys to use when [`is_locale`](#is_locale) is set to `true`. Dialogue content is loaded from the JSON file according to index.

---

## 🔗 Related Docs
- [DialogueLocalization](DialogueLocalization.md)
- [Dialogues](Dialogues.md)
