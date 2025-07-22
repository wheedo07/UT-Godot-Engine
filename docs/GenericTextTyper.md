# GenericTextTyper

UT-Godot-Engine에서 Undertale 스타일의 타이핑 텍스트 효과를 구현하는 노드입니다. 대화 상자에서 글자가 하나씩 나타나는 타이핑 효과를 제공합니다.

## 시그널 (Signals)

| 시그널명 | 인자 | 설명 |
| --- | --- | --- |
| `started_typing` | `(line: int)` | 새 라인 타이핑이 시작될 때 발생 (라인 인덱스 제공) |
| `confirm` | - | 사용자가 확인 버튼을 눌렀을 때 발생 |
| `skip` | - | 사용자가 취소 버튼을 눌러 텍스트를 스킵할 때 발생 |
| `finished_all_texts` | - | 모든 텍스트의 타이핑이 완료됐을 때 발생 |

## 속성 (Properties)

| 속성명 | 타입 | 설명 |
| --- | --- | --- |
| `click_path` | `NodePath` | 타이핑 사운드 효과를 재생할 AudioStreamPlayer 경로    |
| `interval` | `float` | 글자 간 타이핑 간격(초)    |
| `queued_texts_handling` | `enum` | 여러 텍스트 처리 모드 (AWAIT_FINISH, OVERRIDE_CURRENT, VOID_QUEUED)    |
| `entire_text_bbcode` | `String` | 텍스트 앞에 추가할 BBCode 텍스트    |
| `extra_delay` | `String` | 추가 지연이 필요한 문자들  |
| `no_sound` | `String` | 소리가 나지 않는 문자들   |

## 열거형 (Enums)

### TextQueueMode

| 값 | 설명 |
| --- | --- |
| `AWAIT_FINISH` | 현재 타이핑이 완료될 때까지 기다린 후 다음 텍스트 처리 |
| `OVERRIDE_CURRENT` | 현재 타이핑을 중단하고 새 텍스트로 교체 |
| `VOID_QUEUED` | 타이핑 중이면 대기열에 있는 텍스트 무시 |

## 메서드 (Methods)

| 메서드명 | 인자 | 설명 |
| --- | --- | --- |
| `kill_tweens` | `(complete_text: bool = false)` | 현재 실행 중인 모든 트윈을 중단 (complete_text가 true면 텍스트 완료) |
| `type_text` | `(text: PackedStringArray)` | 지정된 텍스트 배열을 타이핑 시작 |

## 💡 사용 예시

### 기본 설정

1. 씬에 `GenericTextTyper` 노드 추가
2. RichTextLabel을 상속 받았으므로 RichTextLabel 속성도 설정 가능
3. `click_path` 속성에 타이핑 소리 효과를 재생할 AudioStreamPlayer 노드 경로 지정
4. `interval` 속성으로 타이핑 속도 조절 (값이 클수록 느림)

### GDScript 예시

```gdscript
# 텍스트 상자에 타이핑 효과 추가하기
@onready var text_typer = $GenericTextTyper

func start_dialogue():
    var dialogue_lines = [
        "* 안녕하세요! 반갑습니다.",
        "* 이것은 Undertale 스타일 대화창입니다.",
        "* 타이핑 효과가 적용되어 있습니다."
    ]
    
    text_typer.type_text(dialogue_lines)
    text_typer.connect("finished_all_texts", _on_dialogue_finished)
    text_typer.connect("confirm", _on_dialogue_confirm)

func _on_dialogue_finished():
    print("대화 종료")
    
func _on_dialogue_confirm():
    print("다음 대화")
```

### 특수 효과

1. 추가 지연 효과
   - 특정 문장부호나 기호 뒤에 지연 효과를 주려면 `extra_delay` 문자열에 해당 문자를 포함시키세요.
   - 기본값: "@#$%^&+=_-~`<>\"|\\*{}()[].,!?"

2. 무음 문자
   - 타이핑 소리가 나지 않게 할 문자는 `no_sound` 문자열에 포함시키세요.
   - 기본값: "@#$%^&+=_-~`<>\"|\\*{}()[].,!? " (공백 포함)

## 주의사항

- `queued_texts_handling` 속성을 적절히 설정하여 여러 텍스트 대기열 처리 방식을 선택하세요.
- 타이핑 효과가 진행 중일 때 'ui_cancel' 입력으로 현재 텍스트를 건너뛸 수 있습니다.
- 텍스트가 타이핑되는 도중에는 'ui_cancel' 입력으로 빠르게 텍스트를 완성할 수 있습니다.
- 텍스트가 완전히 타이핑된 후에는 'ui_accept' 입력으로 다음 텍스트로 넘어갑니다.
- entire_text_bbcode를 설정할때 BBCode Enabled 를 활성화해야 합니다.