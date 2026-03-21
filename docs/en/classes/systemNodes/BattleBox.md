# BattleBox

> 🔴 **Non-user-creatable**
>> This node cannot be created directly by users; it is instantiated from a scene or used by the system.
>> This node is used in the `res://Engine/Battle/Box/BattleBox.tscn` scene.

---

## Main Roles
`BattleBox` is a node that serves as both the space where the player is located during battle and as the player's menu.
It plays a central role in battle, managing the player's selection UI and interactions. It also functions as the player's space in battle, providing various battle effects such as changing the box's size and position, polygon mode, and web line generation.

---

## Methods

### `get_size() -> Vector2`
Returns the current size of the box.

### `get_box_position() -> Vector2`
Returns the current position of the box.

### `get_box_rotation() -> float`
Returns the current rotation value of the box.

### `reset_box(duration: float = 0.5)`
Resets the box's size, position, and rotation to their initial values.
`duration` sets the time it takes to reset.

### `change_size(new_size: Vector2, relative: bool = false, duration: float = 0.6)`
Changes the size of the box.
`new_size` is the new size.
If `relative` is `true`, `new_size` is treated as a relative size added to the current size.
`duration` sets the time it takes for the size to change.

### `change_position(new_position: Vector2, relative: bool = false, duration: float = 0.6)`
Changes the position of the box.
`new_position` is the new position.
If `relative` is `true`, `new_position` is treated as a relative position added to the current position.
`duration` sets the time it takes for the position to change.

### `advanced_change_size(relative_to: BattleBox.RelativePosition, new_position: Vector2, new_size: Vector2, position_relative: bool = false, size_relative: bool = false, duration: float = 0.6)`
An advanced function that changes the position and size of the box simultaneously.
`relative_to` sets the reference point by which position and size are changed.
`new_position` is the new position.
`new_size` is the new size.
If `position_relative` is `true`, `new_position` is treated as a relative position added to the current position.
If `size_relative` is `true`, `new_size` is treated as a relative size added to the current size.
`duration` sets the time it takes for the position and size to change.

### `advanced_set_size(relative_to: BattleBox.RelativePosition, new_position: Vector2, new_size: Vector2, position_relative: bool = false, size_relative: bool = false)`
An advanced function that changes the position and size of the box simultaneously. Unlike [`advanced_change_size()`](#advanced_change_sizerelative_to-battleboxrelativeposition-new_position-vector2-new_size-vector2-position_relative-bool--false-size_relative-bool--false-duration-float--06), this function applies the changes immediately without any transition effect.
`relative_to` sets the reference point by which position and size are changed.
`new_position` is the new position.
`new_size` is the new size.
If `position_relative` is `true`, `new_position` is treated as a relative position added to the current position.
If `size_relative` is `true`, `new_size` is treated as a relative size added to the current size.

### `rotate_by(rot: float, relative: bool = false, duration: float = 0.6)`
Rotates the box.
`rot` is the rotation value (unit: degrees).
If `relative` is `true`, `rot` is treated as a relative rotation added to the current rotation.
`duration` sets the time it takes to rotate.

### `set_box_rotation(rot: float, relative: bool = false)`
Sets the rotation of the box. Unlike [`rotate_by()`](#rotate_byrot-float-relative-bool--false-duration-float--06), this function applies the rotation immediately without any transition effect.
`rot` is the rotation value (unit: degrees).
If `relative` is `true`, `rot` is treated as a relative rotation added to the current rotation.

### `box_show()`
Makes the battle box visible.

### `box_hide()`
Hides the battle box.

### `clear_webs()`
Removes all purple lines (webs) created by [`set_webs()`](#set_websn-int-separation-float---1-margin-int--0). Use this function to clear or remove all purple lines.

### `set_webs(n: int, separation: float = -1, margin: int = 0)`
A function that sets up the purple lines (webs) required before using `SoulBattle.PURPLE` mode.
Use this function to adjust the number, spacing, and margin of the purple lines.

`n` is the number of purple lines. Setting it to `0` prevents any purple lines from being created.
`separation` is the spacing between purple lines. Setting it to a negative value causes the spacing to be adjusted automatically.
`margin` is the margin between the purple lines and the battle box. Setting it to a positive value adds a margin.

### `get_web_y_pos(id: int) -> float`
Returns the y coordinate of a purple line (web) created by [`set_webs()`](#set_websn-int-separation-float---1-margin-int--0). `id` is the index of the purple line, starting from `0`.
For example, if `id` is `0`, the y coordinate of the first purple line is returned.

### `blitter_print(texts: PackedStringArray)`
This function is used to display text during the player's turn, not the enemy's turn. `texts` is an array of texts to display.

### `polygon_enable()`
Enables polygon mode for the battle box. In polygon mode, the battle box can take on a polygonal shape.

### `polygon_disable(box_size: Vector2, duration: float = 0.3)`
Disables polygon mode for the battle box and restores it to its original rectangular shape.
`box_size` is used to set the size of the battle box when polygon mode is disabled.
`duration` is used to set the time it takes to disable polygon mode.

### `polygon_is_enabled() -> bool`
Returns whether polygon mode is enabled for the battle box. Returns `true` if polygon mode is enabled, `false` otherwise.

### `create_protrusion(direction: Vector2, offset: float, size: Vector2, duration: float = 0.3)`
A function that creates a protrusion from the battle box. A protrusion is a box-shaped part that sticks out from the battle box.
This function can be used after enabling polygon mode with [`polygon_enable()`](#polygon_enable).

`direction` is used to set which face the protrusion extends from. For example, `(1, 0)` creates a protrusion from the right face, and `(0, -1)` from the top face.
`offset` is used to set the position of the protrusion.
`size` is used to set the size of the protrusion.
`duration` is used to set the time it takes to create the protrusion.

### `get_polygon_points() -> PackedVector2Array`
Returns all points of the current polygon in polygon mode.

### `get_vertex_position(vertex_index: int) -> Vector2`
Returns the position of the vertex at the specified index in polygon mode.

### `move_closest_point(target_point: Vector2, duration: float = 0.3) -> int`
Finds the vertex closest to the target point and moves it there.
This function can be used after enabling polygon mode with [`polygon_enable()`](#polygon_enable).

`target_point` is the target point to move the vertex to.
`duration` is used to set the time it takes for the vertex to move.
Returns the index of the moved vertex.

### `move_point_by_index(vertex_index: int, target_point: Vector2, duration: float = 0.3)`
Moves the vertex at the specified index to the target point.
This function can be used after enabling polygon mode with [`polygon_enable()`](#polygon_enable).

`vertex_index` is the index of the vertex to move.
`target_point` is the target point to move the vertex to.
`duration` is used to set the time it takes for the vertex to move.

### `move_point_by_offset(from_point: Vector2, offset: Vector2, duration: float = 0.3) -> int`
Finds the vertex closest to the specified point and moves it by the given offset.
This function can be used after enabling polygon mode with [`polygon_enable()`](#polygon_enable).

`from_point` is the reference point used to find the vertex to move.
`offset` is the offset by which to move the vertex.
`duration` is used to set the time it takes for the vertex to move.
Returns the index of the moved vertex.

### `move_multiple_points(vertex_indices: PackedInt64Array, target_points: PackedVector2Array, duration: float = 0.3)`
Moves multiple specified vertices to their target points.
This function can be used after enabling polygon mode with [`polygon_enable()`](#polygon_enable).

`vertex_indices` is an array of indices of the vertices to move.
`target_points` is an array of target points for each vertex. The order must match `vertex_indices`.
`duration` is used to set the time it takes for the vertices to move.

### `stop_all_point_tweens()`
Immediately stops the tweens of all vertices currently moving in polygon mode. Use this function to forcibly stop all vertex movements.

---

## Properties

### `wintext: String` (default: `"UT_VICTORY"`)
Sets the text that appears when the battle is won.

### `TweenEase: Tween.EaseType` (default: `1`)
Sets the easing type applied to box transition functions.

### `TweenTrans: Tween.TransitionType` (default: `4`)
Sets the transition type applied to box transition functions.

### `morph_speed: float` (default: `200.0`)
Sets the speed at which vertices move in polygon mode. The unit is pixels per second.

### `polygon_point_count: int` (default: `120`)
Sets the number of vertices in polygon mode. A higher vertex count produces a smoother polygon shape, but may impact performance.

---

## Signals

### `moved_to_buttons`
Emitted when the player moves to the button selection state during the player's turn.
> **This signal is used internally by the engine.**

### `move_soul(newpos: Vector2)`
Emitted when the soul moves during the player's turn. `newpos` is the position the soul will move to.
> **This signal is used internally by the engine.**

### `exit_menu`
Emitted when the player exits the menu during the player's turn.
> **This signal is used internally by the engine.**

### `act(target: int, option: int)`
Emitted when a specific action is selected from the ACT menu during the player's turn. `target` is the index of the target for the selected action. `option` is the option index of the selected action.
> **This signal is used internally by the engine.**

### `fight(target: int)`
Emitted when a specific target is selected from the FIGHT menu during the player's turn. `target` is the index of the selected target.
> **This signal is used internally by the engine.**

### `item(item_choice: int)`
Emitted when a specific item is selected from the ITEM menu during the player's turn. `item_choice` is the index of the selected item.
> **This signal is used internally by the engine.**

### `mercy(target: int)`
Emitted when a specific option is selected from the MERCY menu during the player's turn. If `target` is `0`, "Mercy" was chosen; if `1`, "Flee" was chosen.
> **This signal is used internally by the engine.**

### `tween_finished`
Emitted when the box transformation animation is complete.

### `blitter_end`
Emitted when all text printed by the [`blitter_print()`](#blitter_printtexts-packedstringarray) method has finished displaying.

---

## 🔗 Related Docs

- [Enemy - Enemy Node](/classes/userNodes/Enemy.md)
- [AttackBase - Attack Pattern](/classes/userNodes/AttackBase.md)
- [Bullet - Base Bullet](/classes/userNodes/Bullet.md)
- [Creating Battles](/tutorials/battle.md)