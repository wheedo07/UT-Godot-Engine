# DefaultBullet

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

`DefaultBullet` 노드는 [`Bullet`](Bullet.md) 노드를 상속 받아 기본적인 탄환 동작을 구현한 클래스입니다. 이 노드를 상속하여 커스텀 탄환을 쉽게 만들 수 있습니다.

---

## 주요 역할
- 기본 탄환 동작 제공
    - 이동 (좌표로 이동, 반향으로 이동)
    - 자동 충돌 모양 생성
- 사용자 정의 탄환 구현의 기초 제공
    - 지연 발사
    - Tween 기반 애니메이션

---

## 이동 모드

### `MOVEMENT_VELOCITY` (속도 기반)
연속적인 물리 기반 이동을 처리합니다.
타켓 좌표 반향으로 멈추지 않고, 지정된 속도로 계속 이동합니다.

```gdscript
extends DefaultBullet

func ready():
    # 즉시 발사 - 속도 기반
    fire(Vector2(300, 0), MOVEMENT_VELOCITY, 200.0)  # 오른쪽으로 200px/초
```

### `MOVEMENT_TWEEN` (트윈 기반)
부드러운 애니메이션 기반 이동을 처리합니다.
타켓 좌표에 도달하면 멈춥니다.

```gdscript
extends DefaultBullet

func ready():
    # 트윈 설정
    TweenTrans = Tween.TRANS_SINE
    TweenEase = Tween.EASE_OUT
    
    # 트윈 기반 이동
    fire(Vector2(300, 200), MOVEMENT_TWEEN, 150.0)  # 거리/속도로 시간 계산
```

---

## 필수 노드 구조

```
DefaultBullet
├── BulletArea                  # 충돌 감지 영역
│   └── CollisionShape2D        # 충돌 모양 (지정 안해도 자동 생성)
```

---

## 메서드

### `fire(target, movement_type, speed, mode)`
탄환을 즉시 발사합니다.

```gdscript
# 기본 발사
fire(Vector2(100, 100), MOVEMENT_VELOCITY, 200.0)

# 모드와 함께 발사
fire(Vector2(0, 200), MOVEMENT_TWEEN, 150.0, Bullet.MODE_BLUE)

# 매개변수 설명:
# target: 목표 위치 (Vector2)
# movement_type: 이동 방식 (MOVEMENT_VELOCITY | MOVEMENT_TWEEN)
# speed: 속도 또는 시간 계산용 값 (기본: 100.0)
# mode: 탄환 모드 (기본: 이전 설정 유지)
```

#### 속도 계산 방식

**속도 기반 (MOVEMENT_VELOCITY):**
```gdscript
# speed = 실제 이동 속도 (pixels/second)
fire(Vector2(400, 0), MOVEMENT_VELOCITY, 200.0)  # 2초 후 도달
```

**트윈 기반 (MOVEMENT_TWEEN):**
```gdscript
# 시간 = 거리 / speed
var distance = position.distance_to(target)  # 예: 500px
fire(target, MOVEMENT_TWEEN, 100.0)  # 5초에 걸쳐 이동 (500/100)
```


### `queue_fire(delay, target, movement_type, speed, mode)`
지연 시간 후 탄환을 발사합니다.

```gdscript
# 1초 후 발사
queue_fire(1.0, Vector2(200, 0), MOVEMENT_VELOCITY, 150.0)

# 연속 발사 패턴
queue_fire(0.5, Vector2(100, 100), MOVEMENT_TWEEN, 200.0)
queue_fire(1.0, Vector2(-100, 100), MOVEMENT_TWEEN, 200.0)
queue_fire(1.5, Vector2(0, -100), MOVEMENT_TWEEN, 200.0)
```

> 📝 **참고**: 기존 트윈이 실행 중이면 완료 후에 지연 시간이 시작됩니다.


---

## 시그널

### `tween_finished`
트윈 이동이 완료되었을 때 발생합니다. [`MOVEMENT_TWEEN`](#movement_tween) 모드에서만 작동합니다.

```gdscript

bullet.fire(Vector2(300, 0), MOVEMENT_TWEEN, 150.0)
await bullet.tween_finished
print("탄환이 목표 지점에 도달했습니다!")
```

---

## 멤버 변수

### 에디터에서 설정 가능

#### `collision_margin: float`
충돌 영역의 여백 값입니다.
```gdscript
collision_margin = 3.0  # 스프라이트보다 3px 작은 충돌 영역
```

#### `collision_path: NodePath`
충돌 감지용 CollisionShape2D 노드의 경로입니다.
```gdscript
collision_path = NodePath("BulletArea/CollisionShape2D")
```

#### `TweenTrans: Tween.TransitionType`
트윈 전환 타입입니다.
```gdscript
TweenTrans = Tween.TRANS_SINE  # 부드러운 곡선
```

#### `TweenEase: Tween.EaseType`
트윈 이징 타입입니다.
```gdscript
TweenEase = Tween.EASE_OUT  # 끝에서 감속
```

### 스크립트에서만 사용 가능한 변수
| 속성 | 타입 | 설명 |
|------|------|------|
| target_position | Vector2 | 현재 목표 위치 |
| velocity_tween | Tween | 트윈 인스턴스 (트윈 모드에서만 사용) |

---

## ⚠️ 주의사항 & 팁

1. **경로 설정 누락**
```gdscript
# 설정을 안할시 크러시가 발생할 수 있습니다.
# ✅ 올바른 경로 설정
collision_path = NodePath("BulletArea/CollisionShape2D")
```

2. **잘못된 CollisionShape2D가 아닌 CollisionPolygon2D 사용**
```gdscript
# ❌ 잘못된 노드 사용
collision_path = NodePath("BulletArea/CollisionPolygon2D")  # X
```

- `MOVEMENT_VELOCITY` 모드는 지속적인 이동에 적합합니다. 멈추지 않고 계속 움직입니다.
- `MOVEMENT_TWEEN` 모드는 특정 지점으로 부드럽게 이동할 때 유용합니다.
- 트윈 이동 중에 `fire()`를 다시 호출하면 기존 트윈이 취소되고 새 이동이 시작됩니다.

---

## 🔗 관련 문서

- [Bullet 클래스](Bullet.md) - 탄환의 기본 동작과 데미지 모드 시스템
- [AttackBase 클래스](AttackBase.md) - 탄환 생성 및 공격 패턴 관리
- [BulletArea - 탄환 충돌 영역](BulletArea.md)
- [전투 만들기](/tutorials/battle.md)