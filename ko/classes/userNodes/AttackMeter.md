# AttackMeter

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

UT-Godot-Engine에서 전투 중 플레이어가 공격을 선택했을 때 나타나는 미니게임 노드입니다.

[`Encounter`](../resource/Encounter.md) 리소스의 [`BoxSet`](../resource/BoxSet.md)을 구성할 때, 이 노드로 만든 씬을 `meter_scene` 속성에 설정합니다.

---

## 메서드

### `get_base_damage() -> int`
플레이어의 공격력과 장비 공격 보너스를 합산하여 기본 공격 데미지를 반환합니다.
공격 데미지 계산 시 이 메서드를 호출하여 기본 데미지 값을 가져옵니다.

```gdscript
func _on_hit(): # 예시 함수
    var base = get_base_damage()
    var final_damage = base - enemy_def  # 방어력 적용
    damagetarget.emit(final_damage, enemy_id, false)
    queue_free()
```

---

## 멤버 변수

### 스크립트에서만 사용 가능한 변수

#### `enemy_id: int`
플레이어가 공격을 선택하고 이 노드가 인스턴스화될 때, 공격 대상 적의 ID가 저장됩니다.

#### `enemy_def: int`
플레이어가 공격을 선택하고 이 노드가 인스턴스화될 때, 공격 대상 적의 방어력 값이 저장됩니다.
방어력 값 역시 이 노드 내에서 직접 계산해야 합니다.

#### `encounter_script: EncounterScript`
이 노드를 효율적으로 제어하기 위해 만들어진 참조로, [`EncounterScript`](../systemNodes/EncounterScript.md) 클래스의 인스턴스를 가지고 있습니다.
이 참조를 통해 추가적인 커스텀 기능을 구현할 수 있습니다.

---

## 시그널

### `damagetarget(damage: int, enemy_id: int, crit: bool)`
미니게임이 성공적으로 완료되어 공격이 명중했을 때 발생하는 시그널입니다.
이 시그널을 발생시킨 후 [`queue_free()`](https://docs.godotengine.org/en/stable/classes/class_node.html#class-node-method-queue-free)를 호출하여 노드를 제거하는 것을 권장합니다.

- `damage: int` — 공격의 최종 데미지입니다. 커스텀 계산을 통해 결정합니다.
- `enemy_id: int` — 공격이 명중한 적의 ID입니다. [`enemy_id`](#enemy_id-int) 멤버 변수에 저장된 값을 그대로 사용하거나, 다른 적을 지정할 수도 있습니다.
- `crit: bool` — 공격이 크리티컬 히트인지 여부입니다. [`SlashAnimation`] 노드에 전달되어 크리티컬 히트 시 다른 애니메이션이 재생됩니다.

```gdscript
func _on_hit(is_crit: bool):
    var damage = get_base_damage() - enemy_def
    damagetarget.emit(damage, enemy_id, is_crit)
    queue_free()
```

### `missed(enemy_id: int)`
미니게임에 실패하여 공격이 빗나갔을 때 발생하는 시그널입니다.
또는 재미를 위해 [`damagetarget`](#damagetargetdamage-int-enemy_id-int-crit-bool) 대신 발생시킬 수도 있습니다.
이 시그널을 발생시킨 후 `queue_free()`를 호출하여 노드를 제거하는 것을 권장합니다.

- `enemy_id: int` — 공격이 빗나간 적의 ID입니다. [`enemy_id`](#enemy_id-int) 멤버 변수에 저장된 값을 그대로 사용하거나, 다른 적을 지정할 수도 있습니다.

```gdscript
func _on_miss():
    missed.emit(enemy_id)
    queue_free()
```

---

## 🔗 관련 문서

- [BoxSet - 전투보드 설정](../resource/BoxSet.md)
- [커스텀 미터 만들기](/tutorials/custom_meter.md)