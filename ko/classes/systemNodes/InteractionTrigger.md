# InteractionTrigger

> 🔴 **사용자 생성 불가능**
>> 사용자가 직접 생성할 수 없고 씬을 인스턴스화하거나 시스템에서 사용되는 노드입니다.
>> 이 노드는 `res://Engine/Overworld/Interactions/interaction_trigger.tscn` 씬을 인스턴스화하여 사용됩니다.

---

## 주요 역할
- 상호작용 트리거 노드는 게임 내에서 플레이어와 상호작용할 수 있는 부분을 정의합니다.
    - ex) 문 열기, NPC와 대화하기, 아이템 획득 등
- 이 노드는 [`Overworld`](../userNodes/Overworld.md)에서 [`PlayerOverworld`](PlayerOverworld.md)가 상호작용할 것을 감지하는 데 사용됩니다.

---

## 메서드

### `enable()`
상호작용 트리거를 활성화합니다.

### `disable()`
상호작용 트리거를 비활성화합니다.

---

## 신호

### `interacted()`
플레이어가 상호작용 트리거와 상호작용할 때 발생합니다.

---

## 🔗 관련 문서
- [Overworld - 오버월드 노드](../userNodes/Overworld.md)
- [PlayerOverworld - 오버월드 플레이어](PlayerOverworld.md)
- [오버월드 만들기](/tutorials/overworld.md)