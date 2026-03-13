# DialogueControl

> 🔴 **사용자 생성 불가능**
>> 사용자가 직접 생성할 수 없고 씬을 인스턴스화하거나 시스템에서 사용되는 노드입니다.
>> 이 노드는 `res://Engine/Battle/Misc/dialogue.tscn` 씬을 인스턴스화하여 사용됩니다.

---

## 주요 역할
- 전투 중 적의 말풍선 시스템을 관리합니다.
- 각 캐릭터에 맞는 말풍선 스타일을 적용합니다.
- 대화 텍스트의 타이핑 효과 및 표정 변화를 처리합니다.
- [`Enemy`](../userNodes/Enemy.md)의 `play_dialogue()` 또는 `play_set_dialogue()` 를 통해 자동으로 호출되며, `Enemy` 노드의 자식으로 존재하는 `Dialogue` 노드가 이에 해당합니다.

---

## 메서드

### `type_text_bubble(dialogues: Dialogues)`
주어진 [`Dialogues`](../resource/Dialogues.md) 리소스의 내용을 말풍선으로 출력합니다.  
보통 [`Enemy.play_dialogue()`](../userNodes/Enemy.md#play_dialogue-1) 또는 [`Enemy.play_set_dialogue()`](../userNodes/Enemy.md#play_set_dialogue-1)를 통해 호출되지만, 직접 호출하는 것도 가능합니다.

### `kill()`
말풍선을 즉시 제거하고 모든 대화 텍스트 출력을 중단합니다.
대화를 강제로 종료해야 하는 상황에서 사용됩니다.

---

## 멤버 변수

### `character_name: String` (기본값: `"DEFAULT"`)
현재 말풍선의 캐릭터 이름입니다. 이 값을 설정하면 지정된 캐릭터에 맞는 말풍선 스타일과 텍스트 효과가 자동으로 적용됩니다.  
캐릭터 이름은 [`Stagehand`](Stagehand.md)의 Character 자식 노드로 생성된 [`CharacterSetting`](../userNodes/CharacterSetting.md) 노드들의 이름과 일치해야 합니다.

---

## 신호

### `finished_all_texts_dialogue()`
모든 대화 텍스트 출력이 완료되었을 때 발생합니다.  
`type_text_bubble()`로 시작된 대화가 끝나고 말풍선이 사라질 때 발생하며, 대화 종료 또는 전투 시스템에서 다음 턴으로 진행할 때 사용됩니다.

### `set_expression(expressions: Variant)`
대화 텍스트에서 표정 변경 명령이 감지되었을 때 발생합니다.  
`expressions`는 표정 정보를 담은 배열로, 캐릭터의 표정 또는 애니메이션을 변경하는 데 사용됩니다.  
이 신호를 통해 대화 내용에 따라 적 캐릭터의 표정을 동적으로 변경할 수 있습니다.

### `started_dialogue(line: int)`
[`Dialogues.dialogues`](../resource/Dialogues.md#dialogues-1)의 각 대화 텍스트 출력이 시작될 때마다 발생합니다.  
`line`은 현재 출력 중인 대화 텍스트의 인덱스입니다.  
이 신호를 사용하면 대화가 시작될 때마다 특정 효과를 발동하거나, 대화 내용에 따라 캐릭터의 행동을 변경할 수 있습니다.

---

## 🔗 관련 문서
- [`Enemy`](../userNodes/Enemy.md)
- [`Dialogues`](../resource/Dialogues.md)