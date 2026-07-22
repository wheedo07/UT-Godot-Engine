# DialogueLocalization

`DialogueLocalization` 클래스는 대화 리소스들을 현지화하는 데 사용됩니다. 이 클래스는 다양한 언어와 지역에 맞게 대화 내용을 조정할 수 있는 기능을 제공합니다.

---

## 메서드

### `load(path: string) -> DialogueLocalization`
지정된 JSON 파일 경로에서 대화 현지화 데이터를 로드합니다.
- **매개변수:**
  - `path`: 현지화 데이터가 포함된 JSON 파일의 경로입니다.
- **반환값:** 자기 자신(`DialogueLocalization` 인스턴스)을 반환합니다.
- **JSON 예시:**
```json
{
    "SANS_CALL_1": {
        "ko": {
            "text": ["* 전화 받았습니다", "* WA sans"], // 대사
            "speed": [0.05], // 타이핑 속도
            "pauses": [ // DialoguePause 정보
                { "index": 6, "duration": 1.0 }
                // [ { "index": 10, "duration": 0.5 }, { "index": 15, "duration": 0.3 } ] 와 같이 여러 개 추가 가능
            ]
        },
        "en": {
            "text": ["* Call answered", "* WA sans"],
            "speed": [0.03]
        },
        "expressions": [0, 1] // 표정 인덱스 배열
    }
}
```
- **설명:** JSON 파일은 각 대화 ID에 대해 여러 언어의 대사, 타이핑 속도, 일시 중지 지점 및 표정 인덱스를 정의할 수 있습니다.

### `get_dialogues(locale: string, id: string) -> Dialogues`
[`load`](#loadpath-string---dialoguelocalization)를 한 후, 특정 언어와 대화 ID에 해당하는 대화 데이터를 가져옵니다.
- **매개변수:**
  - `locale`: 원하는 언어 코드(예: "ko", "en")입니다.
  - `id`: 대화 ID입니다.
- **반환값:** 해당 언어와 ID에 맞는 [`Dialogues`](Dialogues.md) 객체를 반환합니다.
- **설명:** 이 메서드는 로드된 현지화 데이터에서 특정 언어와 대화 ID에 해당하는 대화 내용을 추출합니다.

### `get_dialogues_default(id: string) -> Dialogues`
[`load`](#loadpath-string---dialoguelocalization)를 한 후, 현재 시스템 언어 설정에 맞는 대화 데이터를 가져옵니다.
- **매개변수:**
  - `id`: 대화 ID입니다.
- **반환값:** 현재 시스템 언어 설정에 맞는 [`Dialogues`](Dialogues.md) 객체를 반환합니다.
- **설명:** 이 메서드는 시스템의 기본 언어 설정에 따라 적절한 대화 내용을 자동으로 선택합니다.

---

## 🔗 관련 문서

- [Dialogues](Dialogues.md)
- [DialoguePause](DialoguePause.md)