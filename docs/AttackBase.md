# AttackBase 노드

UT-Godot-Engine에서 전투 시스템의 공격 패턴을 구현하는 베이스 클래스입니다.
모든 공격 패턴은 이 클래스를 상속받아 구현되며, 탄환 생성, 배치, 마스킹 등의 핵심 기능을 제공합니다.

---

## 📌 주요 기능

* 탄환 생성 및 배치 시스템
* 마스킹 기반 렌더링 제어
* 공격 패턴 생명주기 관리
* 전투 시스템 컴포넌트 접근

---

## 시그널 (Signals)

| 시그널명 | 인자 | 설명 |
| --- | --- | --- |
| `remove_attack` | `(id: int)` | 공격이 끝났을 때 발생하는 시그널 |
| `remove_bullets` | - | 모든 탄환을 제거하라는 실행 시그널  |

---

## ✅ 속성 (Properties)

| 속성명 | 타입 | 설명 |
| --- | --- | --- |
| `attack_id` | `int` | 공격 패턴의 고유 ID |
| `main` | `BattleMain` | 전투 시스템 메인 객체 (읽기 전용) |
| `box` | `BattleBox` | 전투 박스 객체 (읽기 전용) |
| `soul` | `SoulBattle` | 플레이어 영혼 객체 (읽기 전용) |
| `mask` | `AttackManager` | 마스킹된 공격 컨테이너 (읽기 전용) |
| `camera` | `CameraRemoteController` | 카메라 컨트롤러 (읽기 전용) |

---

## 열거형 (Enums)

### Masking

탄환 렌더링 위치와 클리핑 방식을 결정합니다.

| 이름 | 설명 |
| --- | --- |
| `ABSOLUTE_CLIP` | 절대 위치 + 박스 영역으로 클리핑 (기본값) |
| `ABSOLUTE` | 절대 위치 + 클리핑 없음 |
| `RELATIVE_BOX_CLIP` | 박스 기준 상대 위치 + 클리핑 |
| `RELATIVE_BOX` | 박스 기준 상대 위치 + 클리핑 없음 |

---

## 🔧 사용 가능한 메서드

### 가상 함수 (오버라이드 필수)

| 함수명 | 설명 |
| --- | --- |
| `ready()` | 공격 패턴 초기화 함수 |
| `start_attack()` | 공격 시작 함수 |

### 탄환 관리 함수

| 함수명 | 설명 |
| --- | --- |
| `add_bullet(bullet: Node, mask: Masking)` | 탄환 노드를 씬에 추가 |
| `quick_bullet(scene: PackedScene, pos: Vector2, rot: float, mask: Masking)` | 탄환 씬을 빠르게 생성하고 배치 |
| `end_attack()` | 공격 종료 (모든 탄환 제거 및 자신 제거) |

---

## 💡 사용 예시

### GDScript 공격 패턴

```gdscript
extends AttackBase

@export var bullet_scene: PackedScene

func ready():
    # 공격 초기화
    pass

func start_attack():
    # 박스 크기 조정
    box.change_size(Vector2(200, 200), false, 0.5)
    
    # 0.5초 후 탄환 생성 시작
    await get_tree().create_timer(0.5).timeout
    create_bullet_pattern()

func create_bullet_pattern():
    # 원형으로 탄환 배치
    var center = Vector2(320, 240)
    var bullet_count = 8
    
    for i in range(bullet_count):
        var angle = (float(i) / bullet_count) * PI * 2.0
        var pos = center + Vector2(cos(angle), sin(angle)) * 100
        
        # 탄환 생성
        var bullet = quick_bullet(bullet_scene, pos, rad_to_deg(angle))
        bullet.velocity = Vector2(cos(angle), sin(angle)) * 200
        
        await get_tree().create_timer(0.1).timeout
    
    # 3초 후 공격 종료
    await get_tree().create_timer(3.0).timeout
    end_attack()
```

### C++ 공격 패턴

```cpp
#include "my_attack.h"

void MyAttack::ready() {
    bullet_scene = ResourceLoader::get_singleton()->load("res://bullets/basic_bullet.tscn");
}

void MyAttack::start_attack() {
    // 박스 설정
    box->change_size(Vector2(300, 150), false, 0.3f);
    
    // 타이머로 패턴 시작
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(0.5f);
    timer->connect("timeout", Callable(this, "spawn_bullets"));
}

void MyAttack::spawn_bullets() {
    Vector2 spawn_pos = Vector2(100, 100);
    
    for (int i = 0; i < 5; i++) {
        Node* bullet = quick_bullet(bullet_scene, spawn_pos, 0, ABSOLUTE_CLIP);
        if (bullet) {
            bullet->call("set_velocity", Vector2(200, 0));
        }
        spawn_pos.y += 50;
    }
    
    // 공격 종료
    Ref<SceneTreeTimer> end_timer = get_tree()->create_timer(4.0f);
    end_timer->connect("timeout", Callable(this, "end_attack"));
}
```

### 고급 탄환 배치 예시

```gdscript
extends AttackBase

func start_attack():
    create_spiral_pattern()

func create_spiral_pattern():
    var center = box.get_size() / 2
    var bullets_per_arm = 10
    var arms = 4
    
    for arm in range(arms):
        for i in range(bullets_per_arm):
            var progress = float(i) / bullets_per_arm
            var angle = (float(arm) / arms) * PI * 2.0 + progress * PI * 4.0
            var radius = progress * 80
            
            var pos = center + Vector2(cos(angle), sin(angle)) * radius
            
            # 박스 상대 위치로 배치 (박스가 움직여도 따라감)
            var bullet = quick_bullet(bullet_scene, pos, 0, RELATIVE_BOX_CLIP)
            
            await get_tree().create_timer(0.05).timeout
    
    await get_tree().create_timer(2.0).timeout
    end_attack()
```

### 마스킹 모드별 활용

```gdscript
func demonstrate_masking():
    var center = Vector2(320, 240)
    
    # 박스 밖에도 표시되는 탄환
    quick_bullet(bullet_scene, center + Vector2(-200, 0), 0, ABSOLUTE)
    
    # 박스 영역으로 잘리는 탄환
    quick_bullet(bullet_scene, center + Vector2(200, 0), 0, ABSOLUTE_CLIP)
    
    # 박스 기준 상대 위치 (박스가 움직이면 따라감)
    quick_bullet(bullet_scene, Vector2(50, 50), 0, RELATIVE_BOX)
    
    # 박스 기준 + 클리핑
    quick_bullet(bullet_scene, Vector2(50, 100), 0, RELATIVE_BOX_CLIP)
```

---

## 주의사항

- `ready()`와 `start_attack()` 함수는 반드시 구현해야 합니다
- 투명 모드에서는 `ABSOLUTE` 계열 마스킹을 사용할 수 없습니다
- 공격이 끝나면 반드시 `end_attack()`을 호출해야 합니다
- 탄환에 `shake_camera` 시그널이 있으면 자동으로 카메라와 연결됩니다
- 탄환에 `fade()` 메서드가 있으면 `remove_bullets` 시그널과 자동 연결됩니다

---

## 관련 클래스

- [`Enemy`](Enemy.md): 적 클래스에서 공격 패턴 호출
- [`AttackManager`](AttackManager.md): 공격 패턴 관리자
- [`BattleMain`](BattleMain.md): 전투 시스템 메인
- [`Bullet`](Bullet.md): 탄환 클래스
- [`BattleBox`](BattleBox.md): 전투 박스 시스템