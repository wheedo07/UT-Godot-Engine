# Overworld 노드

UT-Godot-Engine에 Overworld를 만들기 위한 노드입니다


## 시그널 (Signals)

| 시그널명                 | 설명                       |
| -------------------- | ------------------------ |
| `initialized()`      | Overworld 노드 초기화 완료 시 발생 |
| `room_initialized()` | RoomEntranceNode으로 Overworld를 들어왔을때 발생하는 신호  |


## 속성 (Properties)

| 속성명              | 타입                                    | 설명                     |
| ---------------- | ------------------------------------- | ---------------------- |
| `world_name`     | `String`                              | 현재 월드의 이름 (주로 세이브 파일이나 표시 용도로 사용) |
| `player_path`    | `NodePath` (`PlayerOverworld`)        | 오버월드 플레이어 노드의 경로 지정 (필수)    |
| `camera_path`    | `NodePath` (`CameraRemoteController`) | 카메라 경로 설정 (필수)              |
| `music`          | `AudioStream`                         | 배경 음악 설정               |
| `room_entrances` | `Array<RoomEntranceNode>`             | 현재 맵에 포함된 입구 노드 지정     |

## 🔧 사용 가능한 메서드

| 함수명                     | 설명                                     |
| ----------------------- | -------------------------------------- |
| `ready()`               | 사용자 정의 초기화 함수 (GDScript에서도 override 가능) |
| `summontextbox()`       | 대화 상자 노드를 화면에 소환                       |

## 💡 사용 예시
<details>
<summary>GDScript</summary>

```gdscript
extends Overworld
@onready var sansTres = preload("res://Game/encounters/tres/sans.tres");

func ready():
	print("ready 실행");

func _on_batle():
	var textbox = summontextbox();
	textbox.generic(Dialogues.new().from([
		"* 전투를 하시겠습니까?", "* 나중에 다시 전투 할수 있습니다"
	]), ["예", "아니요"]);
	textbox.connect("selected_option", func(i:int):
		if i == 0:
			scene_changer.load_battle(sansTres);
	);
	await textbox.dialogue_finished;
	global.set_player_can_move(true);
```

</details>

<details>
<summary>C++</summary>

```C++
#include "test.h"
#include "env.h"

void TEST_WORLD::_bind_methods() {}

void TEST_WORLD::ready() {
}
```

</details>