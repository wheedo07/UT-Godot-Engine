# DefaultBullet 노드

DefaultBullet 노드는 정말 기본적인 날라기는 탄환을 만들때 주로 사용됩니다.
기본적인 탄환 발사 기능과 충돌 처리를 제공합니다.

---

## 속성 (Properties)

| 속성명               | 타입                        | 설명                                           |
| ------------------- | -------------------------- | -------------------------------------------- |
| `collision_margin`  | `float`                    | 충돌 영역 마진 (기본값: 4.0)                   |
| `collision_path`    | `NodePath`                 | 충돌 감지용 CollisionShape2D 노드 경로         |
| `TweenTrans`        | `Tween::TransitionType`    | 트윈 전환 타입 (기본값: TRANS_QUAD)           |
| `TweenEase`         | `Tween::EaseType`          | 트윈 이징 타입 (기본값: EASE_IN_OUT)          |

---

## 열거형 (Enums)

| 모드                    | 설명                                      |
| ---------------------- | --------------------------------------- |
| `MOVEMENT_VELOCITY`    | 속도 기반 이동 (안 멈춤)            |
| `MOVEMENT_TWEEN`       | 트윈 기반 이동 (부드러운 애니메이션)       |

---

## 🔧 사용 가능한 메서드

| 함수명                                              | 설명                                  |
| -------------------------------------------------- | ----------------------------------- |
| `ready()`                                         | 초기화 함수 (GDScript에서도 override 가능) |
| `fire(target, movement_type, speed = 100.0)`      | 탄환을 즉시 발사                     |
| `queue_fire(delay, target, movement_type, speed)` | 지연 시간 후 탄환 발사               |

---

## 💡 사용 예시

<details>
<summary>GDScript</summary>

```gdscript
extends Node2D

@onready var bullet_scene = preload("res://bullets/DefaultBullet.tscn")

func _ready():
    # 기본 탄환 생성
    var bullet = bullet_scene.instantiate()
    add_child(bullet)
    
    # 속성 설정
    bullet.collision_margin = 2.0
    bullet.TweenTrans = Tween.TRANS_SINE
    bullet.TweenEase = Tween.EASE_OUT
    
    # 즉시 발사 (속도 기반)
    bullet.fire(Vector2(100, 100), Bullet.MOVEMENT_VELOCITY, 200.0)
    
    # 지연 발사 (트윈 기반)
    bullet.queue_fire(2.0, Vector2(200, 200), Bullet.MOVEMENT_TWEEN, 150.0)
```

</details>

<details>
<summary>C++</summary>

```cpp
#include "defaultbullet.h"

void BattleManager::spawn_bullet() {
    DefaultBullet* bullet = memnew(DefaultBullet);
    add_child(bullet);
    
    // 속성 설정
    bullet->set_collision_margin(2.0f);
    bullet->set_tweenTrans(Tween::TRANS_SINE);
    bullet->set_tweenEase(Tween::EASE_OUT);
    
    // 즉시 발사 (속도 기반)
    bullet->fire(Vector2(100, 100), DefaultBullet::MOVEMENT_VELOCITY, 200.0f);
    
    // 지연 발사 (트윈 기반)  
    bullet->queue_fire(2.0f, Vector2(200, 200), DefaultBullet::MOVEMENT_TWEEN, 150.0f);
}
```

</details>

---

## 📋 필수 노드 구조 (예시)

```
DefaultBullet (DefaultBullet)
├── [sprite_path에 설정된 스프라이트 노드]
└── [collision_path에 설정된 CollisionShape2D 노드]
```

---

## ⚠️ 주의사항

- `sprite_path`와 `collision_path`는 반드시 설정해야 합니다
- `collision_path`는 `CollisionShape2D` 노드를 가리켜야 합니다
- `queue_fire()` 사용 시 기존 트윈이 실행 중이면 완료 후 발사됩니다
- 스프라이트 크기가 변경되면 자동으로 충돌 모양이 업데이트됩니다