# Overworld

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

오버월드 씬의 루트 노드로, 플레이어 배치, 카메라 통합, BGM 페이드인을 관리하는 클래스입니다.

---

## 주요 역할
- 오버월드 상태 관리
    - 플레이어와 카메라를 노드 경로를 통해 실제 노드를 찾아 초기화
    - 맵 출입구([`RoomEntranceNode`](RoomEntranceNode.md)) 정보를 사용하여 입구 방향 및 좌표 설정
    - BGM을 페이드인하여 재생
- 오버월드 전투 모드
    - 오버월드에서 탄환 시스템 지원
    - 조우 시스템 토글 기능

---

## 필수 설정
- [`player_path`](#player_path-nodepath) 및 [`camera_path`](#camera_path-nodepath)는 반드시 설정해야 합니다
- [`PlayerOverworld`](../../systemNodes/PlayerOverworld.md) 및 [`CameraController`](CameraController.md) 노드가 존재해야 합니다

---

## 메서드

### `ready()` (virtual)
일반적인 `_ready()`와 달리 오버월드가 로드될 때마다 자동으로 호출됩니다.
오버월드 특화 초기화 로직을 구현하려면 오버라이드하세요.

```gdscript
extends Overworld
@onready var tres := preload("res://encounters/tres/sans.tres")

func ready() -> void:
    print("오버월드 ready() 호출됨")
    var tb := stagehand.summontextbox()
    tb.generic(Dialogues.new().from(["* 싸우고 싶나?", "* 나중에 할 수 있어."]), ["Yes","No"])
    tb.connect("selected_option", func(i:int):
        if i == 0:
            scene_changer.load_battle(tres)
    )
    await tb.dialogue_finished
    global.set_player_can_move(true)
```

### `start_cellphone(id: int)` (virtual)
[UI](../../systemNodes/UI.md)에서 전화를 걸 때 호출됩니다.
[`id`] 매개변수는 [`Global.cells`](../../systemNodes/Global.md#cells) 배열의 인덱스를 나타냅니다.
[`Stagehand.summontextbox()`](../../systemNodes/Stagehand.md#summontextbox) 등으로 텍스트 박스를 표시하는 등의 작업을 할 수 있습니다.

```gdscript
func start_cellphone(id: int):
    match id:
        0:  # 토리엘
            var tb := stagehand.summontextbox()
            tb.generic(Dialogues.new().from(["* 안녕, 얘야."]))
            await tb.dialogue_finished
        1:  # 파피루스
            var tb := stagehand.summontextbox()
            tb.generic(Dialogues.new().from(["* 냐하하! 위대한 파피루스다!"]))
            await tb.dialogue_finished
```

### `player_died()` (virtual)
플레이어가 사망했을 때 호출됩니다.
죽음 메시지를 포함하는 `PackedStringArray`를 반환해야 합니다.

```gdscript
func player_died() -> PackedStringArray:
    return ["여기서 멈출 순 없어...", "결정을 다시 해야 해."]
```

### `start_music_fade_in()`
배경 음악을 페이드인 효과와 함께 재생하기 시작합니다.
현재 재생 중인 음악과 다르거나 전투에서 돌아올 때만 새로운 음악을 재생합니다.
초기화 완료 시 자동으로 호출됩니다.

```gdscript
# 음악 변경 후 페이드인
music = preload("res://music/new_bgm.ogg")
start_music_fade_in()
```

### `toggle_encounter()`
오버월드 전투 모드를 토글합니다.
전투 모드가 비활성화되어 있으면 활성화하고, 활성화되어 있으면 비활성화하며 모든 탄환을 제거합니다.

```gdscript
# 오버월드 전투 시작
toggle_encounter()

# 탄환 생성
add_bullet(preload("res://bullets/fireball.tscn"))

# 전투 종료 (탄환 모두 제거)
toggle_encounter()
```

### `add_bullet(bullet_scene: PackedScene) -> Bullet`
오버월드에 탄환을 추가합니다.
탄환 씬을 인스턴스화하고 자식으로 추가한 후, [`remove_bullets`](#remove_bullets) 시그널에 연결합니다.
탄환 씬의 루트 노드는 [`Bullet`](Bullet.md) 클래스여야 하며 `fade()` 메서드가 있어야 합니다.
오버월드 전투 모드에서 사용됩니다.

```gdscript
# 기본 탄환 추가
var bullet = add_bullet(preload("res://bullets/basic_bullet.tscn"))
if bullet:
    bullet.global_position = Vector2(100, 100)
    bullet.damage = 10
```

### `quick_bullet(bullet_scene: PackedScene, pos: Vector2, rot_deg: float = 0, scale: Vector2 = Vector2(0.5, 0.5)) -> Bullet`
오버월드에 탄환을 추가하고 즉시 위치, 회전, 크기를 설정합니다.
내부적으로 [`add_bullet()`](#add_bulletbullet_scene-packedscene---bullet)을 호출하여 탄환을 생성한 후, 지정된 위치(pos), 회전 각도(rot_deg), 크기(scale)를 설정합니다.
오버월드 전투 모드에서 사용됩니다.

```gdscript
# 위치와 회전이 설정된 탄환 빠르게 생성
var bullet = quick_bullet(
    preload("res://bullets/spinning_bullet.tscn"),
    Vector2(200, 150),  # 위치
    45,                 # 45도 회전
    Vector2(1.0, 1.0)   # 원래 크기
)
```

---

## 멤버 변수

### 에디터에서 설정 가능한 변수

#### `world_name: String`
현재 오버월드의 이름입니다.
세이브 파일 표시나 디버깅 목적으로 사용됩니다.
기본값: `"overworld room"`

#### `player_path: NodePath<PlayerOverworld>`
오버월드 플레이어 노드([`PlayerOverworld`](../../systemNodes/PlayerOverworld.md))의 경로입니다.
이는 필수 설정이며, 비어있으면 초기화가 중단됩니다.

#### `camera_path: NodePath<CameraController>`
카메라 컨트롤러([`CameraController`](../../systemNodes/CameraController.md))의 경로입니다.
이는 필수 설정이며, 비어있으면 초기화가 중단됩니다.

#### `music: AudioStream`
배경 음악 리소스입니다.
설정하지 않으면 기본 오버월드 BGM이 로드됩니다.

#### `room_entrances: Array<RoomEntranceNode>`
이 맵에 배치된 [`RoomEntranceNode`](../../systemNodes/RoomEntranceNode.md)들의 배열입니다.
맵 간 이동 시 플레이어의 시작 위치와 방향을 결정하는 데 사용됩니다.

### 스크립트에서만 사용 가능한 변수

#### `player: PlayerOverworld`
[`player_path`](#player_path-nodepathplayeroverworld)를 통해 찾은 플레이어 노드입니다.
스크립트에서 읽기 전용으로 사용됩니다.

#### `camera: CameraController`
[`camera_path`](#camera_path-nodepathcameracontroller)를 통해 찾은 카메라 컨트롤러 노드입니다.
스크립트에서 읽기 전용으로 사용됩니다.

#### `music_player: AudioStreamPlayer`
배경 음악 플레이어([`Global.get_Music()`](../../systemNodes/Global.md#get_music)와 동일)입니다.
스크립트에서 읽기 전용으로 사용됩니다.

---

## 시그널

### `cached_initialized`
전투 종료 후 오버월드가 다시 로드되거나 [`SceneChanger.load_cached_scene()`](../../systemNodes/SceneChanger.md#load_cached_scene)이 호출될 때 발생합니다.

### `remove_bullets`
모든 탄환을 제거하라는 시그널입니다.
오버월드의 모든 탄환을 제거해야 할 때 발생합니다.
[`toggle_encounter()`](#toggle_encounter)에 의해 전투 모드가 비활성화될 때도 발생합니다.

### `room_initialized`
방이 초기화될 때 발생합니다.
방 초기화에 사용된 데이터가 포함된 딕셔너리를 매개변수로 전달합니다.

---

## 🔗 관련 문서

- [PlayerOverworld - 오버월드 플레이어](../../systemNodes/PlayerOverworld.md)
- [CameraController - 카메라 컨트롤러](CameraController.md)
- [RoomEntranceNode - 방 출입구](RoomEntranceNode.md)
- [Bullet - 기본 탄환](Bullet.md)
- [SceneChanger - 씬 변경기](../../systemNodes/SceneChanger.md)
- [오버월드 만들기](/tutorials/overworld.md)