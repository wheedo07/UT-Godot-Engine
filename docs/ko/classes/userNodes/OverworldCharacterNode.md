# OverworldCharacterNode

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

---

## 주요 역할
- **이동 시스템**
    - 방향성 이동과 idle/walking 상태 간 자동 애니메이션 전환
    - 캐릭터 이동 속도 및 방향 제어
- **애니메이션 관리**
    - 방향별 애니메이션 자동 처리 (위, 아래, 좌우)
    - 스프라이트 자동 플립 기능
- **대화 시스템**
    - [`DialogueAsset`](../resource/DialogueAsset.md)과 [`TextBox`](../systemNodes/TextBox.md) 통합
    - 캐릭터별 대화 관리
- **상호작용 시스템**
    - [`InteractionTrigger`](../systemNodes/InteractionTrigger.md)를 사용한 플레이어 상호작용 감지
    - 상호작용 시 자동 대화 시작
- **알림 시스템**
    - 조우나 상호작용 시 시각적 알림과 효과음

---

## 필수 컴포넌트

### 노드 구조
```
OverworldCharacterNode
├── OverworldSprite     # 애니메이션 스프라이트 (필수)
└── InteractionTrigger  # 상호작용 영역 (필수)
```

### 필수 리소스
- [`DialogueAsset`](../resource/DialogueAsset.md) - 캐릭터 대화 데이터

---

## 애니메이션 상태

캐릭터는 속도에 따라 자동으로 idle과 이동 애니메이션 간 전환합니다:

### Idle 애니메이션
- `"idle_down"` - 아래 방향 대기
- `"idle_side"` - 좌우 방향 대기
- `"idle_up"` - 위 방향 대기

### 이동 애니메이션
- `"move_down"` - 아래 방향 이동
- `"move_side"` - 좌우 방향 이동
- `"move_up"` - 위 방향 이동

### 특수 애니메이션
- `"act"` - 커스텀 액션용 애니메이션

---

## 메서드

### `start_walking(direction: Vector2i = Vector2i(0, 0))`
지정된 방향으로 캐릭터 이동을 시작하거나 정지합니다.
`direction`이 0이 아니면 이동을 시작하고 이동 애니메이션으로 전환합니다.
`direction`이 0이면 이동을 정지하고 idle 애니메이션으로 전환합니다.

```gdscript
# 오른쪽으로 이동 시작
character.start_walking(Vector2i(1, 0))

# 아래쪽으로 이동
character.start_walking(Vector2i(0, 1))

# 이동 정지
character.start_walking(Vector2i(0, 0))
```

### `force_direction(dir: Vector2)`
이동하지 않고 특정 방향을 바라보도록 강제합니다.
정규화된 방향 벡터를 기반으로 애니메이션 상태와 스프라이트 방향을 업데이트합니다.
플레이어나 특정 객체를 바라보도록 할 때 유용합니다.

```gdscript
# 플레이어를 바라보도록 설정
var player_pos = global.get_player().global_position
var direction = (player_pos - global_position).normalized()
character.force_direction(direction)

# 특정 방향으로 고정
character.force_direction(Vector2.DOWN)  # 아래쪽을 바라봄
```

### `show_alert(duration: float = 0.35)`
지정된 시간 동안 캐릭터 위에 알림 표시기를 보여줍니다.
조우 효과음을 재생하고 [`frame_alert`](#frame_alert-int)를 사용해 알림 스프라이트를 표시합니다.
시간이 지나면 자동으로 알림이 숨겨집니다.

```gdscript
# 기본 시간(0.35초) 알림 표시
character.show_alert()

# 1초 동안 알림 표시
character.show_alert(1.0)

# 조우 시 알림
func on_enemy_encounter():
    character.show_alert(0.5)
```

### `set_frame(index: int)`
"act" 애니메이션의 특정 프레임을 표시하도록 캐릭터 스프라이트를 설정합니다.
일반 애니메이션 사이클 외부에서 커스텀 포즈나 표정을 표시할 때 유용합니다.

```gdscript
# 특별한 포즈 표시
character.set_frame(2)  # act 애니메이션의 2번 프레임

# 다양한 표정 변경
match emotion:
    "happy":
        character.set_frame(0)
    "sad":
        character.set_frame(1)
    "angry":
        character.set_frame(2)
```

### `play_anim(key: String, speed: float = 1.0, from_end: bool = false)`
선택적 속도 및 방향 제어와 함께 특정 애니메이션을 재생합니다.
`animation_finished` 시그널에 연결되어 완료 시 [`animation_finished`](#animation_finished) 시그널을 발생시킵니다.

```gdscript
# 일반 애니메이션 재생
character.play_anim("act")
await character.animation_finished

# 빠른 속도로 재생
character.play_anim("move_down", 2.0)

# 역재생
character.play_anim("act", 1.0, true)
```

---

## 멤버 변수

### 에디터에서 설정 가능한 변수

#### `character: String`
대화 표시에 사용되는 캐릭터의 이름입니다.
캐릭터가 말할 때 대화 박스에 표시되는 이름입니다.

#### `walk_speed: int`
캐릭터의 초당 이동 속도(픽셀 단위)입니다.
**범위:** 0-400. 값이 클수록 캐릭터가 더 빠르게 이동합니다.
**기본값:** 60

#### `frame_alert: int`
알림 표시 시 표시할 프레임 인덱스입니다.
알림 스프라이트의 애니메이션 시퀀스에서 프레임을 참조합니다.
**기본값:** 0

#### `sprite: OverworldSprite`
캐릭터 비주얼을 처리하는 [`OverworldSprite`](OverworldSprite.md) 컴포넌트에 대한 참조입니다.
캐릭터가 올바르게 표시되고 애니메이션되려면 반드시 설정해야 합니다.

#### `area_interact: InteractionTrigger`
플레이어 상호작용을 감지하는 [`InteractionTrigger`](../systemNodes/InteractionTrigger.md) 컴포넌트에 대한 참조입니다.
(res://Engine/Overworld/Interactions/interaction_trigger.tscn) 씬을 사용하여 설정해야 합니다.

#### `current_index: int`
이 캐릭터와 상호작용할 때 사용할 현재 대화 인덱스입니다.
[`dialogues`](#dialogues-dialogueasset) 에셋의 대화 배열에서 인덱스를 참조합니다.
다른 대화 상태로 진행하기 위해 변경할 수 있습니다.
**기본값:** 0

#### `dialogues: DialogueAsset`
이 캐릭터의 대화 데이터를 포함하는 [`DialogueAsset`](../resource/DialogueAsset.md) 리소스입니다.
플레이어가 캐릭터와 상호작용할 때 대화 텍스트를 표시하는 데 사용됩니다.

---

## 시그널

### `character_finished`
캐릭터의 대화 상호작용이 완료되었을 때 발생합니다.
플레이어 상호작용으로 시작된 대화 시퀀스가 끝난 후 발생합니다.

```gdscript
func _on_character_finished():
    print("캐릭터와의 대화가 끝났습니다")
    global.set_player_can_move(true)
```

### `animation_finished`
[`play_anim()`](#play_animkey-string-speed-float--10-from_end-bool--false)을 통해 재생된 커스텀 애니메이션이 완료되었을 때 발생합니다.
애니메이션을 연결하거나 특정 애니메이션 후 이벤트를 발생시킬 때 유용합니다.

```gdscript
func play_sequence():
    character.play_anim("act")
    await character.animation_finished
    print("특별한 액션이 완료되었습니다!")
```

---

## 🔗 관련 문서

- [OverworldSprite - 오버월드 스프라이트](OverworldSprite.md)
- [DialogueAsset - 대화 에셋](../resource/DialogueAsset.md)
- [InteractionTrigger - 상호작용 트리거](../systemNodes/InteractionTrigger.md)
- [TextBox - 텍스트 박스](../systemNodes/TextBox.md)
- [오버월드 만들기](/tutorials/overworld.md)