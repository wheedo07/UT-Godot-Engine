# Dialogue

`Dialogue` 리소스는 하나의 대화을 나타냅니다. 여러개의 대화를 묶어서 `Dialogues` 리소스로 관리할 수 있습니다.

## 속성

### `dialog_expressions`
- **타입**: `Array<int>`
- **설명**: 대화에 사용되는 표정 인덱스의 배열입니다. 각 인덱스는 `AnimatedSprite2D` 노드에서 인덱스로 사용됩니다.
- **기본값**: `[ -1 ]`

### `dialog_speed`
- **타입**: `float`
- **설명**: 대화 텍스트가 표시되는 속도입니다. 값이 작을수록 텍스트가 더 빠르게 표시됩니다.
- **기본값**: `0.05`

### `dialog_text`
- **타입**: `String`
- **설명**: 대화의 실제 텍스트 내용입니다.
- **기본값**: `"test"`

### `pauses`
- **타입**: `Array<DialoguePause>`
- **설명**: 텍스트 타이핑 중 특정 문자 위치에서 일시정 시간 동안 멈추는 지점을 정의하는 `DialoguePause` 리소스의 배열입니다.
- **기본값**: `[]`

## 🔗 관련 문서
- [DialoguePause](DialoguePause.md)
- [Dialogues](Dialogues.md)
- [TextBox](/classes/systemNodes/TextBox.md)