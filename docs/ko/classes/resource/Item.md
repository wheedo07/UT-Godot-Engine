# Item

`Item` 클래스는 이 엔진에서 아이템을 정의하는 리소스입니다.

---

## 속성

### `item_name: string`
아이템의 이름입니다. 이 이름은 플레이어한테 아이템 이름을 표시할 때 사용됩니다.

### `item_information: PackedStringArray`
`UI`에서 `정보` 버튼을 눌렀을 때 표시되는 아이템의 상세 정보입니다.

### `throw_message: PackedStringArray`
`UI`에서 `버리기` 버튼을 눌렀을 때 표시되는 메시지입니다.

### `use_message: PackedStringArray`
이 아이템을 사용할 때 표시되는 메시지입니다.

### `is_consumable: bool`
이 아이템이 소모품인지 여부입니다. `true`로 설정된 경우 아이템이 사용된 후 인벤토리에서 제거됩니다.

## 메서드

### `get_item_name_tr()`
- **반환 형식**: `String`
- **설명**: 현재 아이템의 이름의 번역된 문자열을 반환합니다.

--- 

## 🔗 관련 문서
- [UI](/classes/systemNodes/UI.md)
- [Global](/classes/systemNodes/Global.md)