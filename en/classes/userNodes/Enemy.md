# Enemy

> 🟢 **User-creatable**
>> Users can create this node and write scripts for it.

The core class for implementing battle enemies in UT-Godot-Engine.
Handles all Undertale-style enemy behavior including turn-based combat, dialogue, animation, and state management.

---

## Main Roles
- Implement turn-based battle system
    - Handle enemy turn start and end
    - Manage and execute attack patterns ([AttackBase](AttackBase.md))
- Stat and state management
    - Manage stats like HP and defense
    - State change and reward system
- Virtual function system
    - Provides virtual functions that respond to player actions

---


## Required Node Structure
```
Enemy
├── sprites            # Enemy sprite node
├── Spare              # Spare effect node (res://Engine/Battle/Enemies/spare.tscn)
├── DustTransition     # Death effect node (res://Engine/Battle/Enemies/dust_transition.tscn)
├── Dialogue           # Dialogue box node (res://Engine/Battle/Misc/dialogue.tscn)
└── AudioStreamPlayer  # Sound player node for hit sounds
```

---

## Methods
Enemy provides virtual functions that respond to player actions.
The `on_*` functions must each emit their respective signal to proceed to the next action.

### `ready()` (virtual)
Enemy initialization function.

```gdscript
extends Enemy

func ready():
    pass
```

### `_on_get_turn()` (virtual)
Called when the enemy's turn begins.

```gdscript
func _on_get_turn():
    # Resize box
    box.change_size(Vector2(350, 140), false, 0.3)
    await get_tree().create_timer(0.4).timeout
    
    # Execute attack pattern
    var attack = attacks.add_attack(preload("res://attacks/basic_bullets.tscn"))
    attack.start_attacks()
```

### `_on_end_turn()` (virtual)
Called when the enemy's turn ends.

```gdscript
func _on_end_turn():
    $sprites.play("idle")
```

### `on_fight(miss: bool)` (virtual)
Called when the player selects FIGHT.

```gdscript
func on_fight(miss: bool):
    if miss:
        play_dialogue(1)  # "You can't hit me like that"
    else:
        play_dialogue(0)  # "Oh, you hit me!"
    await finished_dialogue
    emit_signal("on_fight_end")  # Required!
```

### `on_act(option: int)` (virtual)
Called when the player selects ACT.

```gdscript
func on_act(option: int):
    match option:
        0:
            box.blitter_print([ "* You check the enemy." ])
        1:
            box.blitter_print([ "* You tease the enemy." ])
            change_act(1)   # Change state
        2:
            box.blitter_print([ "* You share a joke." ])
            change_act(2)   # Change state
    await box.blitter_end
    emit_signal("on_act_end")  # Required!
```

### `on_item(option: int)` (virtual)
Called when the player uses an ITEM.

```gdscript
func on_item(option: int):
    # Check the used item from inventory
    var used_item = global.items[option]
    emit_signal("on_item_end")  # Required!
```

### `on_mercy()` (virtual)
Called when the player selects MERCY.

```gdscript
func on_mercy():
    if can_spare():  # Check spare condition
        play_dialogue(6)  # "Alright, I'll accept your mercy!"
        await finished_dialogue
        main.spare_enemy(enemy_id)
    else:
        play_dialogue(7)  # "Sorry, but I can't do that."
        await finished_dialogue
    emit_signal("on_mercy_end")  # Required!

func can_spare() -> bool:
    # Spare condition logic (e.g. after performing certain ACTs)
    return current_act >= 2
```

### `on_defeat(death: bool)` (virtual)
Called when the enemy is defeated.

```gdscript
func on_defeat(death: bool):
    if death:
        # Death handling
        play_dialogue(8)  # "Ugh... I lost..."
        await finished_dialogue
    else:
        # Spare handling
        play_dialogue(9)  # "You're a good person."
        await finished_dialogue
    
    emit_signal("on_defeat_end")  # Required! (particles play automatically on call)
```

### `damage_info(amount: int)` (virtual)
Called when the enemy takes damage.
You can use the `[time=X]` tag to control how long the damage message is displayed.

```gdscript
func damage_info(amount: int) -> String:
    # Return a custom damage message
    if amount >= 50:
        return "[time=0.5]CRITICAL HIT!"
    elif amount <= 10:
        return "HIT"
    else:
        return ""  # Use default message
```

### `handle_victory()` (virtual)
Called when the enemy wins.
The return value can be a message shown when the player is defeated.
```gdscript
func handle_victory():
    return "I can't stop here..."  # Defeat message
```

### `change_act(act_index: int)`
Changes the enemy's ACT.
`act_index` is an index in the [`enemy_acts`](#enemy_acts-arrayenemyact) array.

```gdscript
change_act(1)  # Change to ACT 1
```

### `play_dialogue(index: int, duration: float = 0, skip: bool = true, keep_expression: bool = false)` :id=play_dialogue-1
Shows the enemy's dialogue box and plays a dialogue.
`index` is the index in the dialogue resource.

```gdscript
play_dialogue(2)  # Play dialogue at index 2
await finished_dialogue  # Wait until dialogue is done
play_dialogue(0, 2.0, false, true)  # Play index 0, auto-close after 2s, no skip, keep expression
```

### `play_set_dialogue(dialogue: Dialogues, duration: float = 0, skip: bool = true, keep_expression: bool = false)` :id=play_set_dialogue-1
Shows the enemy's dialogue box and plays a dialogue resource.

```gdscript
var custom_dialogue = Dialogues.new().from([
    "Custom enemy dialogue 1",
    "Custom enemy dialogue 2"
])
play_set_dialogue(custom_dialogue)
await finished_dialogue
```

### `modify_stats(value: Dictionary)`
Modifies the enemy's stats.

```gdscript
modify_stats({"hp": stats.hp - 20})  # Decrease HP by 20
modify_stats({"def": stats.def + 5}) # Increase defense by 5
```

### `get_sprites() -> Node`
Returns the node at the specified [`sprites_path`].

```gdscript
var sprites_node = get_sprites()
```

### `get_solo() -> bool`
Returns whether the current battle is a solo battle.

```gdscript
var is_solo = get_solo()
print("Is alone: ", is_solo)
```

---

## Member Variables

### Editor-configurable Variables

#### `enemy_name: String`
The enemy's name.

#### `kr: bool`
Whether the enemy inflicts KR (Karma) on the player. <br>
If `true`, the player's HP continuously decreases when hit by the enemy's attacks.

#### `isFirstTurn: bool`
Whether the first turn belongs to the enemy at the start of battle. <br>
If `true`, the first turn starts as the enemy's turn.

#### `dodging: bool`
Whether the enemy dodges. <br>
If `true`, the enemy dodges the player's attacks.

#### `stats: Dictionary`
The enemy's stats. <br>
Default: `{"hp": 100, "max_hp": 100, "def": 0}`

#### `rewards: Dictionary`
The rewards the enemy gives the player upon defeat. <br>
Default: `{"exp": 10, "gold": 10}`

#### `dialogues: DialogueAsset`
The enemy's dialogue resource. <br>
You can set the enemy's lines per dialogue index.

#### `current_act: int`
The current ACT index of the enemy. <br>
Points to an index in the [`enemy_acts`](#enemy_acts-arrayenemyact) array.

#### `enemy_acts: Array<EnemyAct>`
The enemy's ACT array. <br>
Each ACT is an [`EnemyAct`](../resource/EnemyAct.md) resource.

#### `one_by_one_override: bool`
Whether the enemy executes override one by one when the ACT changes. <br>
e.g. When changing from ACT 0 to ACT 1:
ACT 0 ("Check")
ACT 1 ("Tease", "Joke")
With `one_by_one_override` set to `true`, the result is ("Check", "Joke").

#### `hurt_sound_path: NodePath<AudioStreamPlayer>`
The path to the `AudioStreamPlayer` node played when the enemy is hit.

#### `dust_path: NodePath<DustTransition>`
The path to the [`DustTransition`](DustTransition.md) node played when the enemy dies.

#### `spare_path: NodePath<GPUParticles2D>`
The path to the `GPUParticles2D` node played when the enemy is spared.

#### `sprites_path: NodePath<Node>`
The path to the enemy's sprite node. <br>
Can be any node type: `Node2D`, `AnimatedSprite2D`, etc.

#### `dialogue_path: NodePath<DialogueControl>`
The path to the enemy's dialogue box node. <br>
Must be a [`DialogueControl`](../systemNodes/DialogueControl.md) node.

#### `expression_sprites: Array<AnimatedSprite2D>` :id=expression_sprites-arrayanimatedsprite2d
An array of expression sprite nodes for the enemy. <br>
Index order: [0]=body, [1]=head, etc. Remembering the index allows [changing expressions during dialogue.](#expression-control)

### Script-only Variables
| Property | Type | Description |
|----------|------|-------------|
| `enemy_id` | `int` | Enemy ID |
| `main` | `BattleMain` | Battle main node |
| `box` | `BattleBox` | Battle box (soul area) |
| `soul` | `SoulBattle` | Player soul |
| `camera` | `CameraController` | Camera controller |
| `dialogue` | `DialogueControl` | Dialogue box node |
| `attacks` | `AttackManager` | Attack manager |
| `bg` | `TextureRect` | Battle background node |
| `encounter_script` | `EncounterScript` | Encounter script node |

---

## Signals

### `finished_dialogue`
- Dialogue complete signal
- Emitted when dialogue started by [`play_dialogue()`](#play_dialogueindex-int-duration-float--0-skip-bool--true-keep_expression-bool--false) or [`play_set_dialogue()`](#play_set_dialoguedialogue-dialogues-duration-float--0-skip-bool--true-keep_expression-bool--false) is finished.

### `changed_act`
- ACT change signal
- Emitted when ACT is changed via [`change_act()`](#change_actact_index-int).

### `dodged(to_right: bool)`
- Signal emitted when the enemy dodges the player's attack.
- `to_right` indicates whether the enemy dodged to the right.

```gdscript
func _on_dodged(to_right: bool):
    if to_right:
        pass  # Handle dodge-right animation
    else:
        pass  # Handle dodge-left animation
```

### `hit(damage: int)`
- Signal emitted when the enemy is hit by the player's attack.
- `damage` is the amount of damage the enemy took.

```gdscript
func _on_hit(damage: int):
    print("Enemy took ", damage, " damage!")
```

### `on_fight_end`
- FIGHT handling complete signal
- Must be emitted in the [`on_fight()`](#on_fightmiss-bool-virtual) virtual function.

### `on_act_end`
- ACT handling complete signal
- Must be emitted in the [`on_act()`](#on_actoption-int-virtual) virtual function.

### `on_item_end`
- ITEM handling complete signal
- Must be emitted in the [`on_item()`](#on_itemoption-int-virtual) virtual function.

### `on_mercy_end`
- MERCY handling complete signal
- Must be emitted in the [`on_mercy()`](#on_mercy-virtual) virtual function.

### `on_defeat_end`
- Defeat handling complete signal
- Must be emitted in the [`on_defeat()`](#on_defeatdeath-bool-virtual) virtual function.

---

## Stats and State Management

### Stats System

```gdscript
# Default stat structure
stats = {
    "hp": 100,        # Current HP
    "max_hp": 100,    # Maximum HP
    "def": 10         # Defense
}

# Modifying stats
func buff_defense():
    modify_stats({"def": stats.def + 5})
    
func heal_self():
    var new_hp = min(stats.hp + 30, stats.max_hp)
    modify_stats({"hp": new_hp})
```

### Reward System

```gdscript
# Default reward structure
rewards = {
    "exp": 20,        # Experience
    "gold": 15        # Gold
}
```

## Animation and Expression System

### Expression Control

```gdscript
# Set expression sprites
expression_sprites = [
    get_node("Sprites/Body"),   # [0]: body
    get_node("Sprites/Head")    # [1]: head (expression)
]

# Change expression during dialogue
play_set_dialogue(Dialogues.new().from([
    "Angry",
    "Happy",
    "Sad"
]).set_expressions([1, [0, 1], 2])) # Specify expression index (2D array is [body, head] order)
```

---

## Notes & Tips

### Recommendations

1. **Always emit signals**
   ```gdscript
   func on_fight_used(miss: bool):
       # Processing logic...
       emit_signal("on_fight_end")  # Required!
   ```

2. **Wait for dialogue completion**
   ```gdscript
   play_dialogue(0)
   await finished_dialogue  # Wait until dialogue is done
   ```

### Things to Avoid

1. **Missing signal emission**
   ```gdscript
   func on_act_used(option: int):
       # Processes but forgets to emit signal → game freezes!
   ```

2. **Wrong node path**
   ```gdscript
   sprites_path = NodePath("WrongPath")  # Non-existent path
   ```

---

## 🔗 Related Docs

- [AttackBase - Attack Pattern](/classes/userNodes/AttackBase.md)
- [Bullet - Base Bullet](/classes/userNodes/Bullet.md)
- [Dialogues - Dialogue Resource](/classes/resource/Dialogues.md)
- [EnemyAct - Enemy State](/classes/resource/EnemyAct.md)
- [Creating Battles](/tutorials/battle.md)
