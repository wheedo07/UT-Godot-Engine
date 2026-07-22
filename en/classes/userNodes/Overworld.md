# Overworld

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

The root node for overworld scenes in UT-Godot-Engine.
Manages background music, random encounters, and the overworld environment.

---

## Main Roles
- Overworld scene root node
    - Background music management (fade in/out)
    - Random encounter toggle
- Virtual function system
    - Provides virtual functions that respond to overworld events
- Bullet management in overworld
    - Spawn bullets during random encounters

---

## Required Node Structure

```
Overworld
```

> The `Overworld` node is the root of the scene. Add child nodes as needed within the scene.

---

## Methods

### `ready()` (virtual)
Called when the overworld scene is initialized.

```gdscript
extends Overworld

func ready():
    start_music_fade_in()
```

### `start_cellphone()` (virtual)
Called when the player opens the cellphone (menu).

```gdscript
func start_cellphone():
    # Custom logic when opening cellphone
    pass
```

### `player_died()` (virtual)
Called when the player dies.

```gdscript
func player_died():
    # Custom logic when the player dies
    pass
```

### `start_music_fade_in(time: float = 1.0)`
Starts playing background music with a fade-in effect.

```gdscript
start_music_fade_in()         # Fade in over 1 second
start_music_fade_in(2.0)      # Fade in over 2 seconds
```

### `toggle_encounter(enable: bool)`
Enables or disables random encounters.

```gdscript
toggle_encounter(true)   # Enable encounters
toggle_encounter(false)  # Disable encounters (safe area)
```

### `add_bullet(bullet: PackedScene) -> Node`
Adds a bullet to the overworld encounter.

```gdscript
var b = add_bullet(preload("res://attacks/my_bullet.tscn"))
```

### `quick_bullet(bullet: PackedScene, pos: Vector2, dir: Vector2, speed: float) -> Node`
Quickly spawns a bullet with position, direction, and speed.

```gdscript
quick_bullet(
    preload("res://attacks/my_bullet.tscn"),
    Vector2(100, 200),   # Spawn position
    Vector2(1, 0),       # Direction (right)
    200.0                # Speed
)
```

---

## Member Variables

### Editor-configurable Variables

#### `music: AudioStream`
The background music that plays in this overworld scene.

```gdscript
music = preload("res://Musics/ruins.ogg")
```

#### `encounter_rate: float`
The rate at which random encounters occur (steps between encounters).

```gdscript
encounter_rate = 20.0  # Encounter every ~20 steps
```

#### `encounter_scene: PackedScene`
The battle scene to load when a random encounter is triggered.

```gdscript
encounter_scene = preload("res://Battle/froggit_battle.tscn")
```

#### `no_encounter: bool`
If `true`, disables all random encounters in this scene.

```gdscript
no_encounter = true  # Safe area - no encounters
```

### Script-only Variables
| Property | Type | Description |
|----------|------|-------------|
| `player` | `OverworldCharacterNode` | The player character node |
| `global` | `Global` | Global singleton reference |

---

## Signals

### `cached_initialized`
Emitted when the scene's cache is fully initialized and ready.

```gdscript
await cached_initialized
# Now safe to do scene-dependent initialization
```

### `remove_bullets`
Emitted to remove all active overworld bullets.

```gdscript
emit_signal("remove_bullets")  # Clear all bullets
```

### `room_initialized`
Emitted when the room has finished initializing.

```gdscript
await room_initialized
# Room is fully set up
```

---

## ⚠️ Notes & Tips

- Always call `start_music_fade_in()` in `ready()` if you want background music.
- Use `toggle_encounter(false)` in indoor areas or safe zones.
- The `add_bullet()` and `quick_bullet()` functions are intended for overworld encounter sequences.

---

## 🔗 Related Docs

- [OverworldCharacterNode - NPC Character](OverworldCharacterNode.md)
- [OverworldSprite - Character Sprite](OverworldSprite.md)
- [Creating an Overworld](/tutorials/overworld.md)
