# AttackMeter

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

A minigame node that appears when the player selects an attack during battle in UT-Godot-Engine.

When configuring the [`BoxSet`](../resource/BoxSet.md) resource inside an [`Encounter`](../resource/Encounter.md) resource, set the scene built with this node to the `meter_scene` property.

---

## Methods

### `get_base_damage() -> int`
Returns the base attack damage by summing the player's attack power and equipment attack bonus.
Call this method when calculating attack damage to retrieve the base damage value.

```gdscript
func _on_hit(): # example function
    var base = get_base_damage()
    var final_damage = base - enemy_def  # apply defense
    damagetarget.emit(final_damage, enemy_id, false)
    queue_free()
```

---

## Member Variables

### Script-only Variables

#### `enemy_id: int`
When the player selects an attack and this node is instantiated, this variable stores the ID of the enemy being targeted.

#### `enemy_def: int`
When the player selects an attack and this node is instantiated, this variable stores the defense value of the enemy being targeted.
The defense value must also be calculated directly within this node.

#### `encounter_script: EncounterScript`
A reference created for efficient control of this node, holding an instance of the [`EncounterScript`](../systemNodes/EncounterScript.md) class.
Through this reference, you can implement additional custom functionality.

---

## Signals

### `damagetarget(damage: int, enemy_id: int, crit: bool)`
A signal emitted when the minigame is successfully completed and the attack lands.
It is recommended to call [`queue_free()`](https://docs.godotengine.org/en/stable/classes/class_node.html#class-node-method-queue-free) after emitting this signal to remove this node.

- `damage: int` — The final damage of the attack. Determined through custom calculation.
- `enemy_id: int` — The ID of the enemy hit by the attack. This can be the same as the value stored in the [`enemy_id`](#enemy_id-int) member variable, or you can specify a different enemy.
- `crit: bool` — Whether the attack is a critical hit. This is passed to the `SlashAnimation` node so that a different animation plays on a critical hit.

```gdscript
func _on_hit(is_crit: bool):
    var damage = get_base_damage() - enemy_def
    damagetarget.emit(damage, enemy_id, is_crit)
    queue_free()
```

### `missed(enemy_id: int)`
A signal emitted when the minigame fails and the attack misses.
Can also be emitted instead of [`damagetarget`](#damagetargetdamage-int-enemy_id-int-crit-bool) for creative use.
It is recommended to call `queue_free()` after emitting this signal to remove this node.

- `enemy_id: int` — The ID of the enemy the attack missed. This can be the same as the value stored in the [`enemy_id`](#enemy_id-int) member variable, or you can specify a different enemy.

```gdscript
func _on_miss():
    missed.emit(enemy_id)
    queue_free()
```

---

## 🔗 Related Docs

- [BoxSet - Battle Board Config](../resource/BoxSet.md)
- [Creating a Custom Meter](/tutorials/custom_meter.md)
