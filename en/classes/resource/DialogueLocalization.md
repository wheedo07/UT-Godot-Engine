# DialogueLocalization

The `DialogueLocalization` class is used to localize dialogue resources. It provides the ability to adjust dialogue content to match different languages and regions.

---

## Methods

### `load(path: string) -> DialogueLocalization`
Loads dialogue localization data from the specified JSON file path.
- **Parameters:**
  - `path`: The path to the JSON file containing the localization data.
- **Return value:** Returns itself (the `DialogueLocalization` instance).
- **JSON Example:**
```json
{
    "SANS_CALL_1": {
        "ko": {
            "text": ["* 전화 받았습니다", "* WA sans"],
            "speed": [0.05],
            "pauses": [
                { "index": 6, "duration": 1.0 }
            ]
        },
        "en": {
            "text": ["* Call answered", "* WA sans"],
            "speed": [0.03]
        },
        "expressions": [0, 1]
    }
}
```
- **Description:** The JSON file can define dialogue lines, typing speed, pause points, and expression indices for multiple languages per dialogue ID.

### `get_dialogues(locale: string, id: string) -> Dialogues`
After calling [`load`](#loadpath-string---dialoguelocalization), retrieves the dialogue data for a specific language and dialogue ID.
- **Parameters:**
  - `locale`: The desired language code (e.g., `"ko"`, `"en"`).
  - `id`: The dialogue ID.
- **Return value:** Returns a [`Dialogues`](Dialogues.md) object matching the given language and ID.

### `get_dialogues_default(id: string) -> Dialogues`
After calling [`load`](#loadpath-string---dialoguelocalization), retrieves the dialogue data matching the current system language setting.
- **Parameters:**
  - `id`: The dialogue ID.
- **Return value:** Returns a [`Dialogues`](Dialogues.md) object matching the current system language.
- **Description:** This method automatically selects the appropriate dialogue content based on the system's default language setting.

---

## 🔗 Related Docs

- [Dialogues](Dialogues.md)
- [DialoguePause](DialoguePause.md)
