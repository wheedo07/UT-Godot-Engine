# EnemyAct

`EnemyAct` 클래스는 게임 내에서 [적 캐릭터](../userNodes/Enemy.md)의 행동(Act) 옵션을 정의하는 데 사용됩니다. 이 클래스는 각 적이 플레이어와 상호작용할 수 있는 다양한 Act 옵션을 포함하며, 각 옵션에 대한 세부 정보를 `ActInfo` 객체로 관리합니다.

## 속성

### `acts`
- **형식**: `Array<ActInfo>`
- **설명**: 이 적 캐릭터가 사용할 수 있는 Act 옵션들의 배열입니다. 각 Act 옵션은 `ActInfo` 객체로 표현됩니다.

### `sparable`
- **형식**: `bool`
- **설명**: 이 적 캐릭터가 플레이어에 의해 용서될 수 있는지 여부를 나타냅니다. true로 설정되면 플레이어가 이 적을 자비를 선택할 수 있으며 시스템에서 알아서 처리합니다.
- **기본값**: `true`


## 🔗 관련 문서

- [ActInfo](ActInfo.md)
- [Enemy - 적 캐릭터](/classes/userNodes/Enemy.md)
- [전투 만들기](/tutorials/battle.md)