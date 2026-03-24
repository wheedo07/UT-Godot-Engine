# Item

The `Item` class is a resource that defines items in this engine.

---

## Properties

### `item_name: string`
The name of the item. This name is used to display the item name to the player.

### `item_information: PackedStringArray`
The detailed information of the item displayed when the `Info` button is pressed in the `UI`.

### `throw_message: PackedStringArray`
The message displayed when the `Throw` button is pressed in the `UI`.

### `use_message: PackedStringArray`
The message displayed when this item is used.

### `is_consumable: bool`
Whether this item is consumable. If set to `true`, the item is removed from inventory after being used.

## Methods

### `get_item_name_tr()`
- **Return type**: `String`
- **Description**: Returns the translated string of the current item's name.

--- 

## 🔗 Related Docs
- [UI](/classes/systemNodes/UI.md)