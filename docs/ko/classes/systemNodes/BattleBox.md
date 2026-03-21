# BattleBox

> 🔴 **사용자 생성 불가능**
>> 사용자가 직접 생성할 수 없고 씬을 인스턴스화하거나 시스템에서 사용되는 노드입니다.
>> 이 노드는 `res://Engine/Battle/Box/BattleBox.tscn` 씬에서 사용됩니다.

---

## 주요 역할
`BattleBox`는 전투에서 플레이어가 위치하는 공간이자 플레이어의 메뉴가 되는 노드입니다.
전투에서 중심적인 역할을 하며, 플레이어의 선택 UI와 상호작용을 관리합니다. 또한, 전투에서 플레이어의 공간으로 사용되며, 박스의 크기와 위치 변경, 폴리곤 모드, 웹 라인 생성 등 다양한 전투 효과를 제공합니다.

---

## 메서드

### `get_size() -> Vector2`
박스의 현재 크기를 반환합니다.

### `get_box_position() -> Vector2`
박스의 현재 위치를 반환합니다.

### `get_box_rotation() -> float`
박스의 현재 회전값을 반환합니다.

### `reset_box(duration: float = 0.5)`
박스의 크기, 위치, 회전을 초기값으로 되돌립니다.
`duration`으로 초기화하는 데 걸리는 시간을 설정할 수 있습니다.

### `change_size(new_size: Vector2, relative: bool = false, duration: float = 0.6)`
박스의 크기를 변경합니다.
`new_size`는 새 크기입니다.
`relative`가 `true`면, `new_size`는 현재 크기에 더해지는 상대적인 크기가 됩니다.
`duration`은 크기가 변경되는 데 걸리는 시간을 설정합니다.

### `change_position(new_position: Vector2, relative: bool = false, duration: float = 0.6)`
박스의 위치를 변경합니다.
`new_position`는 새 위치입니다.
`relative`가 `true`면, `new_position`는 현재 위치에 더해지는 상대적인 위치가 됩니다.
`duration`은 위치가 변경되는 데 걸리는 시간을 설정합니다.

### `advanced_change_size(relative_to: BattleBox.RelativePosition, new_position: Vector2, new_size: Vector2, position_relative: bool = false, size_relative: bool = false, duration: float = 0.6)`
박스의 위치와 크기를 동시에 변경하는 고급 함수입니다.
`relative_to`는 위치와 크기가 어떤 기준에 따라 변경되는지를 설정합니다.
`new_position`는 새 위치입니다.
`new_size`는 새 크기입니다.
`position_relative`가 `true`면, `new_position`는 현재 위치에 더해지는 상대적인 위치가 됩니다.
`size_relative`가 `true`면, `new_size`는 현재 크기에 더해지는 상대적인 크기가 됩니다.
`duration`은 위치와 크기가 변경되는 데 걸리는 시간을 설정합니다.

### `advanced_set_size(relative_to: BattleBox.RelativePosition, new_position: Vector2, new_size: Vector2, position_relative: bool = false, size_relative: bool = false)`
박스의 위치와 크기를 동시에 변경하는 고급 함수입니다. 이 함수는 [`advanced_change_size()`](#advanced_change_sizerelative_to-battleboxrelativeposition-new_position-vector2-new_size-vector2-position_relative-bool--false-size_relative-bool--false-duration-float--06)와 달리 변화 효과 없이 즉시 적용됩니다.
`relative_to`는 위치와 크기가 어떤 기준에 따라 변경되는지를 설정합니다.
`new_position`는 새 위치입니다.
`new_size`는 새 크기입니다.
`position_relative`가 `true`면, `new_position`는 현재 위치에 더해지는 상대적인 위치가 됩니다.
`size_relative`가 `true`면, `new_size`는 현재 크기에 더해지는 상대적인 크기가 됩니다.

### `rotate_by(rot: float, relative: bool = false, duration: float = 0.6)`
박스를 회전시킵니다.
`rot`는 회전값입니다. (단위: 도)
`relative`가 `true`면, `rot`는 현재 회전에 더해지는 상대적인 회전값이 됩니다.
`duration`은 회전하는 데 걸리는 시간을 설정합니다.

### `set_box_rotation(rot: float, relative: bool = false)`
박스의 회전을 설정합니다. 이 함수는 [`rotate_by()`](#rotate_byrot-float-relative-bool--false-duration-float--06)와 달리 변화 효과 없이 즉시 적용됩니다.
`rot`는 회전값입니다. (단위: 도)
`relative`가 `true`면, `rot`는 현재 회전에 더해지는 상대적인 회전값이 됩니다.

### `box_show()`
배틀 박스를 보이게 합니다.

### `box_hide()`
배틀 박스를 숨깁니다.

### `clear_webs()`
[`set_webs()`](#set_websn-int-separation-float---1-margin-int--0)로 생성된 보라색 선(웹)을 모두 제거합니다. 이 함수를 사용하여 보라색 선을 초기화하거나 제거할 수 있습니다.

### `set_webs(n: int, separation: float = -1, margin: int = 0)`
`SoulBattle.PURPLE` 모드를 쓰기 전 사용해야 하는 보라색 선(웹)을 설정하는 함수입니다.
이 함수를 사용하여 보라색 선의 개수, 간격, 여백을 조정할 수 있습니다.

`n`은 보라색 선의 개수입니다. `0`으로 설정하면 보라색 선이 생성되지 않습니다.
`separation`은 보라색 선 사이의 간격입니다. 음수로 설정하면 간격이 자동으로 조정됩니다.
`margin`은 보라색 선과 배틀 박스 사이의 여백입니다. 양수로 설정하면 여백이 생깁니다.

### `get_web_y_pos(id: int) -> float`
[`set_webs()`](#set_websn-int-separation-float---1-margin-int--0)로 생성된 보라색 선(웹)의 y 좌표를 반환합니다. `id`는 보라색 선의 번호로, `0`부터 시작합니다.
예를 들어, `id`가 `0`이면 첫 번째 보라색 선의 y 좌표가 반환됩니다.

### `blitter_print(texts: PackedStringArray)`
적 턴이 아닌 플레이어 턴에 텍스트를 출력하는 데 사용됩니다. `texts`는 출력할 텍스트의 배열입니다.

### `polygon_enable()`
배틀 박스의 폴리곤 모드를 활성화합니다. 폴리곤 모드에서는 배틀 박스가 다각형 형태로 변할 수 있습니다.

### `polygon_disable(box_size: Vector2, duration: float = 0.3)`
배틀 박스의 폴리곤 모드를 비활성화하고, 배틀 박스를 원래의 사각형 형태로 되돌립니다.
`box_size`는 폴리곤 모드가 비활성화될 때 배틀 박스의 크기를 설정하는 데 사용됩니다.
`duration`은 폴리곤 모드가 비활성화되는 데 걸리는 시간을 설정하는 데 사용됩니다.

### `polygon_is_enabled() -> bool`
배틀 박스의 폴리곤 모드가 활성화되어 있는지 여부를 반환합니다. 폴리곤 모드가 활성화되어 있으면 `true`를 반환하고, 그렇지 않으면 `false`를 반환합니다.

### `create_protrusion(direction: Vector2, offset: float, size: Vector2, duration: float = 0.3)`
배틀 박스에서 돌출부를 생성하는 함수입니다. 돌출부는 배틀 박스에서 박스 모양으로 튀어나오는 부분입니다.
[`polygon_enable()`](#polygon_enable)으로 폴리곤 모드를 활성화한 후에 이 함수를 사용할 수 있습니다.

`direction`은 돌출부가 튀어나올 면을 설정하는 데 사용됩니다. 예를 들어, `(1, 0)`은 오른쪽 면으로, `(0, -1)`은 위쪽 면으로 돌출부가 생성됩니다.
`offset`은 돌출부의 위치를 설정하는 데 사용됩니다.
`size`는 돌출부의 크기를 설정하는 데 사용됩니다.
`duration`은 돌출부가 생성되는 데 걸리는 시간을 설정하는 데 사용됩니다.

### `get_polygon_points() -> PackedVector2Array`
폴리곤 모드에서 현재 폴리곤의 모든 점들을 반환합니다.

### `get_vertex_position(vertex_index: int) -> Vector2`
폴리곤 모드에서 지정된 인덱스의 꼭짓점 위치를 반환합니다.

### `move_closest_point(target_point: Vector2, duration: float = 0.3) -> int`
목표 지점에 가장 가까운 꼭짓점을 찾아 이동시키는 함수입니다.
[`polygon_enable()`](#polygon_enable)으로 폴리곤 모드를 활성화한 후에 이 함수를 사용할 수 있습니다.

`target_point`는 꼭짓점을 이동시킬 목표 지점입니다.
`duration`은 꼭짓점이 이동하는 데 걸리는 시간을 설정하는 데 사용됩니다.
이 함수는 이동된 꼭짓점의 인덱스를 반환합니다.

### `move_point_by_index(vertex_index: int, target_point: Vector2, duration: float = 0.3)`
지정된 인덱스의 꼭짓점을 목표 지점으로 이동시키는 함수입니다.
[`polygon_enable()`](#polygon_enable)으로 폴리곤 모드를 활성화한 후에 이 함수를 사용할 수 있습니다.

`vertex_index`는 이동시킬 꼭짓점의 인덱스입니다.
`target_point`는 꼭짓점을 이동시킬 목표 지점입니다.
`duration`은 꼭짓점이 이동하는 데 걸리는 시간을 설정하는 데 사용됩니다.

### `move_point_by_offset(from_point: Vector2, offset: Vector2, duration: float = 0.3) -> int`
지정된 지점에서 가장 가까운 꼭짓점을 찾아 주어진 오프셋만큼 이동시키는 함수입니다.
[`polygon_enable()`](#polygon_enable)으로 폴리곤 모드를 활성화한 후에 이 함수를 사용할 수 있습니다.

`from_point`는 이동시킬 꼭짓점을 찾을 기준 지점입니다.
`offset`은 꼭짓점을 이동시킬 오프셋입니다.
`duration`은 꼭짓점이 이동하는 데 걸리는 시간을 설정하는 데 사용됩니다.
이 함수는 이동된 꼭짓점의 인덱스를 반환합니다.

### `move_multiple_points(vertex_indices: PackedInt64Array, target_points: PackedVector2Array, duration: float = 0.3)`
지정된 여러 꼭짓점을 목표 지점으로 이동시키는 함수입니다.
[`polygon_enable()`](#polygon_enable)으로 폴리곤 모드를 활성화한 후에 이 함수를 사용할 수 있습니다.

`vertex_indices`는 이동시킬 꼭짓점들의 인덱스 배열입니다.
`target_points`는 꼭짓점들을 이동시킬 목표 지점들의 배열입니다. `vertex_indices`와 순서가 일치해야 합니다.
`duration`은 꼭짓점들이 이동하는 데 걸리는 시간을 설정하는 데 사용됩니다.

### `stop_all_point_tweens()`
현재 폴리곤 모드에서 이동 중인 모든 꼭짓점들의 트윈을 즉시 중지시키는 함수입니다. 이 함수를 사용하여 모든 꼭짓점의 이동을 강제로 멈출 수 있습니다.

---

## 맴버 변수

### `wintext: String` (기본값: `"UT_VICTORY"`)
전투에서 승리했을 때 나타나는 텍스트를 설정합니다.

### `TweenEase: Tween.EaseType` (기본값: `1`)
박스 변화 함수들에 적용되는 이징 타입을 설정합니다.

### `TweenTrans: Tween.TransitionType` (기본값: `4`)
박스 변화 함수들에 적용되는 트랜지션 타입을 설정합니다.

### `morph_speed: float` (기본값: `200.0`)
폴리곤 모드에서 버텍스가 이동하는 속도를 설정합니다. 단위는 픽셀/초입니다.

### `polygon_point_count: int` (기본값: `120`)
폴리곤 모드에서 버텍스의 개수를 설정합니다. 버텍스의 개수가 많을수록 더 부드러운 모양의 폴리곤이 만들어지지만, 성능에 영향을 줄 수 있습니다.

---

## 신호

### `moved_to_buttons`
플레이어 턴일 때 버튼 선택 상태로 이동했을 때 발생합니다.
> **이 신호는 엔진 내부에서 사용하는 신호입니다.**

### `move_soul(newpos: Vector2)`
플레이어 턴일 때 소울이 이동할 때 발생합니다. `newpos`는 소울이 이동할 위치입니다.
> **이 신호는 엔진 내부에서 사용하는 신호입니다.**

### `exit_menu`
플레이어 턴일 때 메뉴에서 나올 때 발생합니다.
> **이 신호는 엔진 내부에서 사용하는 신호입니다.**

### `act(target: int, option: int)`
플레이어 턴일 때 행동 메뉴에서 특정 행동을 선택했을 때 발생합니다. `target`는 선택한 행동의 대상 인덱스입니다. `option`는 선택한 행동의 옵션 인덱스입니다.
> **이 신호는 엔진 내부에서 사용하는 신호입니다.**

### `fight(target: int)`
플레이어 턴일 때 싸우기 메뉴에서 특정 대상을 선택했을 때 발생합니다. `target`는 선택한 대상 인덱스입니다.
> **이 신호는 엔진 내부에서 사용하는 신호입니다.**

### `item(item_choice: int)`
플레이어 턴일 때 아이템 메뉴에서 특정 아이템을 선택했을 때 발생합니다. `item_choice`는 선택한 아이템의 인덱스입니다.
> **이 신호는 엔진 내부에서 사용하는 신호입니다.**

### `mercy(target: int)`
플레이어 턴일 때 자비 메뉴에서 특정 대상을 선택했을 때 발생합니다. `target`이 `0`이면 '자비'를 선택한 것이고, `1`이면 '도망'을 선택한 것입니다.
> **이 신호는 엔진 내부에서 사용하는 신호입니다.**

### `tween_finished`
박스 변형 애니메이션이 완료되었을 때 발생합니다.

### `blitter_end`
[`blitter_print()`](#blitter_printtexts-packedstringarray) 메서드로 출력된 텍스트가 모두 출력되었을 때 발생합니다.

---

## 🔗 관련 문서

- [Enemy - 적 노드](/classes/userNodes/Enemy.md)
- [AttackBase - 공격 패턴](/classes/userNodes/AttackBase.md)
- [Bullet - 기본 탄환](/classes/userNodes/Bullet.md)
- [전투 만들기](/tutorials/battle.md)