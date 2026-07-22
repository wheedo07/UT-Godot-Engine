# ActInfo

The `ActInfo` class is used to store and manage information about an enemy's ACT options in the game. It stores the name and description of each ACT option and provides the data needed when the player interacts with the enemy.

---

## Properties

### `act`
- **Type**: `String`
- **Description**: The name of this ACT option. For example, it could be `"* Check"`, `"* Talk"`, etc.
- **Default**: `"* Check"`

### `description`
- **Type**: `PackedStringArray`
- **Description**: The description text displayed when this ACT option is selected.
- **Default**: `PackedStringArray(["* ENEMY - ATK 1 DEF 1 \n* ???"])`

---

## Methods

#### `get_act_tr()`
- **Return type**: `String`
- **Description**: Returns the translated string of the current ACT option's name.

---

## 🔗 Related Docs

- [EnemyAct](EnemyAct.md)
- [Enemy - Enemy Character](/classes/userNodes/Enemy.md)
- [Creating Battles](/tutorials/battle.md)
