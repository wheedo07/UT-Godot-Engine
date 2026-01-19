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
현재 활성화된 씬 컨테이너(SceneContainer) 노드를 반환합니다. 씬 컨테이너는 게임의 다양한 씬을 관리하는 역할을 합니다.

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