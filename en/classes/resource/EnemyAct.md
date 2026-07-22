# EnemyAct

The `EnemyAct` class is used to define the ACT options of an [enemy character](../userNodes/Enemy.md) in the game. It contains the various ACT options that the enemy can interact with and manages the details of each option as `ActInfo` objects.

---

## Properties

### `acts`
- **Type**: `Array<ActInfo>`
- **Description**: An array of ACT options available to this enemy. Each ACT option is represented as an `ActInfo` object.

### `sparable`
- **Type**: `bool`
- **Description**: Indicates whether this enemy can be spared by the player. If set to `true`, the player can select MERCY on this enemy and the system will handle it automatically.
- **Default**: `true`

---

## 🔗 Related Docs

- [ActInfo](ActInfo.md)
- [Enemy - Enemy Character](/classes/userNodes/Enemy.md)
- [Creating Battles](/tutorials/battle.md)
