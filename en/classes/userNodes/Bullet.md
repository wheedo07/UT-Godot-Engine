# Bullet

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

The `Bullet` node is used in the battle system of UT-Godot-Engine.
Bullets interact with the player and can have various effects.

---

## Main Roles
- Player collision handling
    - Implements the damage mode system
    - Provides callback methods called on collision
- Custom bullet implementation
    - e.g. Gaster Blaster, bones, spears, amalgamate attacks, etc.

---

## Damage Mode System

Implements Undertale's core mechanism — the color-based bullet system:

| Mode | Color | Effect | Usage Example |
|------|-------|--------|---------------|
| `MODE_NULL` | Keeps previous state (white if none) | No change | Default value |
| `MODE_WHITE` | ⚪ White | Always deals damage | Standard bullets |
| `MODE_GREEN` | 🟢 Green | Restores HP | Healing bullet |
| `MODE_BLUE` | 🔵 Blue | Deals damage when moving | Stop puzzle |
| `MODE_ORANGE` | 🟠 Orange | Deals damage when still | Move puzzle |

### Automatic Color Change
```gdscript
# Color is automatically applied when mode is set
bullet.set_mode(Bullet.MODE_BLUE)   # Changes to blue
bullet.set_mode(Bullet.MODE_GREEN)  # Changes to green
bullet.set_mode(Bullet.MODE_NULL)   # Stays green (no change)
```

---

## Required Node Structure
```
Bullet
├── BulletArea  # Collision area
│   └── CollisionShape2D  # Collision shape
```

---

## Methods

### `ready()` (virtual)
Bullet initialization function.

```gdscript
extends Bullet

func ready():
    # Basic setup
    damage = 15
    set_mode(Bullet.MODE_BLUE)
    sprite_path = NodePath("Sprite2D")  # Usually set in the editor
    
    # Add special effects on spawn
    setup_custom_effects()

func setup_custom_effects():
    # Rotation effect
    var tween = create_tween()
    tween.set_loops()
    tween.tween_property(self, "rotation", rotation + PI * 2, 2.0)
```

### `on_hit_player()` (virtual)
Called when the bullet collides with the player.

```gdscript
extends Bullet

func on_hit_player():
    # Special collision effect
    print("Player was hit by the bullet!")
    
    # Create explosion effect
    create_explosion_effect()
    
    # Spawn child bullets
    spawn_child_bullets()
    
    # Remove bullet
    kill()

func create_explosion_effect():
    var explosion = preload("res://effects/explosion.tscn").instantiate()
    get_parent().add_child(explosion)
    explosion.global_position = global_position

func spawn_child_bullets():
    for i in range(4):
        var child_bullet = preload("res://bullets/small_bullet.tscn").instantiate()
        get_parent().add_child(child_bullet)
        child_bullet.global_position = global_position
        var angle = (PI * 2 / 4) * i
        child_bullet.velocity = Vector2(cos(angle), sin(angle)) * 100
```

### `on_hit_yellow()` (virtual)
Called when the bullet is hit by a yellow soul's bullet.

```gdscript
extends Bullet

func on_hit_yellow():
    # Destroyed by yellow bullet
    print("Destroyed by yellow bullet!")
    kill()
```

### `on_hit_player_shield()` (virtual)
Called when the bullet is blocked by the green soul's shield.

```gdscript
extends Bullet

func on_hit_player_shield():
    # Blocked by shield
    print("Blocked by shield!")
    kill()
```

### `_physics_update(delta: float)` (virtual)
A custom update function called every physics frame. <br>
Use this method instead of overriding `_physics_process`.

```gdscript
extends Bullet

func _physics_update(delta: float):
    # Replaces _physics_process
    pass
```

### `set_mode(mode: DamageMode)`
Sets the bullet's damage mode.

```gdscript
bullet.set_mode(Bullet.MODE_ORANGE)  # Set orange mode
```

### `get_mode() -> DamageMode`
Returns the bullet's current damage mode.

```gdscript
var current_mode = bullet.get_mode()
print("Current mode: ", current_mode)
```

### `fade()`
Fades out the bullet. Usually called by [`end_attack()`](AttackBase.md#remove_bullets).

```gdscript
bullet.fade()
```

---

## Member Variables

### Editor-configurable Variables

#### `damage: int`
The amount of damage this bullet deals.

#### `iframe_grant`
The number of invincibility frames granted to the player on hit.

#### `kr_amount: float`
The amount of KR (Karma) the bullet inflicts on the player — gradually reduces HP over time.

#### `delete_upon_hit: bool`
Whether to automatically remove the bullet when it hits the player. If `true`, [`on_hit_player()`](#on_hit_player-virtual) is not called.

#### `sprite_path: NodePath`
The path to the bullet's sprite. Required for color changes. **(Must be set)**

#### `colors: Array<Color>`
An array of colors for the bullet. <br>
Index order: [0]=white, [1]=green, [2]=blue, [3]=orange <br>
Default: `[Color.WHITE, Color.GREEN, Color(0, 0.85, 1), Color(1, 0.65, 0)]`

---

## ⚠️ Notes

### Recommended

1. **Set the sprite path**
   ```gdscript
   func ready():
       sprite_path = NodePath("Sprite2D")  # Required for color changes (set in editor)
   ```

2. **Proper collision handling**
   ```gdscript
   # For normal cases
   delete_upon_hit = true
   
   # When special interaction is needed
   delete_upon_hit = false
   
   func on_hit_player():
       # Custom logic
       kill()  # Remove manually
   ```

3. **Performance optimization**
   ```gdscript
   # Auto-remove when off screen
   func _on_visible_on_screen_notifier_2d_screen_exited():
       queue_free()
   ```

### ❌ Things to Avoid

1. **Never override `_physics_process`**
   ```gdscript
   # ❌ Don't do this!
   func _physics_process(delta):
       # Conflicts with engine internal logic
   
   # ✅ Do this instead
   func _physics_update(delta):
       # Custom physics logic
   ```

2. **Missing sprite path**
   ```gdscript
   # ❌ Color changes won't work
   # (no sprite_path set)
   
   # ✅ Correct setup
   sprite_path = NodePath("Sprite2D")
   ```

---

## 🔗 Related Docs

- [AttackBase - Attack Pattern](AttackBase.md)
