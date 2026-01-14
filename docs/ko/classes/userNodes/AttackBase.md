# AttackBase

> 🟢 **사용자 생성 가능**
>>  사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

AttackBase는 UT-Godot-Engine에서 적의 공격 패턴을 만들때 상속하여 사용하는 기본 클래스입니다.

---

## 마스킹 시스템

AttackBase는 4가지 렌더링 모드를 제공합니다:

| 모드 | 설명 | 위치 기준 | 클리핑 | 사용 예시 |
|------|------|-----------|--------|----------|
| `ABSOLUTE_CLIP` | 절대 위치 + 박스 클리핑 | 화면 절대 좌표 | ✅ | 일반적인 탄막 |
| `ABSOLUTE` | 절대 위치 + 클리핑 없음 | 화면 절대 좌표 | ❌ | 박스 밖에서 공격 |
| `RELATIVE_BOX_CLIP` | 박스 상대 위치 + 클리핑 | 박스 기준 상대 | ✅ | 박스 안에서 공격 |
| `RELATIVE_BOX` | 박스 상대 위치 + 클리핑 없음 | 박스 기준 상대 | ❌ | 박스을 기준으로 밖에서 공격 |

> ⚠️ **주의**: `ABSOLUTE` 모드는 투명 모드에서 사용할 수 없습니다.

---

## 메서드

### `ready()` (virtual)
```gdscript
func ready():
    # 공격 패턴 초기화
    # 탄환 씬 로딩, 변수 초기화 등
    pass
```
- 공격 패턴 초기화 함수
- 전투 시스템 컴포넌트들은 이미 초기화된 상태

### `start_attack()` (virtual)
```gdscript
func start_attack():
    # 실제 공격 로직 구현
    # 반드시 end_attack() 호출로 마무리
    await get_tree().create_timer(3.0).timeout
    end_attack()
```
- 실제 탄환 생성 로직 구현
- 박스 크기 조정, 타이밍 제어 등
- **완료 시 반드시 `end_attack()` 호출**

---

### `quick_bullet(bullet_scene, pos, rot_deg, mask)`
**가장 자주 사용되는** 탄환 생성 메서드입니다.

```gdscript
# 기본 탄환 생성
var bullet = quick_bullet(bullet_scene, Vector2(100, 50))

# 회전된 탄환 생성  
var bullet = quick_bullet(bullet_scene, Vector2(100, 50), 45.0)

# 마스킹 모드 지정
var bullet = quick_bullet(bullet_scene, Vector2(100, 50), 0, AttackBase.RELATIVE_BOX_CLIP)
```

**매개변수**:
- `bullet_scene`: 탄환 PackedScene
- `pos`: 탄환 위치 (Vector2)
- `rot_deg`: 회전각 (도 단위, 기본값: 0)
- `mask`: 마스킹 모드 (기본값: ABSOLUTE_CLIP)

**반환값**: 생성된 탄환 노드 (실패 시 null)

### `add_bullet(bullet, mask)`
일반적으로 사용되는 탄환 추가 메서드입니다.

```gdscript
var bullet = bullet_scene.instantiate()
bullet.position = Vector2(100, 50)
add_bullet(bullet, AttackBase.RELATIVE_BOX_CLIP)
```

### `end_attack()`
**모든 공격 패턴은** 완료 시 이 메서드를 호출해야 합니다.

```gdscript
func start_attack():
    # 공격 로직...
    await get_tree().create_timer(2.0).timeout
    end_attack()  # 필수!
```

자동으로 다음을 수행합니다:
- [`remove_bullets`](#remove_bullets) 시그널 발생 → 모든 탄환 제거
- [`remove_attack`](#remove_attack) 시그널 발생 → 자신을 제거

---

## 멤버 변수
| 속성 | 타입 | 설명 |
|------|------|------|
| `attack_id` | `int` | 공격 패턴 ID |
| `main` | `BattleMain` | 전투 메인 노드 |
| `box` | `BattleBox` | 전투 박스 (소울 영역) |
| `soul` | `SoulBattle` | 플레이어 소울 |
| `camera` | `CameraController` | 카메라 컨트롤러 |
| `mask` | `AttackManager` | 공격 매니저 |

---

## 시그널

### `remove_bullets`
- 모든 탄환 제거 시그널
- 공격 패턴 완료 시 자동 발생 ([`end_attack()`](#end_attack) 호출 시)

### `remove_attack`
- 공격 패턴(현재 노드) 제거 시그널
- 공격 패턴 완료 시 자동 발생 ([`end_attack()`](#end_attack) 호출 시)

---

## 🔗 관련 문서

- [Enemy - 적 캐릭터](/classes/userNodes/Enemy.md)
- [Bullet - 기본 탄환](/classes/userNodes/Bullet.md)
- [전투 만들기](/tutorials/battle.md)