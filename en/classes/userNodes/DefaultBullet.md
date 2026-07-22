# DefaultBullet

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

The `DefaultBullet` node inherits from [`Bullet`](Bullet.md) and implements basic bullet behavior. You can inherit from this node to easily create custom bullets.

---

## Main Roles
- Provides basic bullet behavior
    - Movement (move to coordinates, move in a direction)
    - Automatic collision shape creation
- Provides a foundation for custom bullet implementation
    - Delayed firing
    - Tween-based animation

---

## Movement Modes

### `MOVEMENT_VELOCITY` (Velocity-based)
Handles continuous physics-based movement.
Moves in the direction of the target coordinates at the specified speed without stopping.

```gdscript
extends DefaultBullet

func ready():
    # Instant fire - velocity based
    fire(Vector2(300, 0), MOVEMENT_VELOCITY, 200.0)  # Move right at 200px/sec
```

### `MOVEMENT_TWEEN` (Tween-based)
Handles smooth animation-based movement.
Stops when the target coordinates are reached.

```gdscript
extends DefaultBullet

func ready():
    # Tween settings
    TweenTrans = Tween.TRANS_SINE
    TweenEase = Tween.EASE_OUT
    
    # Tween-based movement
    fire(Vector2(300, 200), MOVEMENT_TWEEN, 150.0)  # Time calculated by distance/speed
```

---

## Required Node Structure

```
DefaultBullet
├── BulletArea                  # Collision detection area
│   └── CollisionShape2D        # Collision shape (auto-created if not specified)
```

---

## Methods

### `fire(target, movement_type, speed, mode)`
Fires the bullet immediately.

```gdscript
# Basic fire
fire(Vector2(100, 100), MOVEMENT_VELOCITY, 200.0)

# Fire with mode
fire(Vector2(0, 200), MOVEMENT_TWEEN, 150.0, Bullet.MODE_BLUE)

# Parameter descriptions:
# target: Target position (Vector2)
# movement_type: Movement mode (MOVEMENT_VELOCITY | MOVEMENT_TWEEN)
# speed: Speed or value used for time calculation (default: 100.0)
# mode: Bullet mode (default: keeps previous setting)
```

#### Speed Calculation

**Velocity-based (MOVEMENT_VELOCITY):**
```gdscript
# speed = actual movement speed (pixels/second)
fire(Vector2(400, 0), MOVEMENT_VELOCITY, 200.0)  # Arrives after 2 seconds
```

**Tween-based (MOVEMENT_TWEEN):**
```gdscript
# time = distance / speed
var distance = position.distance_to(target)  # e.g. 500px
fire(target, MOVEMENT_TWEEN, 100.0)  # Moves over 5 seconds (500/100)
```


### `queue_fire(delay, target, movement_type, speed, mode)`
Fires the bullet after a delay.

```gdscript
# Fire after 1 second
queue_fire(1.0, Vector2(200, 0), MOVEMENT_VELOCITY, 150.0)

# Rapid-fire pattern
queue_fire(0.5, Vector2(100, 100), MOVEMENT_TWEEN, 200.0)
queue_fire(1.0, Vector2(-100, 100), MOVEMENT_TWEEN, 200.0)
queue_fire(1.5, Vector2(0, -100), MOVEMENT_TWEEN, 200.0)
```

> 📝 **Note**: If an existing tween is running, the delay starts after it completes.


---

## Signals

### `tween_finished`
Emitted when the tween movement is complete. Only works in [`MOVEMENT_TWEEN`](#movement_tween-tween-based) mode.

```gdscript
bullet.fire(Vector2(300, 0), MOVEMENT_TWEEN, 150.0)
await bullet.tween_finished
print("Bullet has reached its target!")
```

---

## Member Variables

### Editor-configurable Variables

#### `collision_margin: float`
The margin value for the collision area.
```gdscript
collision_margin = 3.0  # Collision area 3px smaller than the sprite
```

#### `collision_path: NodePath`
The path to the `CollisionShape2D` node used for collision detection.
```gdscript
collision_path = NodePath("BulletArea/CollisionShape2D")
```

#### `TweenTrans: Tween.TransitionType`
The tween transition type.
```gdscript
TweenTrans = Tween.TRANS_SINE  # Smooth curve
```

#### `TweenEase: Tween.EaseType`
The tween easing type.
```gdscript
TweenEase = Tween.EASE_OUT  # Decelerate at end
```

### Script-only Variables
| Property | Type | Description |
|----------|------|-------------|
| target_position | Vector2 | Current target position |
| velocity_tween | Tween | Tween instance (used in tween mode only) |

---

## ⚠️ Notes & Tips

1. **Missing path setting**
```gdscript
# Not setting this may cause crashes.
# ✅ Correct path setup
collision_path = NodePath("BulletArea/CollisionShape2D")
```

2. **Using CollisionPolygon2D instead of CollisionShape2D**
```gdscript
# ❌ Wrong node type
collision_path = NodePath("BulletArea/CollisionPolygon2D")  # X
```

- `MOVEMENT_VELOCITY` mode is suitable for continuous movement — it never stops.
- `MOVEMENT_TWEEN` mode is ideal for smoothly moving to a specific point.
- Calling `fire()` again while a tween is running cancels the existing tween and starts a new movement.

---

## 🔗 Related Docs

- [Bullet class](Bullet.md) - Base bullet behavior and damage mode system
- [AttackBase class](AttackBase.md) - Bullet creation and attack pattern management
- [BulletArea - Bullet collision area](BulletArea.md)
- [Creating Battles](/tutorials/battle.md)
