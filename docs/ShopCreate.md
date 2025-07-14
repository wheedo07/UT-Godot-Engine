# ShopCreate 노드

UT-Godot-Engine에서 상점을 구현할 때 사용하는 전용 노드입니다.
상점 인터페이스, 대화 연출, 물건 구매 처리 등을 위한 커스터마이징 가능한 구성요소입니다.

---

## 📌 주요 기능

* 상점 점원의 대화 처리
* 판매 가능 여부 설정
* 상점 음악 설정
* 선택지 및 구매 처리

---

## ✅ 속성 (Properties)

| 속성명              | 타입                               | 설명                      |
| ---------------- | -------------------------------- | ----------------------- |
| `shop_music`     | `AudioStream`                    | 상점 BGM 설정               |
| `click_path`     | `NodePath` (`AudioStreamPlayer`) | 대사 재생시 나올 효과음 (예시: 샌즈 효과음)      |
| `can_be_sold_to` | `bool`                           | 플레이어가 아이템을 판매할 수 있는지 여부 |
| `room_id`        | `int`                            | 상점에서 나가면 나올 RoomEntranceNode에 room ID     |
| `room_path`      | `String`                         | 상점에서 나가면 나올 Overworld 경로       |


## 🔧 사용 가능한 메서드

| 함수명                       | 설명                                     |
| ------------------------- | -------------------------------------- |
| `ready()`                 | GDScript에서 상점 노드가 준비될 때 실행되는 오버라이드용 함수 |


## 💡 사용 예시

<details>
<summary>GDScript</summary>

```gdscript
extends ShopCreate

func ready():
    print("상점 초기화");
```

</details>

<details>
<summary>C++</summary>

```cpp
#include "test_shop.h"
#include "env.h"

void TEST_SHOP::_bind_methods() {}

void TEST_SHOP::ready() {
}
```

</details>

---
