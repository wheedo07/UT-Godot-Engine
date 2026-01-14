# Enemy

> 🟢 **사용자 생성 가능**

UT-Godot-Engine에서 전투 시스템의 적을 구현하는 핵심 클래스입니다.
턴 기반 전투, 대화, 애니메이션, 상태 관리 등 Undertale 스타일의 적 행동을 모두 처리합니다.

---

## 가상 함수 시스템

Enemy는 플레이어의 행동에 반응하는 가상 함수들을 제공합니다.
on_*_used 함수들과 on_defeat 함수는 끝에 반드시 시그널을 발생시켜야 합니다.

### `_on_get_turn()`
적의 턴이 시작될 때 호출됩니다.

```gdscript
func _on_get_turn():
    # 박스 크기 조정
    box.change_size(Vector2(350, 140), false, 0.3)
    await get_tree().create_timer(0.4).timeout
    
    # 공격 패턴 실행
    var attack = attacks.add_attack(preload("res://attacks/basic_bullets.tscn"))
    attack.start_attacks()
```

### `_on_end_turn()`
적의 턴이 종료될 때 호출됩니다.

```gdscript
func _on_end_turn():
    $sprites.play("idle")
```

### `on_fight_used(miss: bool)`
플레이어가 FIGHT를 선택했을 때 호출됩니다.

```gdscript
func on_fight_used(miss: bool):
    if miss:
        play_dialogue(1)  # "그정도로는 날 맞출 수 없다고"
    else:
        play_dialogue(0)  # "오 날 맞췄군!"
    await finished_dialogue
    emit_signal("on_fight_end")  # 필수!
```

### `on_act_used(option: int)`
플레이어가 ACT를 선택했을 때 호출됩니다.

```gdscript
func on_act_used(option: int):
    match option:
        0:
            box.blitter_print([ "* 당신은 적을 관찰했다." ]);
        1:
            box.blitter_print([ "* 당신은 적을 도발했다." ]);
            change_state(1)   # 상태 변경
        2:
            box.blitter_print([ "* 당신은 적과 농담을 주고받았다." ]);
            change_state(2)   # 상태 변경
    await box.blitter_end;
    emit_signal("on_act_end")  # 필수!
```

### `on_item_used(option: int)`
플레이어가 ITEM을 사용했을 때 호출됩니다.

```gdscript
func on_item_used(option: int):
    # 인벤토리에서 사용된 아이템 정보 확인
    var used_item = global.items[option]
    emit_signal("on_item_end")  # 필수!
```

### `on_mercy_used()`
플레이어가 MERCY를 선택했을 때 호출됩니다.

```gdscript
func on_mercy_used():
    if can_spare():  # 자비 조건 확인
        play_dialogue(6)  # "좋아 자비를 받아들이마!"
        await finished_dialogue
        main.spare_enemy(enemy_id)
    else:
        play_dialogue(7)  # "미안하지만 난 그럴 수 없어."
        await finished_dialogue
    emit_signal("on_mercy_end")  # 필수!

func can_spare() -> bool:
    # 자비 조건 로직 (예: 특정 ACT 수행 후)
    return current_state >= 2
```

### `on_defeat(death: bool)`
적이 패배했을 때 호출됩니다.

```gdscript
func on_defeat(death: bool):
    if death:
        # 죽음 처리
        play_dialogue(8)  # "크윽 ... 나는 졌다..."
        await finished_dialogue
    else:
        # 자비 처리
        play_dialogue(9)  # "너는 착한 녀석이구나"
        await finished_dialogue
    
    emit_signal("on_defeat_end")  # 필수! (호출시 자동으로 파티클 실행)
```

### `on_damage(amount: int)`
적이 데미지를 입을 때 호출됩니다.
`[time=X]` 태그를 사용하여 데미지 메시지의 표시 시간을 조절할 수 있습니다.

```gdscript
func on_damage(amount: int) -> String:
    # 커스텀 데미지 메시지 반환
    if amount >= 50:
        return "[time=0.5]CRITICAL HIT!"
    elif amount <= 10:
        return "HIT"
    else:
        return ""  # 기본 메시지 사용
```

---

## 능력치 및 상태 관리

### 능력치 시스템

```gdscript
# 기본 능력치 구조
stats = {
    "hp": 100,        # 현재 HP
    "max_hp": 100,    # 최대 HP
    "def": 10         # 방어력
}

# 능력치 수정
func buff_defense():
    modify_stats({"def": stats.def + 5})
    
func heal_self():
    var new_hp = min(stats.hp + 30, stats.max_hp)
    modify_stats({"hp": new_hp})
```

### 보상 시스템

```gdscript
# 기본 보상 구조
rewards = {
    "exp": 20,        # 경험치
    "gold": 15        # 골드
}
```

## 애니메이션 및 표정 시스템

### 표정 제어

```gdscript
# 표정 스프라이트 설정
expression_sprites = [
    get_node("Sprites/Body"),   # [0]: 몸체
    get_node("Sprites/Head")    # [1]: 머리 (표정)
]

# 대화 중 표정 변경
play_set_dialogue(Dialogues.new().from([
    "화난다",
    "기쁜다",
    "슬픈다"
].set_expressions([1, [0, 1], 2]))) # 표정 인덱스 지정 (2차원 배열은 [몸체, 머리] 순서)
```

---

## 🚨 주의사항 & 팁

### 권장사항

1. **반드시 시그널 발생**
   ```gdscript
   func on_fight_used(miss: bool):
       # 처리 로직...
       emit_signal("on_fight_end")  # 필수!
   ```

2. **대화 완료 대기**
   ```gdscript
   play_dialogue(0)
   await finished_dialogue  # 대화 완료까지 대기
   ```

### 피해야 할 것들

1. **시그널 발생 누락**
   ```gdscript
   func on_act_used(option: int):
       # 처리만 하고 시그널 발생 안함 → 게임 멈춤!
   ```

2. **잘못된 노드 경로**
   ```gdscript
   sprites_path = NodePath("WrongPath")  # 존재하지 않는 경로
   ```

---

## 시그널 목록

### 필수 시그널 (가상 함수에서 발생)
- `on_fight_end` - FIGHT 처리 완료
- `on_act_end` - ACT 처리 완료  
- `on_item_end` - ITEM 처리 완료
- `on_mercy_end` - MERCY 처리 완료
- `on_defeat_end` - 패배 처리 완료

### 자동 발생 시그널
- `finished_dialogue` - 대화 완료
- `changed_state` - 상태 변경
- `dodged(to_right: bool)` - 회피 발생
- `hit(damage: int)` - 피격 발생

---

## 🔗 관련 문서

- [AttackBase - 공격 패턴](/classes/userNodes/AttackBase.md)
- [Bullet - 기본 탄환](/classes/userNodes/Bullet.md)
- [Dialogues - 대화 리소스](/classes/resource/Dialogues.md)
- [EnemyState - 적 상태](/classes/resource/EnemyState.md)
- [전투 만들기](/tutorials/battle.md)