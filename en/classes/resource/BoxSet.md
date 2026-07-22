# BoxSet

The `BoxSet` resource is used by the [`Encounter`](Encounter.md) resource to configure the color, custom meter, or default meter settings of the battle board.

---

## Properties

### `board_color`
- **Type**: `Color`
- **Description**: The background color of the battle board.

### `board_border_color`
- **Type**: `Color`
- **Description**: The border color of the battle board.

### `is_custom_meter`
- **Type**: `bool`
- **Description**: Whether to use a custom meter.  
The default meter is a system where the player times their attacks, but a custom meter allows developers to implement their own meter system.

### `meter_scene`
- **Type**: `Ref<PackedScene>`
- **Description**: The scene to use as the custom meter.  
If `is_custom_meter` is set to `true`, this scene will be used as the meter. The root node must be an [`AttackMeter`](../userNodes/AttackMeter.md) node.

### `meter_texture`
- **Type**: `Ref<Texture>`
- **Description**: The texture to apply to the meter when using the default meter.

### `meter_scale`
- **Type**: `Vector2`
- **Description**: The scale that adjusts the size of the meter when using the default meter.

---

## 🔗 Related Docs
- [Encounter](Encounter.md)
- [BattleBox](/classes/systemNodes/BattleBox.md)
- [AttackMeter](/classes/userNodes/AttackMeter.md)