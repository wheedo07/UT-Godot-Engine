# OverworldCharacterNode

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

---

## Main Roles
- **Movement System**
    - Directional movement with automatic animation switching between idle/walking states
    - Character movement speed and direction control
- **Animation Management**
    - Automatic handling of directional animations (up, down, left/right)
    - Automatic sprite flip functionality
- **Dialogue System**
    - Integration with [`DialogueAsset`](../resource/DialogueAsset.md) and [`TextBox`](../systemNodes/TextBox.md)
    - Per-character dialogue management
- **Interaction System**
    - Player interaction detection using [`InteractionTrigger`](../systemNodes/InteractionTrigger.md)
    - Automatic dialogue start on interaction
- **Alert System**
    - Visual alert and sound effects on encounter or interaction

---

## Required Components

### Node Structure
```
OverworldCharacterNode
├── OverworldSprite     # Animation sprite (required)
└── InteractionTrigger  # Interaction area (required)
```

### Required Resources
- [`DialogueAsset`](../resource/DialogueAsset.md) - Character dialogue data

---

## Animation States

The character automatically transitions between idle and movement animations based on speed:

### Idle Animations
- `"idle_down"` - Idle facing down
- `"idle_side"` - Idle facing left/right
- `"idle_up"` - Idle facing up

### Move Animations
- `"move_down"` - Moving downward
- `"move_side"` - Moving left/right
- `"move_up"` - Moving upward

### Special Animations
- `"act"` - Animation for custom actions

---

## Methods

### `start_walking(direction: Vector2i = Vector2i(0, 0))`
Starts or stops character movement in the specified direction.
If `direction` is non-zero, starts movement and transitions to the walking animation.
If `direction` is zero, stops movement and transitions to the idle animation.

```gdscript
# Start moving right
character.start_walking(Vector2i(1, 0))

# Move downward
character.start_walking(Vector2i(0, 1))

# Stop movement
character.start_walking(Vector2i(0, 0))
```

### `force_direction(dir: Vector2)`
Forces the character to face a specific direction without moving.
Updates the animation state and sprite direction based on the normalized direction vector.
Useful for making a character face the player or a specific object.

```gdscript
# Set to face the player
var player_pos = global.get_player().global_position
var direction = (player_pos - global_position).normalized()
character.force_direction(direction)

# Lock to a specific direction
character.force_direction(Vector2.DOWN)  # Face downward
```

### `show_alert(duration: float = 0.35)`
Shows an alert indicator above the character for the specified duration.
Plays the encounter sound effect and displays the alert sprite using [`frame_alert`](#frame_alert-int).
The alert automatically hides after the duration has elapsed.

```gdscript
# Show alert for default duration (0.35s)
character.show_alert()

# Show alert for 1 second
character.show_alert(1.0)

# Alert on enemy encounter
func on_enemy_encounter():
    character.show_alert(0.5)
```

### `set_frame(index: int)`
Sets the character sprite to display a specific frame of the "act" animation.
Useful for displaying custom poses or expressions outside the normal animation cycle.

```gdscript
# Display a special pose
character.set_frame(2)  # Frame 2 of the act animation

# Change between different expressions
match emotion:
    "happy":
        character.set_frame(0)
    "sad":
        character.set_frame(1)
    "angry":
        character.set_frame(2)
```

### `play_anim(key: String, speed: float = 1.0, from_end: bool = false)`
Plays a specific animation with optional speed and direction control.
Connected to the `animation_finished` signal, emits the [`animation_finished`](#animation_finished) signal on completion.

```gdscript
# Play normal animation
character.play_anim("act")
await character.animation_finished

# Play at double speed
character.play_anim("move_down", 2.0)

# Play in reverse
character.play_anim("act", 1.0, true)
```

---

## Member Variables

### Editor-configurable Variables

#### `character: String`
The name of the character used for dialogue display.
The name displayed in the dialogue box when the character speaks.

#### `walk_speed: int`
The character's movement speed in pixels per second.
**Range:** 0–400. Higher values make the character move faster.
**Default:** 60

#### `frame_alert: int`
The frame index to display when showing the alert indicator.
References a frame in the alert sprite's animation sequence.
**Default:** 0

#### `sprite: OverworldSprite`
A reference to the [`OverworldSprite`](OverworldSprite.md) component that handles character visuals.
Must be set for the character to display and animate correctly.

#### `area_interact: InteractionTrigger`
A reference to the [`InteractionTrigger`](../systemNodes/InteractionTrigger.md) component that detects player interaction.
Should be set using the (res://Engine/Overworld/Interactions/interaction_trigger.tscn) scene.

#### `current_index: int`
The current dialogue index used when interacting with this character.
References an index in the dialogue array of the [`dialogues`](#dialogues-dialogueasset) asset.
Can be changed to progress to a different dialogue state.
**Default:** 0

#### `dialogues: DialogueAsset`
The [`DialogueAsset`](../resource/DialogueAsset.md) resource containing this character's dialogue data.
Used to display dialogue text when the player interacts with the character.

---

## Signals

### `character_finished`
Emitted when the character's dialogue interaction is complete.
Emitted after a dialogue sequence initiated by player interaction has ended.

```gdscript
func _on_character_finished():
    print("Dialogue with character has ended")
    global.set_player_can_move(true)
```

### `animation_finished`
Emitted when a custom animation played via [`play_anim()`](#play_animkey-string-speed-float--10-from_end-bool--false) is complete.
Useful for chaining animations or triggering events after a specific animation.

```gdscript
func play_sequence():
    character.play_anim("act")
    await character.animation_finished
    print("Special action complete!")
```

---

## 🔗 Related Docs

- [OverworldSprite - Overworld Sprite](OverworldSprite.md)
- [DialogueAsset - Dialogue Asset](../resource/DialogueAsset.md)
- [InteractionTrigger - Interaction Trigger](../systemNodes/InteractionTrigger.md)
- [TextBox - Text Box](../systemNodes/TextBox.md)
