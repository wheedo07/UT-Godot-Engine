# Intro

> 🔴 **Cannot be created by user**
>> This node cannot be created directly by the user; it is instantiated from a scene or used by the system.
>> This node is used in the `res://Core/Startup/intro.tscn` scene.

---

## Main Roles
- Plays the intro sequence when the game starts.
- Loads intro data from a JSON file and displays images and text sequentially.

---

## JSON Data Format

```json
[
    {
        "image_path": "res://Intro/intro_1.png",
        "text": {
            "ko": "오래 옛날, 두 종족이 지구를 다스렸습니다...",
            "en": "Long ago, two races ruled over Earth...",
            "ja": "むかしむかし、ふたつの種族が地球を支配していた..."
        },
        "speed": {
            "ko": 0.05,
            "en": 0.03,
            "ja": 0.04
        },
        "duration": 3.0
    },
    {
        "image_path": "res://Intro/intro_2.png",
        "text": {
            "ko": "인간과 몬스터입니다.",
            "en": "HUMANS and MONSTERS.",
            "ja": "人間とモンスター。"
        },
        "speed": {
            "ko": 0.05,
            "en": 0.03,
            "ja": 0.04
        },
        "duration": 3.0
    }
]
```

| Field | Description |
|-------|-------------|
| `image_path` | Path to the image displayed at that step |
| `text` | Dialogue text per language code |
| `speed` | Typing speed per language code (seconds/character) |
| `duration` | Wait time before advancing to the next step when using `enable_auto` (seconds) |

---

## Methods

### `ready()` *(virtual)*
A virtual method that can be overridden in a derived class to implement **game-specific** initialization logic.

### `is_intro_completed() → bool` *(const)*
Returns whether the intro has been completed.
Returns `true` when all intro sequences have finished or the user has skipped the intro.

### `next()`
Advances to the next step of the intro sequence.
Can only be called manually to proceed to the next intro step when [`enable_auto`](#enable_auto-bool-default-false) is set to `false`.

---

## Member Variables

### `intro_completed_path: String` (default: `""`)
The file path of the scene to transition to after the intro is complete.
If not set, the current scene remains active after the intro completes.

### `intro_json_path: String` (default: `"res://Intro/intro_data.json"`)
The path to the JSON file where the intro sequence data is defined.
This file must contain the image, text, and timing information for each intro step.

### `music: AudioStream`
The background music to be played during the intro.

### `enable_auto: bool` (default: `false`)
Sets whether the intro sequence advances automatically.
When set to `true`, each intro step automatically proceeds after the specified `duration`.
When set to `false`, [`next()`](#next) must be called manually to advance to the next step.

---

## Signals

### `intro_completed(skipped: bool)`
Emitted when the intro sequence is complete.
`skipped` indicates whether the user skipped the intro.

### `started_intro(index: int)`
Emitted when each step of the intro sequence begins.
`index` is the index of the current intro step.

---

## 🔗 Related Docs
- [Scene Transition System](scene_changer.md)
- [Project Structure](/tutorials/project-structure.md)