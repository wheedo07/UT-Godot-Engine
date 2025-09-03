# BulletArea 노드

UT-Godot-Engine에서 탄환의 충돌 감지를 처리하는 전용 노드입니다.
[`Bullet`](Bullet.md) 노드와 함께 사용되어 플레이어와의 충돌, 데미지 처리, 무적 프레임 등을 관리합니다.
스크립트를 넣는 노드가 아니라 `Bullet` 노드에서 `Area2D` 노드 대신 사용되는 노드입니다

---

## 📌 주요 기능

* 탄환과 플레이어 간의 충돌 감지
* 데미지 모드에 따른 충돌 처리
* 노란색 총알 특수 충돌 처리
* 자동 데미지/무적프레임/KR 값 동기화

---

## ✅ 속성 (Properties)

| 속성명 | 타입 | 설명 |
| --- | --- | --- |
| `bullet_path` | `NodePath` | 연결할 Bullet 노드의 경로 |

---

## 💡 사용 예시

### 기본 설정

1. `bullet_path`에 연결할 [`Bullet`](Bullet.md) 노드 경로 설정
2. 충돌 영역을 위한 `CollisionShape2D` 자식 노드 추가

### 씬 구조 예시

```
Bullet (Bullet 스크립트)
├── Sprite2D
├── BulletArea (bullet_path: "../")
│   └── CollisionShape2D
└── ...
```

### 주의사항

- `bullet_path`는 반드시 유효한 [`Bullet`](Bullet.md) 노드를 가리켜야 합니다
- 충돌 감지는 자동으로 처리되므로 별도 설정이 필요하지 않습니다

---