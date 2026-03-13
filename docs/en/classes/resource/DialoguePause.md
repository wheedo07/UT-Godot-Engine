# DialoguePause

The `DialoguePause` class is used to represent a pause at a specific point during dialogue. <br>
It is used together with the [`AdvancedTextTyper`](../userNodes/AdvancedTextTyper.md) node to control the typing effect of dialogue text.

---

## Properties

### `pause_index`
- **Type**: `int`
- **Description**: The character index in the dialogue text where the pause is applied. Typing will pause when this index is reached.
- **Default**: `0`

### `pause_duration`
- **Type**: `float`
- **Description**: The duration (in seconds) to pause when `pause_index` is reached. All characters pause for 0.1 seconds by default; this property overrides that value.
- **Default**: `0.5`

---

## 🔗 Related Docs
- [Dialogue](Dialogue.md)
- [AdvancedTextTyper](../userNodes/AdvancedTextTyper.md)
