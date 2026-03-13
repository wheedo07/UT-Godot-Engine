# CharacterSetting

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

`CharacterSetting` inherits from [`AudioStreamPlayer`](https://docs.godotengine.org/en/stable/classes/class_audiostreamplayer.html) and is a node that manages per-character dialogue settings such as fonts, speech bubble head icons, and typing sound effects.

---

## Main Roles
- Define per-character dialogue style
    - Apply font, font size, and BBCode style
    - Specify speech bubble head sprite
- Manage typing sounds (inherits `AudioStreamPlayer`)
    - Handles the click sound played each time a character is typed
    - Can suppress sound or add delay for specific characters

---

## Required Node Structure

`CharacterSetting` nodes must be added as children of the `Character` node in `res://Core/autoload/stagehand.tscn`.
**The node's name is used directly as the character name.**

```
Stagehand
└── Character
    ├── DEFAULT          # Default character (required)
    ├── SANS             # Custom character example
    └── MY_CHARACTER     # User-added character
```

> ⚠️ The `DEFAULT` character must always exist. The `generic()` method of [`TextBox`](../systemNodes/Stagehand.md) uses the `DEFAULT` character's settings.

---

## Using Character Names

The node name is used as a string to specify the character.

**For overworld NPC dialogue:**
```gdscript
# Set the character property on OverworldCharacterNode
character = "SANS"
```

**For battle speech bubbles (DialogueControl):**
```gdscript
# Set the character_name property on DialogueControl
$Dialogue.character_name = "SANS"
```

---

## Member Variables

### Editor-configurable Variables

#### `font: Font`
The font used for the character's dialogue.
If not set, the default font is used.

```gdscript
# Example setting in a .tscn file
font = preload("res://assets/Fonts/sans.ttf")
```

#### `text_size: float` (default: `28`)
The font size for the character's dialogue.

```gdscript
text_size = 28.0
```

#### `entire_text_bbcode: String` (default: `""`)
A BBCode style applied to all of the character's dialogue text at once.
Additional BBCode can be applied per individual dialogue line; this setting is applied to all lines globally.

```gdscript
entire_text_bbcode = "[b]"                    # Bold entire text
entire_text_bbcode = "[i]"                    # Italic entire text
entire_text_bbcode = "[wave amp=10 freq=5]"   # Wavy text
```

#### `text_box_head: SpriteFrames`
The head icon sprite displayed next to the character's dialogue speech bubble.
`SpriteFrames` must consist of **50×50** frames, where each frame represents the animation of the icon.

> 📝 If not set, the speech bubble is displayed without a head icon (same as when `head_hide` is `true` in [`TextBox.character()`](../systemNodes/Stagehand.md)).

#### `extra_delay: String` (default: `@#$%^&+=_-~\`<>"|\\*{}()[].,!?`)
When a character in this string is encountered during typing, an extra delay (long pause) is applied.
Punctuation and special characters are included by default for a natural typing rhythm.

```gdscript
extra_delay = ".,!?"   # Only delay at period, comma, exclamation, question mark
```

#### `no_sound: String` (default: `@#$%^&+=_-~\`<>"|\\*{}()[].,!? `)
No click sound is played for characters included in this string during typing.
Spaces, special characters, and punctuation are included by default to provide a natural sound effect.

```gdscript
no_sound = " "   # Silence only at spaces
```

### Inherited AudioStreamPlayer Properties

Since `CharacterSetting` inherits from `AudioStreamPlayer`, properties like `stream`, `bus`, and `max_polyphony` are also used.

| Property | Purpose |
|----------|---------|
| `stream` | Typing click sound file |
| `bus` | Audio bus (`"SFX"` recommended) |
| `max_polyphony` | Max simultaneous plays (value of `3` recommended) |

---

## Usage Examples

### Adding a character in stagehand.tscn

```
# Open the stagehand scene in the Godot editor
# res://Core/autoload/stagehand.tscn

Stagehand
└── Character
    ├── DEFAULT
    └── MY_CHAR   ← Add a CharacterSetting node and name it
```

Set properties in the editor:
```
font               = res://assets/Fonts/my_font.ttf
text_size          = 28
entire_text_bbcode = ""
text_box_head      = [SpriteFrames for MY_CHAR]
stream             = res://sfx/Clicks/my_click.wav
max_polyphony      = 3
bus                = "SFX"
```

### Applying to an overworld NPC

```gdscript
# In OverworldCharacterNode
extends OverworldCharacterNode

func _ready():
    character = "MY_CHAR"  # Must match the CharacterSetting node name
```

### Applying to a battle speech bubble

```gdscript
# In Enemy
extends Enemy

func _on_get_turn():
    $Dialogue.character_name = "MY_CHAR"
    play_dialogue(0)
    await finished_dialogue
```

---

## ⚠️ Notes & Tips

1. **`DEFAULT` character is required**
   ```
   # If DEFAULT is missing, an error occurs when using generic text boxes.
   Character
   └── DEFAULT  ← Must exist
   ```

2. **Node name is case-sensitive**
   ```gdscript
   # ❌ Wrong
   character = "Sans"   # If the node is named "SANS", it won't be found
   
   # ✅ Correct
   character = "SANS"
   ```

3. **text_box_head frame size**
   ```
   # Each frame in SpriteFrames must be exactly 50×50 pixels.
   ```

---

## 🔗 Related Docs

- [Stagehand](../systemNodes/Stagehand.md)
- [DialogueControl - Battle Speech Bubble](../systemNodes/DialogueControl.md)
- [OverworldCharacterNode - Overworld Character](OverworldCharacterNode.md)
- [Dialogues - Dialogue Resource](../resource/Dialogues.md)
