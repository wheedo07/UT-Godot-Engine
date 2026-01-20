# Global

> 🔴 **사용자 생성 불가능**
>> 사용자가 직접 생성할 수 없고 씬을 인스턴스화하거나 시스템에서 사용되는 노드입니다.
>> 이 노드는 `res://Core/autoload/global.tscn` 씬에서 사용됩니다.

---

## 주요 역할
`Global` 노드는 `(res://Core/autoload/global.tscn)` 씬에서 사용되는 노드로, UT-Godot-Engine의 전역 설정 및 기능을 관리합니다. 이 노드는 게임 전반에 걸쳐 접근할 수 있는 전역 변수와 메서드를 제공하고 여러 유틸함수를 포함합니다.

---

## 메서드

### `check_level_up() -> bool`
플레이어의 경험치가 레벨업 조건을 충족하는지 확인하고, 충족할 경우 레벨업을 처리합니다.

### `disable_input(key: String)`
특정 입력 키를 비활성화합니다. `key` 매개변수는 비활성화할 입력 키의 이름입니다. <br>
ex) "ut_confirm", "ut_cancel"

### `enable_input(key: String)`
특정 입력 키를 활성화합니다. `key` 매개변수는 활성화할 입력 키의 이름입니다. <br>
ex) "ut_confirm", "ut_cancel"

### `has_input_disabled(key: String) -> bool`
특정 입력 키가 비활성화되어 있는지 확인합니다. `key` 매개변수는 확인할 입력 키의 이름입니다.
비활성화되어 있으면 true를 반환하고, 그렇지 않으면 false를 반환합니다. <br>
ex) "ut_confirm", "ut_cancel"

### `alert(message: String, title: String = "Alert")`
게임 내에서 경고 메시지 창을 표시합니다. `message` 매개변수는 표시할 메시지 내용을 나타내고, `title` 매개변수는 창의 제목을 나타냅니다. 제목은 기본값으로 "Alert"로 설정되어 있습니다.

### `save_file(slot: int, save_data: Dictionary)`
게임 데이터를 지정된 슬롯에 저장합니다.
저장 파일의 이름은 'file0~file8' 형식을 따르며, `saveDir` 매개변수에 따라 저장 위치가 결정됩니다. <br>
`slot` 매개변수는 저장 슬롯 번호를 나타내며(0~8), `save_data` 매개변수는 저장할 데이터를 포함하는 딕셔너리입니다. <br>
저장이 완료되면 `saved` 신호가 발생합니다.

### `load_file(slot: int) -> Dictionary`
지정된 슬롯에서 게임 데이터를 불러옵니다.
저장 파일의 이름은 'file0~file8' 형식을 따르며, `saveDir` 매개변수에 따라 저장 위치가 결정됩니다. <br>
`slot` 매개변수는 불러올 저장 슬롯 번호를 나타내며(0~8) 불러온 데이터는 딕셔너리 형태로 반환됩니다.

### `exists_file(slot: int) -> bool`
지정된 슬롯에 파일이 존재하는지 확인합니다. `slot` 매개변수는 확인할 저장 슬롯 번호를 나타내며(0~8) 파일이 존재하면 true를 반환하고, 그렇지 않으면 false를 반환합니다.

### `change_setting(setting_name: String, value: Variant)`
게임의 설정 값을 변경합니다. <br>
setting_name 매개변수는 변경할 설정의 이름을 나타내며 ("Master", "Music", "SFX", "border", "shake", "vfx", "locale" 중 하나) <br>
value 매개변수는 설정할 값을 나타냅니다 ("Master", "Music", "SFX"는 float; "border", "shake", "vfx"는 bool; "locale"은 String). <br>
설정 변경은 관련 시스템에 즉시 반영됩니다.

### `get_Music() -> AudioStreamPlayer`
현재 재생 중인 배경 음악(AudioStreamPlayer) 노드를 반환합니다.

### `get_battle_text_box() -> bool`
배틀 중에 텍스트 박스가 활성화되어 있는지 여부를 반환합니다. 활성화되어 있으면 true를 반환하고, 그렇지 않으면 false를 반환합니다.

### `get_frist() -> bool`
게임이 처음 시작되었는지 여부를 반환합니다. 처음 시작되었으면 true를 반환하고, 그렇지 않으면 false를 반환합니다.

### `get_fullscreen() -> bool`
게임이 전체 화면 모드인지 여부를 반환합니다. 전체 화면 모드이면 true를 반환하고, 그렇지 않으면 false를 반환합니다.

### `get_player_in_menu() -> bool`
플레이어가 메뉴 상태인지 여부를 반환합니다. 메뉴 상태이면 true를 반환하고, 그렇지 않으면 false를 반환합니다.

### `get_player_kr() -> int`
플레이어의 현재 KR(Karma) 값을 반환합니다.

### `get_player_position() -> Vector2`
플레이어의 현재 위치(Vector2)를 반환합니다.

### `get_player_text_box() -> bool`
플레이어가 텍스트 박스 상태인지 여부를 반환합니다. 텍스트 박스 상태이면 true를 반환하고, 그렇지 않으면 false를 반환합니다.

### `get_scene_container() -> SceneContainer`
현재 활성화된 씬 컨테이너([`SceneContainer`](SceneContainer.md)) 노드를 반환합니다. 씬 컨테이너는 게임의 다양한 씬을 관리하는 역할을 합니다.

### `set_flag(key: String, value: Variant)`
지정된 키에 대한 플래그 값을 설정합니다. `key` 매개변수는 플래그의 이름을 나타내고, `value` 매개변수는 설정할 값을 나타냅니다.
flag는 세이브 데이터에 저장되며, 게임 진행 상황을 추적하는 데 사용됩니다.

### `get_flag(key: String, defaultValue: Variant = false) -> Variant`
지정된 키에 대한 플래그 값을 가져옵니다. `key` 매개변수는 플래그의 이름을 나타내고, `defaultValue` 매개변수는 플래그가 존재하지 않을 경우 반환할 기본값을 나타냅니다. 플래그가 존재하면 해당 값을 반환하고, 그렇지 않으면 기본값을 반환합니다.

### `set_g_flags(key: String, value: Variant)`
지정된 키에 대한 글로벌 플래그 값을 설정합니다. `key` 매개변수는 플래그의 이름을 나타내고, `value` 매개변수는 설정할 값을 나타냅니다.
글로벌 플래그는 `undertale.ini` 파일에 저장되며, [`resetgame()`](#resetgame) 메서드를 호출해도 초기화되지 않습니다.

### `get_g_flags(key: String, defaultValue: Variant = false) -> Variant`
지정된 키에 대한 글로벌 플래그 값을 가져옵니다. `key` 매개변수는 플래그의 이름을 나타내고, `defaultValue` 매개변수는 플래그가 존재하지 않을 경우 반환할 기본값을 나타냅니다. 플래그가 존재하면 해당 값을 반환하고, 그렇지 않으면 기본값을 반환합니다.

### `save_flag(key: String, value: Variant)`
지정된 키에 대한 세이브 플래그 값을 설정합니다. `key` 매개변수는 플래그의 이름을 나타내고, `value` 매개변수는 설정할 값을 나타냅니다. <br>
[`set_flag()`](#set_flagkey-string-value-variant) 메서드와 유사하지만 함수 후출 이후 플레이어가 저장해야지 플래그가 저장되는 것과 달리 이 함수는 즉시 플래그를 저장합니다. <br>
ex) 플레이어가 저장을 안했지만 샌즈는 그 행동을 기억해야 할 때.

### `heal(amount: int)`
플레이어의 HP를 지정된 양만큼 회복합니다. `amount` 매개변수는 회복할 HP의 양을 나타냅니다.

### `isMobile() -> bool`
현재 플랫폼이 모바일인지 여부를 반환합니다. 모바일 플랫폼이면 true를 반환하고, 그렇지 않으면 false를 반환합니다.

### `save_settings()`
현재 게임 설정을 저장합니다. 여기서 설정이란 오디오 볼륨, 그래픽 설정, 컨트롤 설정 등을 말합니다.

### `toggle_fullscreen()`
게임의 전체 화면 모드를 토글합니다. 전체 화면 모드가 활성화되어 있으면 비활성화하고, 비활성화되어 있으면 활성화합니다.

### `save(room_name: String = "")`
현재 게임 상태를 자동으로 저장합니다. `room_name` 매개변수는 저장할 방의 이름을 나타내며, 기본값은 빈 문자열입니다. <br>
빈 문자열인 경우 현재 노드의 이름이 사용됩니다. <br>
수동으로 저장 포인트를 만들고 싶을 때 사용됩니다.

### `resetgame()`
게임의 플래그와 게임 데이터를 초기화합니다. 이 메서드는 게임을 처음부터 다시 시작할 때 사용됩니다. 단, 글로벌 플래그는 초기화되지 않습니다.

### `true_resetgame()`
게임의 모든 데이터를 완전히 초기화합니다. 이 메서드는 게임을 완전히 새로 시작할 때 사용됩니다. 모든 플래그와 글로벌 플래그, 세이브 슬롯이 초기화됩니다.

---

## 맴버 변수

### 에디터에서 설정 가능한 변수

#### `item_list: Array[Item]`
게임에 존재하는 모든 아이템의 목록입니다. 각 요소는 인덱스로 참조되며 아이템 ID와 일치합니다.

#### `items: Array[int]`
플레이어가 현재 보유하고 있는 아이템의 ID 목록입니다. ID란 [`item_list`](#item_list-arrayitem)에 인덱스를 말합니다.

#### `boxitems: Array[int]`
플레이어가 차원 상자의 보유하고 있는 아이템의 ID 목록입니다. ID란 [`item_list`](#item_list-arrayitem)에 인덱스를 말합니다.

#### `equipment: Dictionary`
플레이어가 현재 장착하고 있는 장비의 정보를 담고 있는 딕셔너리입니다. <br>
키는 장비 슬롯 이름(예: "weapon", "armor")이고 값은 해당 슬롯에 장착된 장비의 ID입니다. ID란 [`item_list`](#item_list-arrayitem)에 인덱스를 말합니다.

#### `cells: PackedStringArray`
[`Overworld`](../userNodes/Overworld.md)에서 플레이어가 전화를 할수 있는 전화 목록입니다. <br>
각 요소는 전화 제목을 나타내며, 인덱스는 [`start_cellphone()`](../userNodes/Overworld.md#start_cellphoneid-int-virtual) 메서드에서 사용됩니다.

#### `flags: Dictionary`
게임의 진행 상황을 추적하는 데 사용되는 플래그를 저장하는 딕셔너리입니다. <br>
키는 플래그의 이름이고 값은 해당 플래그의 값입니다. 이 변수는 주로 에디터에서 수정하고 스크립트에서는 [`set_flag()`](#set_flagkey-string-value-variant) 및 [`get_flag()`](#get_flagkey-string-defaultvalue-variant--false---variant) 등 메서드를 통해 접근하는 것이 권장됩니다.

#### `player_name: String`
플레이어의 이름을 저장하는 문자열입니다.

#### `player_attack: int`
플레이어의 공격력을 나타내는 정수입니다. 이 값은 전투에서 플레이어가 적에게 가하는 피해량에 영향을 미칩니다.

#### `player_defense: int`
플레이어의 방어력을 나타내는 정수입니다. 이 값은 전투에서 플레이어가 받는 피해량을 감소시키는 데 영향을 미칩니다.

#### `player_gold: int`
플레이어가 현재 보유하고 있는 골드의 양을 나타내는 정수입니다. 골드는 게임 내 상점에서 아이템을 구매하는 데 사용됩니다.

#### `player_hp: int`
플레이어의 현재 체력(HP)을 나타내는 정수입니다.

#### `player_max_hp: int`
플레이어의 최대 체력(HP)을 나타내는 정수입니다.

#### `player_kills: int`
플레이어가 현재까지 처치한 몬스터의 수를 나타내는 정수입니다. <br>
[`flags`](#flags-dictionary) 변수에 "isGenocide": true 플래그가 설정된 경우 이 값이 [`UI`](UI.md)에도 표시됩니다.

#### `player_exp: int`
플레이어가 현재 보유하고 있는 경험치(EXecution Point)의 양을 나타내는 정수입니다. [`check_level_up()`](#check_level_up---bool) 메서드를 통해 exp를 확인하고 레벨업을 처리할 수 있습니다.

#### `player_lv: int`
플레이어의 현재 LOVE(Level Of ViolencE) 레벨을 나타내는 정수입니다.

#### `saveDir: String`
| 예약어 | 설명 | 실제 경로 |
|--------|------|-----------|
| $home | 사용자 홈 디렉토리 | Windows: C:\Users\Username |
| $appdata | 애플리케이션 데이터 디렉토리 | Windows: C:\Users\Username\AppData\Roaming |
| $localappdata | 로컬 애플리케이션 데이터 디렉토리 | Windows: C:\Users\Username\AppData\Local |

세이브 파일이 저장되는 디렉토리 경로를 나타내는 문자열입니다. 기본값은 "user://"로 설정되어 있습니다. <br>
세이브 파일은 이 디렉토리에 file9 형식으로 저장됩니다. <br>
Windows에서는 user:// 경로외에 예약어를 사용할수 있습니다, ( $appdata, $localappdata, $home ) <br>
사용 예: "%appdata%/MyGame"

>> ⚠️ **주의**: 아직 윈도우 이외의 플랫폼에서는 예약어가 지원되지 않습니다.

#### `settings: Dictionary`
게임의 설정 값을 저장하는 딕셔너리입니다.
```json
{
    "Master": float, // 마스터 볼륨 (0.0 ~ 1.0)
    "Music": float,  // 음악 볼륨 (0.0 ~ 1.0)
    "SFX": float,    // 효과음 볼륨 (0.0 ~ 1.0)
    "border": bool,  // 창 테두리 표시 여부
    "shake": bool,   // 화면 흔들림 효과 사용 여부
    "vfx": bool,     // 시각 효과 사용 여부
    "locale": String // 언어 설정 ("auto", "en", "ko")
}
```

#### `scan_directory: String`
파일 스캔 디렉토리 경로입니다. 디버그 모드에서 사용됩니다. <br>
F1 키를 눌러 디버그 메뉴를 열 때, 나오는 드롭다운 메뉴에 이 디렉토리의 파일들이 자동으로 추가됩니다. <br>
주로 개발 중에 씬 파일을 빠르게 불러오기 위해 사용됩니다.

>> ⚠️ **주의**: (res://Core, res://Engine, res://) 디렉토리는 스캔되지 않습니다.

### 스크립트에서만 접근 가능한 변수

#### `variables: Dictionary`
전역 변수를 저장하는 딕셔너리입니다. <br>
이 변수는 규칙이 없고 자유롭게 사용할 수 있는 변수 저장소로, 스크립트에서 전역적으로 접근할 수 있습니다.

#### `overworld_data: Dictionary`
[`Overworld`](../userNodes/Overworld.md) 씬과 관련된 데이터를 저장하는 딕셔너리입니다. <br>
엔진내에서 자동으로 관리되며, 직접 수정하지 않는 것이 좋습니다.
```json
{
    "room_name": String,        // 현재 방 이름
    "room_pos": Vector2,      // 현재 방 내 플레이어 위치
    "room": String,         // 현재 방 씬 경로
}
```

#### `player_can_move: bool`
플레이어가 현재 움직일 수 있는지 여부를 나타내는 값입니다. <br>
엔진내에서 자동으로 관리되며, 직접 못움직이게 하고 싶다면 [`player_move`] 변수를 사용하세요.

#### `player_move: bool`
플레이어가 움직일 수 있는지 여부를 나타내는 값입니다. <br>
이 변수를 false로 설정하면 플레이어가 움직이지 못하게 됩니다.

#### `player_set_menu: bool`
플레이어가 메뉴 상태인지 여부를 나타내는 값입니다. <br>
이 변수를 true로 설정하면 플레이어가 메뉴 상태가 됩니다.

---

## 신호

### `fullscreen_toggled(to: bool)`
전체 화면 모드가 토글될 때 발생합니다. `to` 매개변수는 전체 화면 모드가 활성화되었는지 여부를 나타냅니다. 활성화되었으면 true를 반환하고, 그렇지 않으면 false를 반환합니다.

### `saved`
게임 데이터가 저장될 때 발생합니다.

---

## 🔗 관련 문서
- [`Overworld`](../userNodes/Overworld.md)
- [`SceneContainer`](SceneContainer.md)
- [`UI`](UI.md)