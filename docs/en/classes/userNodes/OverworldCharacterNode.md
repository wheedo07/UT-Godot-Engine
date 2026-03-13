# OverworldCharacterNode

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

A node for NPC characters in the overworld scene.
Handles movement, animation, dialogue, and interaction area.

---

## Main Roles
- NPC character movement and animation
    - Walking in 4 directions (down, side, up)
    - Idle and move animation states
    - Special `act` animation
- Dialogue and interaction
    - Show dialogue via [`Dialogues`](../resource/Dialogues.md) resource
    - Detect player interaction via `Area2D`
- Alert icon display
    - Show an exclamation mark icon above the NPC

---

## Required Node Structure

```
OverworldCharacterNode
├── OverworldSprite         # Sprite with animations
└── Area2D (area_interact)  # Interaction trigger area
    └── CollisionShape2D
```

---

## Methods

### `start_walking(direction: Vector2, distance: float, speed: float = -1)`
Makes the NPC walk in a direction for a given distance.

```gdscript
# Walk right for 100px
start_walking(Vector2(1, 0), 100.0)

# Walk down at custom speed
start_walking(Vector2(0, 1), 200.0, 80.0)
```

### `force_direction(direction: Vector2)`
Forces the NPC to face a direction without moving.

```gdscript
force_direction(Vector2(0, 1))   # Face down
force_direction(Vector2(1, 0))   # Face right
force_direction(Vector2(0, -1))  # Face up
```

### `show_alert()`
Shows a "!" alert icon above the NPC.

```gdscript
show_alert()
await get_tree().create_timer(0.5).timeout
```

### `set_frame(frame: int)`
Sets the current animation frame of the sprite.

```gdscript
set_frame(0)  # Set to first frame
```

### `play_anim(anim: String)`
Plays a specific animation on the [`OverworldSprite`](OverworldSprite.md).

```gdscript
play_anim("act")        # Play act animation
play_anim("idle_down")  # Play idle down animation
```

---

## Member Variables

### Editor-configurable Variables

#### `character: CharacterSetting`
The character settings resource for this NPC.
Controls font, text box appearance, and dialogue sounds.

```gdscript
character = preload("res://Characters/Toriel.tres")
```

#### `walk_speed: float`
The walking speed of this NPC in pixels per second.

```gdscript
walk_speed = 60.0
```

#### `frame_alert: Texture2D`
The texture used for the alert "!" icon shown by `show_alert()`.

#### `sprite: NodePath`
The path to the `OverworldSprite` node for this character.

```gdscript
sprite = NodePath("OverworldSprite")
```

#### `area_interact: NodePath`
The path to the `Area2D` node used to detect player interaction.

```gdscript
area_interact = NodePath("Area2D")
```

#### `current_index: int`
The current dialogue index. Advances each time the player interacts.

#### `dialogues: DialogueAsset`
The [`DialogueAsset`](../resource/DialogueAsset.md) resource containing this NPC's dialogue lines.

---

## Signals

### `character_finished`
Emitted when the NPC finishes walking (via `start_walking()`).

```gdscript
start_walking(Vector2(1, 0), 100.0)
await character_finished
print("NPC has finished walking.")
```

### `animation_finished`
Emitted when the current animation completes.

```gdscript
play_anim("act")
await animation_finished
print("Act animation finished.")
```

---

## ⚠️ Notes & Tips

- Make sure `sprite` and `area_interact` paths are correctly configured in the Inspector.
- The `character` resource controls the appearance and sound of dialogue — set it to match the NPC's identity.
- `current_index` automatically advances during interaction, so you don't need to manage it manually for linear dialogue.

---

## 🔗 Related Docs

- [OverworldSprite - Animation Setup](OverworldSprite.md)
- [CharacterSetting - Character Config](CharacterSetting.md)
- [DialogueAsset - Dialogue Resource](../resource/DialogueAsset.md)
- [Overworld - Scene Root](Overworld.md)
- [Creating an Overworld](/tutorials/overworld.md)
