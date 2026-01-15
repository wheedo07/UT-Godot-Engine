# BattleObject

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

`BattleObject` 노드는 [`Bullet`](Bullet.md) 노드 처럼 전투 시스템에서 [`AttackBase`](AttackBase.md) 노드에 의해 생성되고 관리되는 전투 오브젝트의 기본 클래스입니다. <br>
[`Bullet`](Bullet.md) 노드와 다르게 특별한 기능이 없으며, 전투 오브젝트를 커스텀으로 만들고자 할 때 이 클래스를 상속받아 사용합니다.
이 노드도 [`add_bullet()`](AttackBase.md#add_bulletbullet-mask) 등의 메서드를 통해 전투에 추가할 수 있습니다.

## 메서드

### `fade()`
탄환을 페이드 아웃시킵니다. 이 함수는 보통 [`end_attack()`](AttackBase.md#remove_bullets)에서 호출됩니다.

## 🔗 관련 문서

- [Bullet](Bullet.md)
- [AttackBase](AttackBase.md)
- [전투 만들기](/tutorials/battle.md)