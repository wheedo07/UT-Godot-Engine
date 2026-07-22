# Enemy

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

UT-Godot-Engine에서 전투 시스템의 적을 구현하는 핵심 클래스입니다.
턴 기반 전투, 대화, 애니메이션, 상태 관리 등 Undertale 스타일의 적 행동을 모두 처리합니다.

---

## 주요 역할
- 턴 기반 전투 시스템 구현
    - 적의 턴 시작 및 종료 처리
    - 공격 패턴([AttackBase](AttackBase.md)) 관리 및 실행
- 능력치 및 상태 관리
    - HP, 방어력 등 능력치 관리
    - 상태 변화 및 보상 시스템
- 가상 함수 시스템 제공
    - 플레이어 행동에 반응하는 가상 함수들 제공

---


## 필수 노드 구조
```
Enemy
├── sprites # 적 스프라이트 노드
├── Spare   # 자비 효과 노드 (res://Engine/Battle/Enemies/spare.tscn)
├── DustTransition # 사망 효과 노드 (res://Engine/Battle/Enemies/dust_transition.tscn)
├── Dialogue # 대화 박스 노드 (res://Engine/Battle/Misc/dialogue.tscn)
└── AudioStreamPlayer # 타격시 소리 재생 노드
```

---

## 메서드
Enemy는 플레이어의 행동에 반응하는 가상 함수들을 제공합니다.
on_* 함수들은 꼭 각각의 시그널을 발생시켜 다음 행동으로 넘어가도록 해야 합니다.

### `ready()` (virtual)
적 초기화 함수입니다.

```gdscript
extends Enemy

func ready():
    pass;
```

### `_on_get_turn()` (virtual)
적의 턴이 시작될 때 호출됩니다.

```gdscript
func _on_get_turn():
    # 박스 크기 조정
    box.change_size(Vector2(350, 140), false, 0.3)
    await get_tree().create_timer(0.4).timeout
    
    # 공격 패턴 실행
    var attack = attacks.add_attack(preload("res://attacks/basic_bullets.tscn"))
    attack.start_attacks()
```

### `_on_end_turn()` (virtual)
적의 턴이 종료될 때 호출됩니다.

```gdscript
func _on_end_turn():
    $sprites.play("idle")
```

### `on_fight(miss: bool)` (virtual)
플레이어가 FIGHT를 선택했을 때 호출됩니다.

```gdscript
func on_fight(miss: bool):
    if miss:
        play_dialogue(1)  # "그정도로는 날 맞출 수 없다고"
    else:
        play_dialogue(0)  # "오 날 맞췄군!"
    await finished_dialogue
    emit_signal("on_fight_end")  # 필수!
```

### `on_act(option: int)` (virtual)
플레이어가 ACT를 선택했을 때 호출됩니다.

```gdscript
func on_act(option: int):
    match option:
        0:
            box.blitter_print([ "* 당신은 적을 관찰했다." ]);
        1:
            box.blitter_print([ "* 당신은 적을 도발했다." ]);
            change_act(1)   # 상태 변경
        2:
            box.blitter_print([ "* 당신은 적과 농담을 주고받았다." ]);
            change_act(2)   # 상태 변경
    await box.blitter_end;
    emit_signal("on_act_end")  # 필수!
```

### `on_item(option: int)` (virtual)
플레이어가 ITEM을 사용했을 때 호출됩니다.

```gdscript
func on_item(option: int):
    # 인벤토리에서 사용된 아이템 정보 확인
    var used_item = global.items[option]
    emit_signal("on_item_end")  # 필수!
```

### `on_mercy()` (virtual)
플레이어가 MERCY를 선택했을 때 호출됩니다.

```gdscript
func on_mercy():
    if can_spare():  # 자비 조건 확인
        play_dialogue(6)  # "좋아 자비를 받아들이마!"
        await finished_dialogue
        main.spare_enemy(enemy_id)
    else:
        play_dialogue(7)  # "미안하지만 난 그럴 수 없어."
        await finished_dialogue
    emit_signal("on_mercy_end")  # 필수!

func can_spare() -> bool:
    # 자비 조건 로직 (예: 특정 ACT 수행 후)
    return current_act >= 2
```

### `on_defeat(death: bool)` (virtual)
적이 패배했을 때 호출됩니다.

```gdscript
func on_defeat(death: bool):
    if death:
        # 죽음 처리
        play_dialogue(8)  # "크윽 ... 나는 졌다..."
        await finished_dialogue
    else:
        # 자비 처리
        play_dialogue(9)  # "너는 착한 녀석이구나"
        await finished_dialogue
    
    emit_signal("on_defeat_end")  # 필수! (호출시 자동으로 파티클 실행)
```

### `damage_info(amount: int)` (virtual)
적이 데미지를 입을 때 호출됩니다.
`[time=X]` 태그를 사용하여 데미지 메시지의 표시 시간을 조절할 수 있습니다.

```gdscript
func damage_info(amount: int) -> String:
    # 커스텀 데미지 메시지 반환
    if amount >= 50:
        return "[time=0.5]CRITICAL HIT!"
    elif amount <= 10:
        return "HIT"
    else:
        return ""  # 기본 메시지 사용
```

### `handle_victory()` (virtual)
적이 승리했을 때 호출됩니다.
리턴 값으로 플레이어가 패배했을 때 볼 메시지를 반환할 수 있습니다.
```gdscript
func handle_victory():
    return "여기서 멈출 순 없어..."  # 패배 메시지
```

### `change_act(act_index: int)`
적의 ACT를 변경합니다.
act_index는 [`enemy_acts`](#enemy_acts-arrayenemyact) 배열의 인덱스입니다.

```gdscript
change_act(1)  # ACT 1로 변경
```

### `play_dialogue(index: int, duration: float = 0, skip: bool = true, keep_expression: bool = false)` :id=play_dialogue-1
적의 대화 박스 보이게 하고 대화를 재생합니다.
index은 대화 리소스의 인덱스입니다.

```gdscript
play_dialogue(2)  # 대화 인덱스 2 재생
await finished_dialogue  # 대화 완료까지 대기
play_dialogue(0, 2.0, false, true)  # 대화 인덱스 0 재생, 2초 후 자동 종료, 스킵 불가, 표정 유지
```

### `play_set_dialogue(dialogue: Dialogues, duration: float = 0, skip: bool = true, keep_expression: bool = false)` :id=play_set_dialogue-1
적의 대화 박스 보이게 하고 대화 리소스를 재생합니다.

```gdscript
var custom_dialogue = Dialogues.new().from([
    "적의 커스텀 대화 1",
    "적의 커스텀 대화 2"
])
play_set_dialogue(custom_dialogue)
await finished_dialogue  # 대화 완료까지 대기
```

### `modify_stats(value: Dictionary)`
적의 능력치를 수정합니다.

```gdscript
modify_stats({"hp": stats.hp - 20})  # HP 20 감소
modify_stats({"def": stats.def + 5})  # 방어력 5 증가
```

### `get_sprites() -> Node`
지정한 [`sprites_path`] 경로의 노드를 반환합니다.

```gdscript
var sprites_node = get_sprites()
```

### `get_solo() -> bool`
현재 전투가 솔로 전투인지 반환합니다.

```gdscript
var is_solo = get_solo()
print("혼자 남아 있는지 : ", is_solo)
```

---

## 멤버 변수

### 에디터에서 설정 가능한 변수

#### `enemy_name: String`
적의 이름입니다.

#### `kr: bool`
적이 플레이어에게 kr(Karma)를 가할지 여부입니다. <br>
값이 `true`이면 플레이어가 적의 공격에 피격될 때 지속적으로 HP가 감소합니다.

#### `isFirstTurn: bool`
적이 전투 시작 시 첫 번째 턴인지 여부입니다. <br>
값이 `true`이면 적의 첫 턴이 적의 턴으로 시작됩니다.

#### `dodging: bool`
적이 회피 할지 여부입니다. <br>
값이 `true`이면 적이 플레이어의 공격을 회피합니다.

#### `stats: Dictionary`
적의 능력치입니다. <br>
기본값: `{"hp": 100, "max_hp": 100, "def": 0}`

#### `rewards: Dictionary`
적이 패배 시 플레이어에게 주는 보상입니다. <br>
기본값: `{"exp": 10, "gold": 10}`

#### `dialogues: DialogueAsset`
적의 대화 리소스입니다. <br>
대화 인덱스별로 적의 대사를 설정할 수 있습니다.

#### `current_act: int`
현재 적의 ACT 인덱스입니다. <br>
값은 [`enemy_acts`](#enemy_acts-arrayenemyact) 배열의 인덱스를 가리킵니다.

#### `enemy_acts: Array<EnemyAct>`
적의 ACT 배열입니다. <br>
각 ACT는 [`EnemyAct`](../resource/EnemyAct.md) 리소스입니다.

#### `one_by_one_override: bool`
ACT 변경 시 적이 하나씩 오버다이를 실행할지 여부입니다. <br>
ex) ACT 0에서 ACT 1으로 변경 시
ACT 0 ("Check")
ACT 1 ("Tease", "Joke")
일때 여기서 `one_by_one_override`가 `true`이면
("Check", "Joke") 가 됩니다.

#### `hurt_sound_path: NodePath<AudioStreamPlayer>`
적이 피격될 때 재생되는 AudioStreamPlayer 노드의 경로입니다.

#### `dust_path: NodePath<DustTransition>`
적이 사망할 때 재생되는 [`DustTransition`](DustTransition.md) 노드의 경로입니다.

#### `spare_path: NodePath<GPUParticles2D>`
적이 자비를 받을 때 재생되는 GPUParticles2D 노드의 경로입니다.

#### `sprites_path: NodePath<Node>`
적의 스프라이트 노드의 경로입니다. <br>
Node2D, AnimatedSprite2D 등 어떤 노드든 설정할 수 있습니다.

#### `dialogue_path: NodePath<DialogueControl>`
적의 대화 박스 노드의 경로입니다. <br>
대화 박스 노드는 [`DialogueControl`](../systemNodes/DialogueControl.md) 노드여야 합니다.

#### `expression_sprites: Array<AnimatedSprite2D>` :id=expression_sprites-arrayanimatedsprite2d
적의 표정 스프라이트 배열입니다. <br>
인덱스 순서: [0]=몸체, [1]=머리 등 index를 기억하면 대화 시 표정 변경에 [사용 가능합니다.](#표정-제어)

### 스크립트에서만 사용 가능한 변수
| 속성 | 타입 | 설명 |
|------|------|------|
| `enemy_id` | `int` | 적 ID |
| `main` | `BattleMain` | 전투 메인 노드 |
| `box` | `BattleBox` | 전투 박스 (소울 영역) |
| `soul` | `SoulBattle` | 플레이어 소울 |
| `camera` | `CameraController` | 카메라 컨트롤러 |
| `dialogue` | `DialogueControl` | 대화 박스 노드 |
| `attacks` | `AttackManager` | 공격 매니저 |
| `bg` | `TextureRect` | 전투 배경 노드 |
| `encounter_script` | `EncounterScript` | 조우 스크립트 노드 |

---

## 시그널

### `finished_dialogue`
- 대화 완료 시그널
- [`play_dialogue()`](#play_dialogueindex-int-duration-float--0-skip-bool--true-keep_expression-bool--false) 또는 [`play_set_dialogue()`](#play_set_dialoguedialogue-dialogues-duration-float--0-skip-bool--true-keep_expression-bool--false) 호출 시 대화가 완료되면 발생합니다.


### `changed_act`
- ACT 변경 시그널
- [`change_act()`](#change_actact_index-int) 호출 시 ACT가 변경되면 발생합니다.

### `dodged(to_right: bool)`
- 적이 플레이어의 공격을 회피했을 때 발생하는 시그널
- `to_right` 매개변수는 적이 오른쪽으로 회피했는지 여부를 나타냅니다.

```gdscript
func _on_dodged(to_right: bool):
    if to_right:
        pass  # 오른쪽으로 회피했을 때 애니메이션 처리
    else:
        pass  # 왼쪽으로 회피했을 때 애니메이션 처리
```

### `hit(damage: int)`
- 적이 플레이어의 공격에 피격되었을 때 발생하는 시그널
- `damage` 매개변수는 적이 받은 데미지 양을 나타냅니다.

```gdscript
func _on_hit(damage: int):
    print("적이 ", damage, "의 데미지를 받았습니다!")
```

### `on_fight_end`
- FIGHT 처리 완료 시그널
- [`on_fight()`](#on_fightmiss-bool-virtual) 가상 함수에서 반드시 발생시켜야 합니다.

### `on_act_end`
- ACT 처리 완료 시그널
- [`on_act()`](#on_actoption-int-virtual) 가상 함수에서 반드시 발생시켜야 합니다.

### `on_item_end`
- ITEM 처리 완료 시그널
- [`on_item()`](#on_itemoption-int-virtual) 가상 함수에서 반드시 발생시켜야 합니다.

### `on_mercy_end`
- MERCY 처리 완료 시그널
- [`on_mercy()`](#on_mercy-virtual) 가상 함수에서 반드시 발생시켜야 합니다.

### `on_defeat_end`
- 패배 처리 완료 시그널
- [`on_defeat()`](#on_defeatdeath-bool-virtual) 가상 함수에서 반드시 발생시켜야 합니다.

---

## 능력치 및 상태 관리

### 능력치 시스템

```gdscript
# 기본 능력치 구조
stats = {
    "hp": 100,        # 현재 HP
    "max_hp": 100,    # 최대 HP
    "def": 10         # 방어력
}

# 능력치 수정
func buff_defense():
    modify_stats({"def": stats.def + 5})
    
func heal_self():
    var new_hp = min(stats.hp + 30, stats.max_hp)
    modify_stats({"hp": new_hp})
```

### 보상 시스템

```gdscript
# 기본 보상 구조
rewards = {
    "exp": 20,        # 경험치
    "gold": 15        # 골드
}
```

## 애니메이션 및 표정 시스템

### 표정 제어

```gdscript
# 표정 스프라이트 설정
expression_sprites = [
    get_node("Sprites/Body"),   # [0]: 몸체
    get_node("Sprites/Head")    # [1]: 머리 (표정)
]

# 대화 중 표정 변경
play_set_dialogue(Dialogues.new().from([
    "화난다",
    "기쁜다",
    "슬픈다"
].set_expressions([1, [0, 1], 2]))) # 표정 인덱스 지정 (2차원 배열은 [몸체, 머리] 순서)
```

---

## 주의사항 & 팁

### 권장사항

1. **반드시 시그널 발생**
   ```gdscript
   func on_fight_used(miss: bool):
       # 처리 로직...
       emit_signal("on_fight_end")  # 필수!
   ```

2. **대화 완료 대기**
   ```gdscript
   play_dialogue(0)
   await finished_dialogue  # 대화 완료까지 대기
   ```

### 피해야 할 것들

1. **시그널 발생 누락**
   ```gdscript
   func on_act_used(option: int):
       # 처리만 하고 시그널 발생 안함 → 게임 멈춤!
   ```

2. **잘못된 노드 경로**
   ```gdscript
   sprites_path = NodePath("WrongPath")  # 존재하지 않는 경로
   ```

---

## 🔗 관련 문서

- [AttackBase - 공격 패턴](/classes/userNodes/AttackBase.md)
- [Bullet - 기본 탄환](/classes/userNodes/Bullet.md)
- [Dialogues - 대화 리소스](/classes/resource/Dialogues.md)
- [EnemyAct - 적 상태](/classes/resource/EnemyAct.md)
- [전투 만들기](/tutorials/battle.md)