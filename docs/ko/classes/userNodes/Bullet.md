# Bullet

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

UT-Godot-Engine의 전투 시스템에서 사용되는 탄환(Bullet) 노드입니다.  
Undertale 스타일의 탄환 움직임, 데미지, 효과 모드 등을 구현할 수 있습니다.

---

## 데미지 모드 시스템

Undertale의 핵심 메커니즘인 색상별 탄환 시스템을 구현합니다:

| 모드 | 색상 | 효과 | 사용 예시 |
|------|------|------|----------|
| `MODE_WHITE` | ⚪ 흰색 | 항상 데미지 | 일반 탄막 |
| `MODE_GREEN` | 🟢 초록색 | HP 회복 | 힐링 탄환 |
| `MODE_BLUE` | 🔵 파란색 | 움직이면 데미지 | 정지 퍼즐 |
| `MODE_ORANGE` | 🟠 주황색 | 멈추면 데미지 | 이동 퍼즐 |

### 색상 자동 변경
```gdscript
# 모드 설정 시 색상 자동 적용
bullet.set_mode(Bullet.MODE_BLUE)  # 파란색으로 변경
bullet.set_mode(Bullet.MODE_GREEN) # 초록색으로 변경
```

---

## 이동 시스템

### MovementMode 종류

#### `MOVEMENT_VELOCITY` (물리 기반)
목표 반향으로 멈추지 않고 지속적으로 이동합니다.

#### `MOVEMENT_TWEEN` (애니메이션 기반)
트윈을 사용하여 지정된 위치로 부드럽게 이동합니다.
목표 위치에 도달하면 자동으로 멈춥니다.

---

## 필요한 노드 구조
```
Bullet
├── Sprite2D                    # 탄환 스프라이트
├── BulletArea # 충돌 영역
│   └── CollisionShape2D        # 충돌 모양
└── VisibleOnScreenNotifier2D   # 화면 밖 감지 (선택 사항) <- (on_exit_screen 함수를 연결)
```

---

## 메서드

### `ready()` (virtual)
탄환 초기화 함수입니다.

```gdscript
extends Bullet

func ready():
    # 기본 설정
    damage = 15
    set_mode(Bullet.MODE_BLUE)
    sprite_path = NodePath("Sprite2D") # 보통 에디터에서 설정
    
    # 생성시 특수 효과 추가
    setup_custom_effects()

func setup_custom_effects():
    # 회전 효과
    var tween = create_tween()
    tween.set_loops()
    tween.tween_property(self, "rotation", rotation + PI * 2, 2.0)
```

### `on_hit_player()` (virtual)
플레이어와 충돌했을 때 호출됩니다.

```gdscript
extends Bullet

func on_hit_player():
    # 특별한 충돌 효과
    print("플레이어가 탄환에 맞았다!")
    
    # 폭발 효과 생성
    create_explosion_effect()
    
    # 추가 탄환 생성
    spawn_child_bullets()
    
    # 탄환 제거
    kill()

func create_explosion_effect():
    var explosion = preload("res://effects/explosion.tscn").instantiate()
    get_parent().add_child(explosion)
    explosion.global_position = global_position

func spawn_child_bullets():
    for i in range(4):
        var child_bullet = preload("res://bullets/small_bullet.tscn").instantiate()
        get_parent().add_child(child_bullet)
        child_bullet.global_position = global_position
        var angle = (PI * 2 / 4) * i
        child_bullet.velocity = Vector2(cos(angle), sin(angle)) * 100
```

### `on_hit_yellow()` (virtual)
노란색 영혼의 탄환에 맞았을 때 호출됩니다.

```gdscript
extends Bullet

func on_hit_yellow():
    # 노란색 탄환에 파괴됨
    print("노란색 탄환에 파괴되었다!")
    kill()
```

### `on_hit_player_shield()` (virtual)
초록색 영혼의 방패에 막혔을 때 호출됩니다.

```gdscript
extends Bullet

func on_hit_player_shield():
    # 방패에 막힘
    print("방패에 막혔다!")
    kill()  # 탄환 제거
```

### `_physics_update(delta: float)` (virtual)
물리 프레임마다 호출되는 커스텀 업데이트 함수입니다. <br>
`_physics_process`를 오버라이드하지 말고 이 메서드를 사용하세요.

```gdscript
extends Bullet

func _physics_update(delta: float):
    # _physics_process 대체
    pass;
```

### `kill()`
탄환을 안전하게 제거합니다.

```gdscript
bullet.kill()
```

### `set_mode(mode: DamageMode)`
탄환의 데미지 모드를 설정합니다.

```gdscript
bullet.set_mode(Bullet.MODE_ORANGE)  # 주황색 모드 설정
```

### `get_mode() -> DamageMode`
현재 탄환의 데미지 모드를 반환합니다.

```gdscript
var current_mode = bullet.get_mode()
print("현재 모드: ", current_mode)
```

### `fade()`
탄환을 페이드 아웃시킵니다. 이 함수는 보통 [`end_attack()`](AttackBase.md#remove_bullets)에서 호출됩니다.

```gdscript
bullet.fade()
```

### `on_exit_screen()`
`VisibleOnScreenNotifier2D` 노드의 `screen_exited` 시그널에 연결하여 화면 밖으로 나갔을 때 탄환을 제거하게 할 수 있습니다.

```gdscript
func _on_visible_on_screen_notifier_2d_screen_exited():
    queue_free()  # 탄환 제거
```

---

## 멤버 변수

### 에디터에서 설정 가능한 변수

#### `damage: int`
탄환이 입히는 데미지 양입니다.

#### `iframe_grant`
탄환이 플레이어에게 부여하는 무적 프레임 수입니다.

#### `kr_amount: float`
탄환이 플레이어에게 가하는 kr(Karma) 양입니다. 서서히 HP를 감소시키는 효과 입니다

#### `delete_upon_hit: bool`
플레이어와 충돌 시 탄환을 자동으로 제거할지 여부입니다. 값이 `true`이면 [`on_hit_player()`](#on_hit_player-virtual) 가 호출이 안됩니다.

#### `sprite_path: NodePath`
탄환 스프라이트의 경로입니다. 색상 변경에 필요합니다. **(필수 설정)**

#### `colors: Array<Color>`
탄환의 색상 배열입니다. <br>
인덱스 순서: [0]=흰색, [1]=초록색, [2]=파란색, [3]=주황색 <br>
기본값: [Color.WHITE, Color.GREEN, Color(0, 0.85, 1), Color(1, 0.65, 0)]

### 스크립트에서만 사용 가능한 변수
| 변수명 | 타입 | 설명 |
|--------|------|------|
| `velocity_tween` | `Tween` | `MOVEMENT_TWEEN` 모드에서 사용하는 트윈 노드입니다. |


---

## ⚠️ 주의사항

### ✅ 권장사항

1. **스프라이트 경로 설정**
   ```gdscript
   func ready():
       sprite_path = NodePath("Sprite2D")  # 색상 변경을 위해 필수
   ```

2. **적절한 충돌 처리**
   ```gdscript
   # 일반적인 경우
   delete_upon_hit = true
   
   # 특별한 상호작용이 필요한 경우
   delete_upon_hit = false
   
   func on_hit_player():
       # 커스텀 로직
       kill()  # 수동으로 제거
   ```

3. **성능 최적화**
   ```gdscript
   # 화면 밖 자동 제거
   func _on_visible_on_screen_notifier_2d_screen_exited():
       queue_free()
   ```

### ❌ 피해야 할 것들

1. **`_physics_process` 오버라이드 금지**
   ```gdscript
   # ❌ 이렇게 하지 마세요!
   func _physics_process(delta):
       # 엔진 내부 로직과 충돌
   
   # ✅ 대신 이렇게 하세요
   func _physics_update(delta):
       # 커스텀 물리 로직
   ```

2. **스프라이트 경로 누락**
   ```gdscript
   # ❌ 색상 변경이 작동하지 않음
   # sprite_path 설정 없음
   
   # ✅ 올바른 설정
   sprite_path = NodePath("Sprite2D")
   ```

---

## 🔗 관련 문서

- [AttackBase - 공격 패턴](/classes/userNodes/AttackBase.md)
- [BulletArea - 탄환 영역](/classes/userNodes/BulletArea.md)
- [Enemy - 적 캐릭터](/classes/userNodes/Enemy.md)
- [전투 만들기](/tutorials/battle.md)