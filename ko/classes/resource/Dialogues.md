# Dialogues

UT-Godot-Engine에서 대화 시스템에 사용되는 [`Dialogue`](Dialogue.md) 리소스의 컬렉션을 나타냅니다. 대화를 만들고 관리하는 데 사용됩니다.

---

## 속성

### `dialogues`
- **타입**: `Array<Dialogue>`
- **설명**: 포함된 `Dialogue` 리소스의 배열입니다.
- **기본값**: `[]`

---

## 메서드

### `from(text: PackedStringArray) -> Dialogues`
- **설명**: 주어진 문자열 배열에서 [`Dialogue`](Dialogue.md) 리소스들을 생성하여 [`dialogues`](#dialogues) 속성에 추가합니다.
- **매개변수**:
  - `text: PackedStringArray` - 각 문자열은 [`Dialogue.dialog_text`](Dialogue.md#dialog_text) 속성에 저장됩니다.
- **반환값**: `Dialogues` - 자기 자신의 인스턴스입니다.
- **예제**:
```gdscript
var dialogues = Dialogues.new().from(["안녕!", "반가워요!"])
```

### `get_dialogues_single(dialog_type: DialogueType) -> Array`
- **설명**: 모든 포함된 [`Dialogue`](Dialogue.md) 리소스에서 특정 속성의 값을 배열로 반환합니다. 자세한건 [`DialogueType`](#dialoguetype) 섹션을 참조하세요.
- **매개변수**:
  - `dialog_type: DialogueType` - 가져올 속성의 타입을 지정하는 상수입니다.
- **반환값**: `Array` - 지정된 속성의 값들로 이루어진 배열입니다.
- **예제**:
```gdscript
var dialogues = Dialogues.new().from(["안녕!", "반가워요!"])
var texts = dialogues.get_dialogues_single(Dialogues.DIALOGUE_TEXT)
# texts는 ["안녕!", "반가워요!"]가 됩니다.
```

### `set_expressions(expressions: Array) -> Dialogues`
- **설명**: 기존에 생성된 즉 [`dialogues`](#dialogues) 속성에 포함된 모든 [`Dialogue`](Dialogue.md) 리소스들의 [`Dialogue.dialog_expressions`](Dialogue.md#dialog_expressions) 속성을 일괄적으로 설정합니다.
- **매개변수**:
  - `expressions: Array` - 각 요소는 각 대화에 대한 표정 인덱스 배열입니다.
- **반환값**: `Dialogues` - 자기 자신의 인스턴스입니다.
<br>

- **TextBox 에서 예제**
```gdscript
dialogues.set_expressions([0, 1, 2]) # 각 대화에 단일 표정 설정
dialogues.set_expressions([ # 각 대화에 표정 시퀀스 설정  
    [0, 1],        # 첫 번째 대화: 표정 0 → 1
    -1,            # 두 번째 대화: 표정 변경 없음
    [3, 4, 5]      # 세 번째 대화: 표정 3 → 4 → 5
])
```
- **Enemy 노드에서 예제**: Textbox와 다르게 2차원 배열로 [`Enemy.expression_sprites`](../userNodes/Enemy.md#expression_sprites-arrayanimatedsprite2d) 인덱스를 지정할 수 있습니다.
```gdscript
dialogues.set_expressions([0, 1, 2]) # 각 대화에 단일 표정 설정
dialogues.set_expressions([ # 각 대화에 expression_sprites 인덱스 마다에 따른 표정 시퀀스 설정  
    [ [0, 1], [0, 2] ],   # 첫 번째 대화: 몸체 0 → 1, 머리 0 → 2
    [-1, -1],             # 두 번째 대화: 표정 변경 없음
    [ [2, 3], [1, 0] ]    # 세 번째 대화: 몸체 2 → 3, 머리 1 → 0
])
```

### `set_speed(speeds: Array) -> Dialogues`
- **설명**: 기존에 생성된 즉 [`dialogues`](#dialogues) 속성에 포함된 모든 [`Dialogue`](Dialogue.md) 리소스들의 [`Dialogue.dialog_speed`](Dialogue.md#dialog_speed) 속성을 일괄적으로 설정합니다.
- **매개변수**:
  - `speeds: Array` - 각 요소는 각 대화에 대한 텍스트 표시 속도입니다.
- **반환값**: `Dialogues` - 자기 자신의 인스턴스입니다.
- **예제**:
```gdscript
dialogues.set_speed([0.08, 0.05, 0.12])
```

### `set_pauses(pauses: Array) -> Dialogues`
- **설명**: 기존에 생성된 즉 [`dialogues`](#dialogues) 속성에 포함된 모든 [`Dialogue`](Dialogue.md) 리소스들의 [`Dialogue.pauses`](Dialogue.md#pauses) 속성을 일괄적으로 설정합니다.
- **매개변수**:
  - `pauses: Array` - 각 요소는 각 대화에 대한 일시정지 지점 배열입니다.
- **반환값**: `Dialogues` - 자기 자신의 인스턴스입니다.
- **예제**:
```gdscript
dialogues.set_pauses([
    {"index": 3, "duration": 0.5}, # 첫 번째 대화: 3번째 글자 후 0.5초 일시정지
    {"index": 2, "duration": 1.0}, # 두 번째 대화: 2번째 글자 후 1.0초 일시정지
    {} # 세 번째 대화: 일시정지 없음
])
# 각 대화에 여러 일시정지 설정
dialogues.set_pauses([
    [ {"index": 2, "duration": 0.3}, {"index": 5, "duration": 0.7} ], # 첫 번째 대화
    [ {"index": 4, "duration": 1.0} ], # 두 번째 대화
    [] # 세 번째 대화: 일시정지 없음
])
```

---

## 상수

### `DialogueType`

#### `DIALOGUE_TEXT`
- **설명**:
    - returns: `Array<String>`
대화의 텍스트 내용을 나타내는 타입입니다. [`get_dialogues_single`](#get_dialogues_singledialog_type-dialoguetype---array) 메서드에서 사용하여 모든 [`Dialogue`](Dialogue.md) 리소스의 [`Dialogue.dialog_text`](Dialogue.md#dialog_text) 속성을 가져올 수 있습니다.

#### `DIALOGUE_EXPRESSIONS`
- **설명**:
    - returns: `Array<Array<int>>` <br>
대화의 표정 인덱스 배열을 나타내는 타입입니다. [`get_dialogues_single`](#get_dialogues_singledialog_type-dialoguetype---array) 메서드에서 사용하여 모든 [`Dialogue`](Dialogue.md) 리소스의 [`Dialogue.dialog_expressions`](Dialogue.md#dialog_expressions) 속성을 가져올 수 있습니다.

#### `DIALOGUE_PAUSES`
- **설명**:
    - returns: `Array<Array<DialoguePause>>` <br>
대화의 일시정 지점 배열을 나타내는 타입입니다. [`get_dialogues_single`](#get_dialogues_singledialog_type-dialoguetype---array) 메서드에서 사용하여 모든 [`Dialogue`](Dialogue.md) 리소스의 [`Dialogue.pauses`](Dialogue.md#pauses) 속성을 가져올 수 있습니다.

#### `DIALOGUE_SPEED`
- **설명**:
    - returns: `Array<float>` <br>
대화의 텍스트 표시 속도를 나타내는 타입입니다. [`get_dialogues_single`](#get_dialogues_singledialog_type-dialoguetype---array) 메서드에서 사용하여 모든 [`Dialogue`](Dialogue.md) 리소스의 [`Dialogue.dialog_speed`](Dialogue.md#dialog_speed) 속성을 가져올 수 있습니다.

---

## 🔗 관련 문서
- [Dialogue - 대화 리소스](Dialogue.md)
- [TextBox - 대화 상자 노드](/classes/systemNodes/TextBox.md)