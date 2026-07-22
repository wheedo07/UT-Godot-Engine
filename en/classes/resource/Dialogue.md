# Dialogue

The `Dialogue` resource represents a single line of dialogue. Multiple dialogues can be grouped and managed using the [`Dialogues`](Dialogues.md) resource.

---

## Properties

### `dialog_expressions`
- **Type**: `Array<int>`
- **Description**: An array of expression indices used in this dialogue. Each index is used as an index on an `AnimatedSprite2D` node.
- **Default**: `[ -1 ]`

### `dialog_speed`
- **Type**: `float`
- **Description**: The speed at which the dialogue text is displayed. Smaller values make text appear faster.
- **Default**: `0.05`

### `dialog_text`
- **Type**: `String`
- **Description**: The actual text content of the dialogue.
- **Default**: `"test"`

### `pauses`
- **Type**: `Array<DialoguePause>`
- **Description**: An array of [`DialoguePause`](DialoguePause.md) resources that define pause points at specific character positions during typing.
- **Default**: `[]`

---

## 🔗 Related Docs
- [DialoguePause](DialoguePause.md)
- [Dialogues](Dialogues.md)
- [TextBox](/classes/systemNodes/TextBox.md)
