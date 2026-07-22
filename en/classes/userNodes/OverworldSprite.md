# OverworldSprite

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

A sprite node for characters in the overworld scene.
Automatically sets up directional walking and idle animations based on a configured sprite sheet.

---

## Main Roles
- Automatic animation setup for overworld characters
    - `idle_down`, `idle_side`, `idle_up` — standing animations
    - `move_down`, `move_side`, `move_up` — walking animations
    - `act` — special action animation
- Alert icon offset configuration

---

## Required Node Structure

```
OverworldSprite
```

> `OverworldSprite` extends `AnimatedSprite2D`. Just add it as a child of `OverworldCharacterNode`.

---

## Member Variables

### Editor-configurable Variables

#### `alert_offset: Vector2`
The offset position for the alert "!" icon shown by [`OverworldCharacterNode.show_alert()`](OverworldCharacterNode.md#show_alert).

```gdscript
alert_offset = Vector2(0, -40)  # Show 40px above the sprite
```

---

## Animation Setup

`OverworldSprite` automatically generates the following animations from your `SpriteFrames` resource:

| Animation Name | Description |
|----------------|-------------|
| `idle_down`    | Idle facing down |
| `idle_side`    | Idle facing left/right |
| `idle_up`      | Idle facing up |
| `move_down`    | Walking downward |
| `move_side`    | Walking left/right |
| `move_up`      | Walking upward |
| `act`          | Special action/interaction animation |

> ⚠️ **Do not remove auto-generated animations.** The engine uses these animation names internally to control character movement. Removing or renaming them will break movement behavior.

---

## ⚠️ Notes & Tips

- All 7 animations listed above must be present in the `SpriteFrames` resource for movement to work correctly.
- The `idle_side` and `move_side` animations face right by default — left-facing is handled by flipping the sprite automatically.
- Set `alert_offset` to match the height of your character sprite so the "!" icon appears above the character's head.

---

## 🔗 Related Docs

- [OverworldCharacterNode - NPC Character](OverworldCharacterNode.md)
- [Overworld - Scene Root](Overworld.md)
- [Creating an Overworld](/tutorials/overworld.md)
