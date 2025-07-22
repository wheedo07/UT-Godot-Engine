# RoomEntranceNode

UT-Godot-Engine에서 오버월드 맵 간 이동을 관리하는 노드입니다. 문이나 통로 등 맵 간 전환 지점을 정의할 때 사용됩니다.

## 속성 (Properties)

| 속성명 | 타입 | 설명 |
| --- | --- | --- |
| `facing_direction` | `Vector2` | 플레이어가 새 룸에 입장할 때의 바라보는 방향 |
| `door_id` | `int` | 현재 문의 고유 ID (동일 맵에서는 중복되지 않아야 함) |
| `door_margin` | `int` | 문 주변 마진 (기본값: 16) |
| `new_room` | `String` (파일 경로) | 이동할 새 룸의 씬 파일 경로 (.tscn) |
| `special_room` | `bool` | 특수 룸 여부 (전투, 이벤트 등의 특수 룸일 경우 true) |
| `new_room_entrance_id` | `int` | 이동할 새 룸에서 사용할 입구 ID |

## 메서드 (Methods)

| 메서드명 | 설명 |
| --- | --- |
| `force_enter()` | 플레이어의 위치와 상관없이 강제로 문 진입 이벤트를 발생시킵니다 |

## 💡 사용 예시

### 기본 설정

1. 씬에 `RoomEntranceNode` 추가하기
2. 문/통로 영역 크기를 조절하기
3. 다음 속성 설정:
   - `facing_direction`: 새 맵에서 플레이어가 바라볼 방향 (예: Vector2(0, 1) 아래쪽)
   - `door_id`: 현재 문의 ID 설정
   - `new_room`: 이동할 새 씬 경로 설정 (예: "res://Game/overworld/room2.tscn")
   - `new_room_entrance_id`: 새 룸에서 사용할 입구 ID

### 스크립트 예시

```gdscript
# 특정 이벤트 발생 시 문 강제 진입
func _on_event_trigger():
    var door = $RoomEntranceNode
    door.force_enter()
```

### 룸 연결 예시

```
Room1 (door_id=1, new_room_entrance_id=2) → Room2 (door_id=2, new_room_entrance_id=1)
Room2 (door_id=2, new_room_entrance_id=1) → Room1 (door_id=1, new_room_entrance_id=2)
```

### 주의사항

- 각 룸에서 `door_id`는 고유해야 합니다
- `new_room_entrance_id`는 이동할 룸의 문 `door_id`와 일치해야 합니다
- `special_room`이 true일 경우 
    - door_id 등 문들은 무시하고 그냥 이동 합니다
    - 카메라를 수동으로 페이드 아웃 시켜줘야 합니다
- 플레이어에 위치는 이 노드에 pivot 위치에 등장합니다
- 노드 추가시 Overworld 노드에 [room_entrances](/docs/Overworld.md#속성-properties)에 노드를 추가해야합니다 (그래야지 id가 연결됨)

## 관련 클래스

- [Overworld](/docs/Overworld.md): 룸 입장 및 이벤트를 관리하는 상위 노드