# ActInfo

`ActInfo` 클래스는 게임 내에서 적의 행동(Act) 옵션에 대한 정보를 저장하고 관리하는 데 사용됩니다. 이 클래스는 각 Act 옵션의 이름, 설명을 저장하며 플레이어와 상호작용할 때 필요한 데이터를 제공합니다.

## 속성

### `act`
- **형식**: `String`
- **설명**: 이 Act 옵션의 이름입니다. 예를 들어, "* 살펴보기", "* 대화하기" 등이 될 수 있습니다.
- **기본값**: `"* 살펴보기"`

### `description`
- **형식**: `PackedStringArray`
- **설명**: 이 Act 옵션을 선택하면 표시되는 설명 텍스트입니다.
- **기본값**: `PackedStringArray(["* ENEMY - ATK 1 DEF 1 \n* 정보 몰루?"])`

## 메서드

#### `get_act_tr()`
- **반환 형식**: `String`
- **설명**: 현재 Act 옵션의 이름의 번역된 문자열을 반환합니다.


## 🔗 관련 문서

- [EnemyAct](EnemyAct.md)
- [Enemy - 적 캐릭터](/classes/userNodes/Enemy.md)
- [전투 만들기](/tutorials/battle.md)