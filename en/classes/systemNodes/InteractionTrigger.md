# InteractionTrigger

> 🔴 **Cannot be created by user**
>> This node cannot be created directly by the user; it is instantiated from a scene or used by the system.
>> This node is used by instantiating the `res://Engine/Overworld/Interactions/interaction_trigger.tscn` scene.

---

## Main Roles
- The interaction trigger node defines the parts of the game world where the player can interact.
    - e.g. opening a door, talking to an NPC, picking up an item
- Used in the [`Overworld`](../userNodes/Overworld.md) to detect when [`PlayerOverworld`](PlayerOverworld.md) interacts.

---

## Methods

### `enable()`
Enables the interaction trigger.

### `disable()`
Disables the interaction trigger.

---

## Signals

### `interacted()`
Emitted when the player interacts with the interaction trigger.

---

## 🔗 Related Docs
- [Overworld - Overworld Node](../userNodes/Overworld.md)
- [PlayerOverworld - Overworld Player](PlayerOverworld.md)
- [Creating an Overworld](/tutorials/overworld.md)
