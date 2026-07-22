# AttackBase

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

`AttackBase` is the base class inherited when creating enemy attack patterns in UT-Godot-Engine.

---

## Main Roles
- Bullet creation and management
    - Adding and removing bullet nodes
    - Setting masking modes
- Creating custom attack patterns

---

## Masking System

AttackBase provides 4 rendering modes:

| Mode | Description | Position Reference | Clipping | Usage Example |
|------|-------------|-------------------|----------|---------------|
| `ABSOLUTE_CLIP` | Absolute position + box clipping | Screen absolute coordinates | ✅ | Standard bullet patterns |
| `ABSOLUTE` | Absolute position, no clipping | Screen absolute coordinates | ❌ | Attacks outside the box |
| `RELATIVE_BOX_CLIP` | Box-relative position + clipping | Relative to box | ✅ | Attacks inside the box |
| `RELATIVE_BOX` | Box-relative position, no clipping | Relative to box | ❌ | Attacks outside using box as reference |

> ⚠️ **Note**: `ABSOLUTE` and `ABSOLUTE_CLIP` modes cannot be used in transparent mode.

---

## Methods

### `ready()` (virtual)
```gdscript
func ready():
    # Initialize attack pattern
    # Load bullet scenes, initialize variables, etc.
    pass
```
- Attack pattern initialization function.
- Battle system components are already initialized at this point.

### `start_attack()` (virtual)
```gdscript
func start_attack():
    # Implement actual attack logic
    # Must finish by calling end_attack()
    await get_tree().create_timer(3.0).timeout
    end_attack()
```
- Implement the actual bullet spawning logic here.
- Box resizing, timing control, etc.
- **Must call `end_attack()` when done.**

---

### `quick_bullet(bullet_scene, pos, rot_deg, mask)`
The **most frequently used** bullet creation method.

```gdscript
# Basic bullet creation
var bullet = quick_bullet(bullet_scene, Vector2(100, 50))

# Rotated bullet
var bullet = quick_bullet(bullet_scene, Vector2(100, 50), 45.0)

# With masking mode
var bullet = quick_bullet(bullet_scene, Vector2(100, 50), 0, AttackBase.RELATIVE_BOX_CLIP)
```

**Parameters**:
- `bullet_scene`: The bullet `PackedScene`
- `pos`: Bullet position (`Vector2`)
- `rot_deg`: Rotation angle in degrees (default: `0`)
- `mask`: Masking mode (default: `ABSOLUTE_CLIP`)

**Return value**: The created bullet node (`null` on failure)

### `add_bullet(bullet, mask)`
The standard method for adding a bullet.

```gdscript
var bullet = bullet_scene.instantiate()
bullet.position = Vector2(100, 50)
add_bullet(bullet, AttackBase.RELATIVE_BOX_CLIP)
```

### `end_attack()`
**Every attack pattern** must call this method when complete.

```gdscript
func start_attack():
    # Attack logic...
    await get_tree().create_timer(2.0).timeout
    end_attack()  # Required!
```

Automatically performs the following:
- Emits [`remove_bullets`](#remove_bullets) signal → removes all bullets
- Emits [`remove_attack`](#remove_attack) signal → removes itself

---

## Member Variables

### Script-only Variables
| Property | Type | Description |
|----------|------|-------------|
| `attack_id` | `int` | Attack pattern ID |
| `main` | `BattleMain` | Battle main node |
| `box` | `BattleBox` | Battle box (soul area) |
| `soul` | `SoulBattle` | Player soul |
| `camera` | `CameraController` | Camera controller |
| `mask` | `AttackManager` | Attack manager |

---

## Signals

### `remove_bullets`
- Remove all bullets signal.
- Emitted automatically when the attack pattern completes ([`end_attack()`](#end_attack) is called).

### `remove_attack`
- Remove attack pattern (current node) signal.
- Emitted automatically when the attack pattern completes ([`end_attack()`](#end_attack) is called).

---

## 🔗 Related Docs

- [Enemy - Enemy Character](/classes/userNodes/Enemy.md)
- [Bullet - Base Bullet](/classes/userNodes/Bullet.md)
- [Creating Battles](/tutorials/battle.md)
