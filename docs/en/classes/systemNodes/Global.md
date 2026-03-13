# Global

> 🔴 **Non-user-creatable**
>> This node cannot be created directly by users; it is instantiated from a scene or used by the system.
>> This node is used in the `res://Core/autoload/global.tscn` scene.

---

## Main Roles
The `Global` node is used in the `(res://Core/autoload/global.tscn)` scene and manages the global settings and functionality of UT-Godot-Engine. It provides globally accessible variables and methods throughout the game, as well as a variety of utility functions.

---

## Methods

### `check_level_up() -> bool`
Checks whether the player's experience meets the level-up condition, and processes the level-up if so.

### `disable_input(key: String)`
Disables a specific input key. The `key` parameter is the name of the input key to disable.<br>
ex) "ut_confirm", "ut_cancel"

### `enable_input(key: String)`
Enables a specific input key. The `key` parameter is the name of the input key to enable.<br>
ex) "ut_confirm", "ut_cancel"

### `has_input_disabled(key: String) -> bool`
Checks whether a specific input key is disabled. The `key` parameter is the name of the input key to check.
Returns `true` if disabled, `false` otherwise.<br>
ex) "ut_confirm", "ut_cancel"

### `alert(message: String, title: String = "Alert")`
Displays an alert message window in the game. The `message` parameter is the content to display, and the `title` parameter is the window title. The title defaults to `"Alert"`.

### `save_file(slot: int, save_data: Dictionary)`
Saves game data to the specified slot.
Save file names follow the format `file0~file8`, and the save location is determined by the `saveDir` parameter.<br>
`slot` is the save slot number (0–8), and `save_data` is a dictionary containing the data to save.<br>
The `saved` signal is emitted when saving is complete.

### `load_file(slot: int) -> Dictionary`
Loads game data from the specified slot.
Save file names follow the format `file0~file8`, and the load location is determined by the `saveDir` parameter.<br>
`slot` is the save slot number to load (0–8). The loaded data is returned as a dictionary.

### `exists_file(slot: int) -> bool`
Checks whether a file exists in the specified slot. `slot` is the save slot number to check (0–8).
Returns `true` if the file exists, `false` otherwise.

### `change_setting(setting_name: String, value: Variant)`
Changes a game setting value.<br>
`setting_name` is the name of the setting to change (one of: `"Master"`, `"Music"`, `"SFX"`, `"border"`, `"shake"`, `"vfx"`, `"locale"`).<br>
`value` is the value to set (`"Master"`, `"Music"`, `"SFX"` → float; `"border"`, `"shake"`, `"vfx"` → bool; `"locale"` → String).<br>
Changes are immediately reflected in the related systems.

### `get_Music() -> AudioStreamPlayer`
Returns the currently playing background music (`AudioStreamPlayer`) node.

### `get_battle_text_box() -> bool`
Returns whether the text box is active during battle. Returns `true` if active, `false` otherwise.

### `get_frist() -> bool`
Returns whether the game is being launched for the first time. Returns `true` if it is the first launch, `false` otherwise.

### `get_fullscreen() -> bool`
Returns whether the game is in fullscreen mode. Returns `true` if fullscreen, `false` otherwise.

### `get_player_in_menu() -> bool`
Returns whether the player is in a menu state. Returns `true` if in menu, `false` otherwise.

### `get_player_kr() -> int`
Returns the player's current KR (Karma) value.

### `get_player_position() -> Vector2`
Returns the player's current position as a `Vector2`.

### `get_player_text_box() -> bool`
Returns whether the player is in a text box state. Returns `true` if in text box state, `false` otherwise.

### `get_scene_container() -> SceneContainer`
Returns the currently active [`SceneContainer`](SceneContainer.md) node. The scene container manages the various scenes in the game.

### `set_flag(key: String, value: Variant)`
Sets a flag value for the specified key. `key` is the name of the flag, and `value` is the value to set.
Flags are stored in save data and are used to track game progress.

### `get_flag(key: String, defaultValue: Variant = false) -> Variant`
Gets the flag value for the specified key. `key` is the name of the flag, and `defaultValue` is the value to return if the flag does not exist.
Returns the flag's value if it exists, otherwise returns the default value.

### `set_g_flags(key: String, value: Variant)`
Sets a global flag value for the specified key. `key` is the name of the flag, and `value` is the value to set.
Global flags are stored in `undertale.ini` and are **not** reset when [`resetgame()`](#resetgame) is called.

### `get_g_flags(key: String, defaultValue: Variant = false) -> Variant`
Gets a global flag value for the specified key. `key` is the name of the flag, and `defaultValue` is the value to return if the flag does not exist.
Returns the flag's value if it exists, otherwise returns the default value.

### `save_flag(key: String, value: Variant)`
Sets and immediately saves a flag value for the specified key. `key` is the name of the flag, and `value` is the value to set.<br>
Similar to [`set_flag()`](#set_flagkey-string-value-variant), but unlike that method (which only saves when the player saves), this function saves the flag immediately.<br>
ex) When Sans needs to remember an action even if the player never saved.

### `heal(amount: int)`
Restores the player's HP by the specified amount. `amount` is the HP value to restore.

### `isMobile() -> bool`
Returns whether the current platform is mobile. Returns `true` on mobile platforms, `false` otherwise.

### `save_settings()`
Saves the current game settings, including audio volume, graphics settings, and control settings.

### `toggle_fullscreen()`
Toggles the game's fullscreen mode. Disables fullscreen if currently enabled, and enables it if currently disabled.

### `save(room_name: String = "")`
Automatically saves the current game state. `room_name` is the name of the room to save; defaults to an empty string.<br>
If empty, the current node's name is used.<br>
Use this when you want to manually create a save point.

### `resetgame()`
Resets the game's flags and game data. Used to restart the game from the beginning. Global flags are **not** reset.

### `true_resetgame()`
Completely resets all game data. Used for a full fresh start. All flags, global flags, and save slots are reset.

---

## Member Variables

### Editor-configurable Variables

#### `item_list: Array[Item]`
A list of all items in the game. Each element is referenced by its index, which corresponds to the item ID.

#### `items: Array[int]`
A list of item IDs the player currently holds. IDs refer to indices in [`item_list`](#item_list-arrayitem).

#### `boxitems: Array[int]`
A list of item IDs the player holds in the dimensional box. IDs refer to indices in [`item_list`](#item_list-arrayitem).

#### `equipment: Dictionary`
A dictionary containing the player's currently equipped gear.<br>
Keys are equipment slot names (e.g. `"weapon"`, `"armor"`) and values are the IDs of the equipped items. IDs refer to indices in [`item_list`](#item_list-arrayitem).

#### `cells: PackedStringArray`
A list of phone contacts the player can call from the [`Overworld`](../userNodes/Overworld.md).<br>
Each element is the contact title, and the index is used in the [`start_cellphone()`](../userNodes/Overworld.md#start_cellphoneid-int-virtual) method.

#### `flags: Dictionary`
A dictionary storing flags used to track game progress.<br>
Keys are flag names and values are the corresponding flag values. It is recommended to modify this in the editor and access it via methods like [`set_flag()`](#set_flagkey-string-value-variant) and [`get_flag()`](#get_flagkey-string-defaultvalue-variant--false---variant) in scripts.

#### `player_name: String`
The player's name.

#### `player_attack: int`
The player's attack power. Affects the damage dealt to enemies in battle.

#### `player_defense: int`
The player's defense. Affects the amount of damage reduction in battle.

#### `player_gold: int`
The amount of gold the player currently holds. Gold is used to purchase items in in-game shops.

#### `player_hp: int`
The player's current HP.

#### `player_max_hp: int`
The player's maximum HP.

#### `player_kills: int`
The number of monsters the player has defeated so far.<br>
If the `"isGenocide": true` flag is set in the [`flags`](#flags-dictionary) variable, this value is also shown in the [`UI`](UI.md).

#### `player_exp: int`
The amount of EXecution Points (EXP) the player currently holds. Use [`check_level_up()`](#check_level_up---bool) to check EXP and handle level-ups.

#### `player_lv: int`
The player's current LOVE (Level Of ViolencE) level.

#### `saveDir: String`
The directory path where save files are stored. Defaults to `"user://"`.
Save files are stored in this directory in the format `file0~file8`.<br>
On Windows, reserved keywords can be used in addition to the `user://` path:

| Keyword | Description | Actual Path |
|---------|-------------|-------------|
| `$home` | User home directory | Windows: `C:\Users\Username` |
| `$appdata` | Application data directory | Windows: `C:\Users\Username\AppData\Roaming` |
| `$localappdata` | Local application data directory | Windows: `C:\Users\Username\AppData\Local` |

Example: `"$appdata/MyGame"`

>> ⚠️ **Note**: Reserved keywords are not yet supported on platforms other than Windows.

#### `settings: Dictionary`
A dictionary storing the game's setting values.
```json
{
    "Master": float,  // Master volume (0.0 ~ 1.0)
    "Music": float,   // Music volume (0.0 ~ 1.0)
    "SFX": float,     // Sound effects volume (0.0 ~ 1.0)
    "border": bool,   // Whether to show window border
    "shake": bool,    // Whether to use screen shake
    "vfx": bool,      // Whether to use visual effects
    "locale": String  // Language setting ("auto", "en", "ko")
}
```

#### `scan_directory: String`
The directory path for file scanning. Used in debug mode.<br>
When the debug menu is opened with the F1 key, files in this directory are automatically added to the dropdown menu.<br>
Primarily used during development to quickly load scene files.

>> ⚠️ **Note**: The `res://Core`, `res://Engine`, and `res://` directories are not scanned.

### Script-only Variables

#### `variables: Dictionary`
A dictionary for storing global variables.<br>
This is a free-form variable store with no enforced rules, accessible globally from any script.

#### `overworld_data: Dictionary`
A dictionary storing data related to the [`Overworld`](../userNodes/Overworld.md) scene.<br>
Automatically managed by the engine — direct modification is not recommended.
```json
{
    "room_name": String,  // Current room name
    "room_pos": Vector2,  // Player position within the current room
    "room": String        // Current room scene path
}
```

#### `player_can_move: bool`
Indicates whether the player is currently able to move.<br>
Automatically managed by the engine. To manually prevent movement, use the [`player_move`](#player_move-bool) variable instead.

#### `player_move: bool`
Controls whether the player can move.<br>
Set this to `false` to prevent the player from moving.

#### `player_set_menu: bool`
Indicates whether the player is in a menu state.<br>
Set this to `true` to put the player in a menu state.

---

## Signals

### `fullscreen_toggled(to: bool)`
Emitted when fullscreen mode is toggled. `to` indicates whether fullscreen is now enabled (`true`) or disabled (`false`).

### `saved`
Emitted when game data is saved.

---

## 🔗 Related Docs
- [Overworld - Overworld Node](../userNodes/Overworld.md)
- [SceneContainer - Game Scene Container](SceneContainer.md)
- [UI - Overworld UI](UI.md)