# DialogueControl

> 🔴 **Cannot be created by user**
>> This node cannot be created directly by the user; it is instantiated from a scene or used by the system.
>> This node is used by instantiating the `res://Engine/Battle/Misc/dialogue.tscn` scene.

---

## Main Roles
- Manages the enemy speech bubble system during battle.
- Applies the appropriate speech bubble style for each character.
- Handles typing effects and expression changes for dialogue text.
- Automatically called via [`Enemy`](../userNodes/Enemy.md)'s `play_dialogue()` or `play_set_dialogue()`, corresponding to the `Dialogue` node that exists as a child of the `Enemy` node.

---

## Methods

### `type_text_bubble(dialogues: Dialogues)`
Outputs the content of the given [`Dialogues`](../resource/Dialogues.md) resource as a speech bubble.
Usually called via [`Enemy.play_dialogue()`](../userNodes/Enemy.md#play_dialogue-1) or [`Enemy.play_set_dialogue()`](../userNodes/Enemy.md#play_set_dialogue-1), but direct calls are also possible.

### `kill()`
Immediately removes the speech bubble and stops all dialogue text output.
Use this when you need to forcibly end dialogue.

---

## Member Variables

### `character_name: String` (default: `"DEFAULT"`)
The character name for the current speech bubble. Setting this value automatically applies the speech bubble style and text effects matching the specified character.
The character name must match the names of [`CharacterSetting`](../userNodes/CharacterSetting.md) nodes created as children of the Character node under [`Stagehand`](Stagehand.md).

---

## Signals

### `finished_all_texts_dialogue()`
Emitted when all dialogue text output is complete.
Emitted when the dialogue started by `type_text_bubble()` finishes and the speech bubble disappears. Used to indicate dialogue end or to proceed to the next turn in the battle system.

### `set_expression(expressions: Variant)`
Emitted when an expression change command is detected in the dialogue text.
`expressions` is an array containing expression information, used to change the character's facial expression or animation.
This signal allows you to dynamically change the enemy character's expression based on the dialogue content.

### `started_dialogue(line: int)`
Emitted every time text output begins for each dialogue entry in [`Dialogues.dialogues`](../resource/Dialogues.md#dialogues-1).
`line` is the index of the dialogue text currently being output.
You can use this signal to trigger an effect each time a dialogue starts, or change character behavior based on the dialogue content.

---

## 🔗 Related Docs
- [`Enemy`](../userNodes/Enemy.md)
- [`Dialogues`](../resource/Dialogues.md)
