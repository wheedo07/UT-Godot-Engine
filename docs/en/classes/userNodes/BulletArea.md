# BulletArea

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

The `BulletArea` node is an `Area2D`-based class used to define the collision area of a bullet.  
Used in place of `Area2D` inside a [`Bullet`](Bullet.md) node.

---

## Main Roles
- Collision detection
    - Handles collision with the player
- Automatic system integration
    - Integrates with the bullet's damage mode system
    - Calls the bullet's callback methods on collision

---

## Required Node Structure

```
Bullet
├── BulletArea (bullet_path: "../") # Collision area
│   └── CollisionShape2D            # Collision shape
```

---

## Methods

### `is_player_hit()`
Returns whether the bullet has collided with the player.

```gdscript
if bullet_area.is_player_hit():
    print("Hit the player!")
```

---

## Member Variables

### Editor-configurable Variables

#### `bullet_path: NodePath<Bullet>`
- The path to the `Bullet` node that this `BulletArea` belongs to.
- Must be set.

---

## 🔗 Related Docs

- Bullet - Base Bullet
- AttackBase - Attack Pattern
- Enemy - Enemy Character
- Creating Battles