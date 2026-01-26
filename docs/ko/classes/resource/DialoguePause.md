# DialoguePause

`DialoguePause` 클래스는 대화 중에 특정 지점에서 일시 중지를 나타내는 데 사용됩니다. <br>
[`AdvancedTextTyper`](../userNodes/AdvancedTextTyper.md) 노드와 함께 사용되어 대화 텍스트의 타이핑 효과를 제어할 수 있습니다.

---

## 속성

### `pause_index`
- **타입**: `int`
- **설명**: 대화 텍스트에서 일시 중지를 적용할 문자 인덱스입니다. 이 인덱스에 도달하면 타이핑이 일시 중지됩니다.
- **기본값**: `0`

### `pause_duration`
- **타입**: `float`
- **설명**: `pause_index`에 도달했을 때 일시 중지할 시간(초)입니다. 모든 글자는 기본적으로 0.1초 동안 멈추며, 이 속성으로 변경 할 수 있습니다.
- **기본값**: `0.5`

---

## 🔗 관련 문서
- [Dialogue](Dialogue.md)
- [AdvancedTextTyper](../userNodes/AdvancedTextTyper.md)