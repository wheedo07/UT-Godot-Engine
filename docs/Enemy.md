# Enemy 노드

UT-Godot-Engine에서 전투 시스템의 적 캐릭터를 구현하는 핵심 노드입니다.
턴 기반 전투, 대화, 애니메이션, 상태 관리 등 Undertale 스타일의 적 행동을 모두 처리합니다.


## 📌 주요 기능

* 턴 기반 전투 로직 처리
* 적 상태(Act) 관리 시스템
* 대화 및 표정 연출
* 데미지 처리 및 애니메이션
* 회피(dodge) 및 죽음 연출
* 자비(spare) 처리


---

## 시그널 (Signals)

| 시그널명 | 인자 | 설명 |
| --- | --- | --- |
| `finished_dialogue` | - | 대화가 끝났을 때 발생 |
| `changed_state` | - | ACT 상태가 변경됐을 때 발생 |
| `spared` | `(id_number: int)` | 엔티티에 자비를 강제 발생시키는 시그널 |
| `on_fight_end` | - | 공격 행동이 끝났을 때 발생시키는 시그널 |
| `on_act_end` | - | 행동 선택이 끝났을 때 발생시키는 시그널 |
| `on_item_end` | - | 아이템 사용이 끝났을 때 발생시키는 시그널 |
| `on_mercy_end` | - | 자비 행동이 끝났을 때 발생시키는 시그널 |


## ✅ 속성 (Properties)

| 속성명 | 타입 | 설명 |
| --- | --- | --- |
| `enemy_name` | `String` | 적의 이름 |
| `kr` | `bool` | KR 데미지 사용 여부 |
| `isFirstTurn` | `bool` | 첫 턴에 바로 공격 시작 여부 |
| `dodging` | `bool` | 회피 가능 여부 |
| `stats` | `Dictionary` | 적 능력치 (hp, max_hp, def) |
| `rewards` | `Dictionary` | 승리 시 보상 (exp, gold) |
| `dialogues` | `Array<Dialogues>` | 적의 대사 목록 |
| `flavour_text` | `PackedStringArray` | 플레이버 텍스트 |
| `enemy_states` | `Array<EnemyState>` | ACT 목록 |
| `current_state` | `int` | 현재 ACT 인덱스 |

### NodePath 속성들

| 속성명 | 타입 | 설명 |
| --- | --- | --- |
| `hurt_sound_path` | `NodePath` | 피격 사운드 AudioStreamPlayer 경로 |
| `dust_sound_path` | `NodePath` | 사망 사운드 AudioStreamPlayer 경로 |
| `dust_path` | `NodePath` | 사망 파티클 GPUParticles2D 경로 |
| `e_head_path` | `NodePath` | 머리 애니메이션 AnimatedSprite2D 경로 |
| `e_body_path` | `NodePath` | 몸 애니메이션 AnimatedSprite2D 경로 |
| `sprites_path` | `NodePath` | 스프라이트 컨테이너 노드 경로 |
| `dialogue_path` | `NodePath` | 대화 제어 DialogueControl 경로 |

---

## 🔧 사용 가능한 메서드

### 가상 함수 (오버라이드 가능)

| 함수명 | 설명 |
| --- | --- |
| `ready()` | 적 초기화 함수 |
| `on_fight_used()` | 플레이어가 공격했을 때 호출 |
| `on_act_used(option: int)` | 플레이어가 행동을 선택했을 때 호출 |
| `on_item_used(option: int)` | 플레이어가 아이템을 사용했을 때 호출 |
| `on_mercy_used()` | 플레이어가 자비를 선택했을 때 호출 |
| `_on_get_turn()` | 적의 턴이 시작될 때 호출 |
| `_on_end_turn()` | 적의 턴이 끝날 때 호출 |
| `on_win()` | 적이 승리했을 때 호출 |
| `on_defeat(death: bool)` | 적이 패배했을 때 호출 |

### 유틸리티 함수

| 함수명 | 설명 |
| --- | --- |
| `change_state(new_state: int)` | 적 ACT 변경 |
| `play_set_dialogue(dialogue: Dialogues, duration: float, skip: bool)` | 대사 리소스 재생 |
| `play_dialogue(index: int, duration: float, skip: bool)` | 지정된 인덱스의 대사 재생 |
| `modify_stats(value: Dictionary)` | 적 능력치 변경 |


---

## 💡 사용 예시

<details>
<summary>GDScript</summary>

```gdscript
extends Enemy
@onready var attack = preload("res://Game/mainAttacks/attack_sans_gd.tscn");

func _on_get_turn() -> void:
	box.change_size(Vector2(350, 140), false, 0.3);
	var base = attacks.add_attack(attack) as AttackBase
	base.start_attack();

func on_fight_used() -> void:
	play_set_dialogue(Dialogues.new().from(["헤 내가 맞아줄꺼라 생각했어?"]));
	await finished_dialogue;
	on_fight_end.emit();
```

</details>

<details>
<summary>C++</summary>

```cpp
#include "enemy_amalgamates.h"
#include "src/mainAttacks/amalgamates/attack_amalgamates.h"
#include "env.h"

void Enemy_Amalgamates::ready() {
    attackScene = ResourceLoader::get_singleton()->load("res://Game/mainAttacks/attack_amalgamates.tscn");
}

void Enemy_Amalgamates::_on_get_turn() {
    attacks->add_attack(attackScene);
    attacks->start_attacks();
}

void Enemy_Amalgamates::on_item_used(int option) {
    box->blitter_print({ 
        String::utf8("* 엔도제니가 당신이 방금 장착한 막대기에 관심이 있는거 같다"), String::utf8("* 당신은 막대기를 던졌다")
    });
    sys->executeTrue([this]() { return !global->get_battle_text_box(); },
    [this]() {
        emit_signal("on_item_end");
    });
}
```

</details>