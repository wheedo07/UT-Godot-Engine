# BattleObject

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

The `BattleObject` node is the base class for battle objects created and managed by an [`AttackBase`](AttackBase.md) node in the battle system, similar to the [`Bullet`](Bullet.md) node. <br>
Unlike [`Bullet`](Bullet.md), it has no special built-in functionality and is intended to be subclassed when creating custom battle objects.
This node can also be added to battle via methods like [`add_bullet()`](AttackBase.md#add_bulletbullet-mask).

## Methods

### `fade()`
Fades out the battle object. This function is usually called by [`end_attack()`](AttackBase.md#remove_bullets).

## 🔗 Related Docs

- [Bullet](Bullet.md)
- [AttackBase](AttackBase.md)
- [Creating Battles](/tutorials/battle.md)
