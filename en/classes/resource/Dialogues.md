# Dialogues

Represents a collection of [`Dialogue`](Dialogue.md) resources used in the dialogue system of UT-Godot-Engine. Used to create and manage dialogues.

---

## Properties

### `dialogues`
- **Type**: `Array<Dialogue>`
- **Description**: An array of included `Dialogue` resources.
- **Default**: `[]`

---

## Methods

### `from(text: PackedStringArray) -> Dialogues`
- **Description**: Creates [`Dialogue`](Dialogue.md) resources from the given string array and adds them to the [`dialogues`](#dialogues) property.
- **Parameters**:
  - `text: PackedStringArray` - Each string is stored in the [`Dialogue.dialog_text`](Dialogue.md#dialog_text) property.
- **Return value**: `Dialogues` - Returns itself.
- **Example**:
```gdscript
var dialogues = Dialogues.new().from(["Hello!", "Nice to meet you!"])
```

### `get_dialogues_single(dialog_type: DialogueType) -> Array`
- **Description**: Returns an array of values for a specific property from all included [`Dialogue`](Dialogue.md) resources. See the [`DialogueType`](#dialoguetype) section for details.
- **Parameters**:
  - `dialog_type: DialogueType` - A constant specifying the type of property to retrieve.
- **Return value**: `Array` - An array of values for the specified property.
- **Example**:
```gdscript
var dialogues = Dialogues.new().from(["Hello!", "Nice to meet you!"])
var texts = dialogues.get_dialogues_single(Dialogues.DIALOGUE_TEXT)
# texts will be ["Hello!", "Nice to meet you!"]
```

### `set_expressions(expressions: Array) -> Dialogues`
- **Description**: Bulk-sets the [`Dialogue.dialog_expressions`](Dialogue.md#dialog_expressions) property of all [`Dialogue`](Dialogue.md) resources already in the [`dialogues`](#dialogues) property.
- **Parameters**:
  - `expressions: Array` - Each element is an expression index array for each dialogue.
- **Return value**: `Dialogues` - Returns itself.
<br>

- **TextBox example**
```gdscript
dialogues.set_expressions([0, 1, 2]) # Set a single expression per dialogue
dialogues.set_expressions([       # Set an expression sequence per dialogue
    [0, 1],        # First dialogue: expression 0 → 1
    -1,            # Second dialogue: no expression change
    [3, 4, 5]      # Third dialogue: expression 3 → 4 → 5
])
```
- **Enemy node example**: Unlike TextBox, you can specify [`Enemy.expression_sprites`](../userNodes/Enemy.md#expression_sprites-arrayanimatedsprite2d) indices using a 2D array.
```gdscript
dialogues.set_expressions([0, 1, 2]) # Set a single expression per dialogue
dialogues.set_expressions([       # Set expression sequences per expression_sprites index
    [ [0, 1], [0, 2] ],   # First dialogue: body 0 → 1, head 0 → 2
    [-1, -1],             # Second dialogue: no expression change
    [ [2, 3], [1, 0] ]    # Third dialogue: body 2 → 3, head 1 → 0
])
```

### `set_speed(speeds: Array) -> Dialogues`
- **Description**: Bulk-sets the [`Dialogue.dialog_speed`](Dialogue.md#dialog_speed) property of all [`Dialogue`](Dialogue.md) resources in the [`dialogues`](#dialogues) property.
- **Parameters**:
  - `speeds: Array` - Each element is the text display speed for each dialogue.
- **Return value**: `Dialogues` - Returns itself.
- **Example**:
```gdscript
dialogues.set_speed([0.08, 0.05, 0.12])
```

### `set_pauses(pauses: Array) -> Dialogues`
- **Description**: Bulk-sets the [`Dialogue.pauses`](Dialogue.md#pauses) property of all [`Dialogue`](Dialogue.md) resources in the [`dialogues`](#dialogues) property.
- **Parameters**:
  - `pauses: Array` - Each element is an array of pause points for each dialogue.
- **Return value**: `Dialogues` - Returns itself.
- **Example**:
```gdscript
dialogues.set_pauses([
    {"index": 3, "duration": 0.5}, # First dialogue: pause 0.5s after 3rd character
    {"index": 2, "duration": 1.0}, # Second dialogue: pause 1.0s after 2nd character
    {}                             # Third dialogue: no pause
])
# Multiple pauses per dialogue
dialogues.set_pauses([
    [ {"index": 2, "duration": 0.3}, {"index": 5, "duration": 0.7} ], # First dialogue
    [ {"index": 4, "duration": 1.0} ],                                 # Second dialogue
    []                                                                  # Third: no pause
])
```

---

## Constants

### `DialogueType`

#### `DIALOGUE_TEXT`
- **Description**:
    - returns: `Array<String>`
The type representing the text content of dialogue. Used with the [`get_dialogues_single`](#get_dialogues_singledialog_type-dialoguetype---array) method to retrieve the [`Dialogue.dialog_text`](Dialogue.md#dialog_text) property of all `Dialogue` resources.

#### `DIALOGUE_EXPRESSIONS`
- **Description**:
    - returns: `Array<Array<int>>` <br>
The type representing the expression index array of dialogue. Used with [`get_dialogues_single`](#get_dialogues_singledialog_type-dialoguetype---array) to retrieve the [`Dialogue.dialog_expressions`](Dialogue.md#dialog_expressions) property.

#### `DIALOGUE_PAUSES`
- **Description**:
    - returns: `Array<Array<DialoguePause>>` <br>
The type representing the pause points of dialogue. Used with [`get_dialogues_single`](#get_dialogues_singledialog_type-dialoguetype---array) to retrieve the [`Dialogue.pauses`](Dialogue.md#pauses) property.

#### `DIALOGUE_SPEED`
- **Description**:
    - returns: `Array<float>` <br>
The type representing the text display speed of dialogue. Used with [`get_dialogues_single`](#get_dialogues_singledialog_type-dialoguetype---array) to retrieve the [`Dialogue.dialog_speed`](Dialogue.md#dialog_speed) property.

---

## 🔗 Related Docs
- [Dialogue - Dialogue Resource](Dialogue.md)
