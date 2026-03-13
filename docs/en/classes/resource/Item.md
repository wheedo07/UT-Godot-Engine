# Item

The `Item` class is a resource used to define items in this engine. Items can have various types.

---

## Properties

### `item_name: string`
The name of the item. This name is used when displaying the item name to the player.

### `item_type: ItemType`
The type of the item. [`ItemType`](#itemtype) is an enum indicating what kind of item it is. For example, weapon, armor, consumable, etc.

### `item_information: PackedStringArray`
The detailed information about the item displayed when the `Info` button is pressed in the `UI`.

### `throw_message: PackedStringArray`
The message displayed when the `Throw` button is pressed in the `UI`.

### `use_message: PackedStringArray`
The message displayed when this item is used.

### `weapon_type: WeaponType`
If this item is a weapon, indicates the type of weapon. [`WeaponType`](#weapontype) is an enum defining the weapon type. For example, knife, gun, frying pan, etc.

### `weapon_speed: float`
If this item is a weapon, indicates the speed of the attack bar. A faster attack bar makes timing more difficult.

### `weapon_bars: int`
If this item is a weapon, indicates the number of attack bars. More attack bars makes timing more difficult.

### `weapon_delay: float`
If this item is a weapon, indicates the gap between attack bars. A shorter gap makes timing more difficult.

### `critical_hits: bool`
If this item is a weapon, indicates whether critical hits are possible. When enabled, the player can deal bonus damage by perfectly timing the attack.

### `heal_amount: int`
If this item is a consumable, indicates the amount of HP restored when the item is used.

### `attack_amount: int`
If this item is a consumable, weapon, or armor, indicates the effect on attack power. Positive values increase attack; negative values decrease it.

### `defense_amount: int`
If this item is a consumable, weapon, or armor, indicates the effect on defense. Positive values increase defense; negative values decrease it.

## Methods

### `get_item_name_tr()`
- **Return type**: `String`
- **Description**: Returns the translated string of the current item's name.

## Enums

### `ItemType`
The `ItemType` enum defines the type of item. Possible values:
- `CONSUMABLE`: Consumable item, e.g. a potion or food.
- `WEAPON`: Weapon item.
- `ARMOR`: Armor item.
- `MISC`: Miscellaneous item, e.g. a quest item or material.

### `WeaponType`
The `WeaponType` enum defines the type of weapon. Each weapon type affects the attack impact in battle. Possible values:
- `KNIFE`: Knife effect
- `PUNCH`: Punch effect
- `SHOE`: Kick effect
- `BOOK`: Book effect
- `PAN`: Frying pan effect
- `GUN`: Gun effect

--- 

## 🔗 Related Docs
- [UI](/classes/systemNodes/UI.md)
- [Global](/classes/systemNodes/Global.md)
