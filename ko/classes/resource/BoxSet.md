# BoxSet

`BoxSet` 리소스는 [`Encounter`](Encounter.md) 리소스에 사용되는 리소스로 전투보드의 색상, 커스텀 미터 또는 기본 미터 설정을 구성하는 데 사용됩니다. 

---

## 속성

### `board_color`
- **타입**: `Color`
- **설명**: 전투보드의 배경 색상입니다.

### `board_border_color`
- **타입**: `Color`
- **설명**: 전투보드의 테두리 색상입니다.

### `is_custom_meter`
- **타입**: `bool`
- **설명**: 커스텀 미터를 사용할지 여부입니다.  
기본 미터는 플레이어가 공격 타이밍을 맞추는 시스템이지만, 커스텀 미터는 개발자가 자신만의 미터 시스템을 구현할 수 있도록 합니다.

### `meter_scene`
- **타입**: `Ref<PackedScene>`
- **설명**: 커스텀 미터로 사용할 씬입니다.  
`is_custom_meter`가 `true`로 설정된 경우 이 씬이 미터로 사용됩니다. root 노드가 [`AttackMeter`](../userNodes/AttackMeter.md) 노드여야 합니다.

### `meter_texture`
- **타입**: `Ref<Texture>`
- **설명**: 기본 미터를 사용할 때 미터에 적용할 텍스처입니다.

### `meter_scale`
- **타입**: `Vector2`
- **설명**: 기본 미터를 사용할 때 미터의 크기를 조절하는 스케일입니다.

---

## 🔗 관련 문서
- [Encounter](Encounter.md)
- [BattleBox](/classes/systemNodes/BattleBox.md)
- [AttackMeter](/classes/userNodes/AttackMeter.md)
- [커스텀 미터 만들기](/tutorials/custom_meter.md)