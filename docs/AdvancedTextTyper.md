# AdvancedTextTyper 노드

UT-Godot-Engine의 고급 텍스트 타이핑 연출을 위한 노드입니다.  
대사, 표정, 스피드 등 Undertale 스타일의 복잡한 대화 연출을 구현할 수 있습니다.

---

## 시그널 (Signals)

| 시그널명                    | 설명                                      |
| ----------------------- | --------------------------------------- |
| `advanced_finished_all_texts()` | 모든 대사 출력이 끝났을 때 발생                |
| `click_played()`        | 타이핑이 시작될때 마다 발생                |
| `expression_set(expr)`  | 대사별 표정(혹은 상태) 변경 시 발생 (`expr` 배열 전달) |

---

## 속성 (Properties)

| 속성명                   | 타입                | 설명                                 |
| -------------------- | ----------------- | ---------------------------------- |
| `current_dialogue_index` | `int`              | 현재 출력 중인 대사 인덱스                   |

---

## 🔧 사용 가능한 메서드

| 함수명                                      | 설명                                                         |
| ---------------------------------------- | ---------------------------------------------------------- |
| `type_text_advanced(dialogues: Dialogues)` | Dialogues 리소스를 받아 고급 타이핑 연출 시작                        |
| `get_queued_dialogues()`                  | 현재 대사(Dialogues) 리소스 반환                                 |


## 💡 사용 예시

```gdscript
# GDScript 예시
var adv_typer = $AdvancedTextTyper
var dialogues = Dialogues.new().from([
    "* 테스트 1",
    "* 테스트 2",
    "* 테스트 3"
]).set_expressions([1,4,5]);

adv_typer.type_text_advanced(dialogues);
adv_typer.connect("advanced_finished_all_texts", func():
    print("모든 대사 출력 완료!")
);
adv_typer.connect("expression_set", func(expr):
    # 표정 등 상태 변경 처리
    $Face.set_frame(expr[0])
);
```

## 참고

- `Dialogues` 리소스와 연동하여 대사, 속도, 표정 등 다양한 연출을 지원합니다.
- Undertale 스타일의 복잡한 대화 연출이 필요한 경우

## 주의사항

- `type_text` 함수는 이 노드에서는 안쓰입니다
- `finished_all_texts` 시그널은 이 노드에서 안쓰입니다

## 관련 클래스

- [GenericTextTyper](/docs/GenericTextTyper.md): 이 노드를 이룰때 사용되는 부모 클래스