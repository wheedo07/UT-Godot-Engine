# BulletArea

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

`BulletArea` 노드는 탄환의 충돌 영역을 정의하는 데 사용되는 `Area2D` 기반 클래스입니다.  
[`Bullet`](Bullet.md) 노드에서 `Area2D` 대신 사용됩니다.

---

## 주요 역할
- 충돌 감지
    - 플레이어와의 충돌 처리
- 자동으로 시스템 연동
    - 탄환의 데미지 모드 시스템과 연동
    - 충돌 시 탄환의 콜백 메서드 호출

---

## 필수 노드 구조

```
Bullet
├── BulletArea (bullet_path: "../") # 충돌 영역
│   └── CollisionShape2D        # 충돌 모양
```

---

## 메서드

### `is_player_hit()`
플레이어와 충돌했는지 여부를 반환합니다.

```gdscript
if bullet_area.is_player_hit():
    print("플레이어와 충돌!")
```

---


## 멤버 변수

### 에디터에서 설정 가능한 변수

#### `bullet_path: NodePath<Bullet>`
- BulletArea가 속한 Bullet 노드의 경로입니다.
- 꼭 설정해야 합니다.

---

## 🔗 관련 문서

- [Bullet - 기본 탄환](Bullet.md)
- [AttackBase - 공격 패턴](AttackBase.md)
- [Enemy - 적 캐릭터](Enemy.md)
- [전투 만들기](/tutorials/battle.md)