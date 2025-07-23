# Bullet 노드

UT-Godot-Engine의 전투 시스템에서 사용되는 탄환(Bullet) 노드입니다.  
Undertale 스타일의 탄환 움직임, 데미지, 효과 모드 등을 구현할 수 있습니다.

---

## 시그널 (Signals)

| 시그널명           | 설명                                  |
| --------------- | ----------------------------------- |
| `bullet_fade()` | 탄환이 페이드(사라질 때) 발생                |
| `shake_camera(shake_amt)` | 호출용 시그널 카메라 흔들림 발생시키기 (`shake_amt`: 흔들림 강도) |

---

## 속성 (Properties)

| 속성명             | 타입         | 설명                                   |
| --------------- | ---------- | ------------------------------------ |
| `damage`        | `int`      | 탄환의 데미지 값 (기본값: 5)                  |
| `iframe_grant`  | `int`      | 피격 시 무적 프레임 부여량 (기본값: 50)            |
| `kr_amount`     | `int`      | KR(karma) 데미지량 (기본값: 5)                 |
| `delete_upon_hit` | `bool`    | 피격 시 탄환 삭제 여부                        |
| `sprite_path`   | `NodePath` | 탄환 스프라이트 노드 경로                      |
| `colors`        | `Array<Color>` | 탄환 모드에 따른 색상 배열 (WHITE, GREEN, BLUE, ORANGE) |

---

## 열거형 (Enums)

| 이름            | 값         | 설명                  |
| ------------- | --------- | ------------------- |
| `MovementMode` |           | 탄환 이동 방식             |
|               | `MOVEMENT_VELOCITY` | 속도 기반 이동          |
|               | `MOVEMENT_TWEEN`    | 트윈 기반 이동          |
| `DamageMode`  |           | 탄환 데미지 타입            |
|               | `MODE_WHITE`        | 일반 탄환 (흰색)         |
|               | `MODE_GREEN`        | 회복 탄환 (초록색)        |
|               | `MODE_BLUE`         | 블루 탄환 (움직이면 데미지) |
|               | `MODE_ORANGE`       | 오렌지 탄환 (멈추면 데미지) |

---

## 🔧 사용 가능한 메서드

| 함수명                       | 설명                                   |
| ------------------------ | ------------------------------------ |
| `ready()`               | 사용자 정의 초기화 함수 (GDScript에서도 override 가능) |
| `set_mode(mode: DamageMode)` | 탄환의 데미지 타입 설정 (색상도 변경됨)         |
| `kill()`                  | 탄환 즉시 삭제                            |
| `fade()`                  | 탄환 페이드(서서히 사라짐) 효과 적용             |
| `_on_exit_screen`                 | `VisibleOnScreenNotifier2D` 노드의 `screen_exited` 시그널과 연결용 함수             |

---

## 💡 사용 예시
<details>
<summary>GDScript</summary>

```gdscript
extends Bullet

const TIME: float = 0.8
const BEAM_COLLISION_MARGIN: float = 6

@onready var Beam: Control = $Sprite/Beam
@onready var Rect: NinePatchRect = $Sprite/Beam/NinePatchRect
@onready var AnimPlayer: AnimationPlayer = $AnimationPlayer

func ready() -> void:
	Collision.shape = RectangleShape2D.new()
	Collision.shape.size = Vector2.ZERO


func fire(target: Vector2, size: float = 1, delay: float = 0.5, duration: float = 0.5) -> Blaster:
	$load.play()
	scale = Vector2(max(size, 1), max(size, 1.5))
	target_position = target
	var distance: Vector2 = target_position - global_position
	velocity_tween = create_tween().set_ease(TweenEase).set_trans(TweenTrans).set_parallel()
	velocity_tween.tween_property(self, "position", distance, TIME).as_relative()
	velocity_tween.tween_property(self, "rotation", TAU, TIME).as_relative()
	velocity_tween.chain().tween_interval(delay - TIME)
	velocity_tween.chain().tween_callback(AnimPlayer.play.bind("prepare"))
	velocity_tween.tween_interval(0.15)
	velocity_tween.chain().tween_callback($fire.play)
	velocity_tween.tween_callback(_blast.bind(duration))
	return self


const GROW_TIME: float = 0.2
const SPEED: int = 1000

func _blast(duration: float) -> void:
	Collision.shape.size = Beam.size - Vector2(0, max(BEAM_COLLISION_MARGIN, 0))
	Collision.scale.x = 0
	shakeCamera.emit(0.5)
	Collision.position.y += Beam.size.y / 2.0
	Beam.show()
	AnimPlayer.play("fire")

	var tween_beam := create_tween().set_trans(Tween.TRANS_QUAD).set_loops()
	tween_beam.pause()
	tween_beam.tween_property(Rect, "scale:x", 0.75, GROW_TIME)
	tween_beam.tween_property(Rect, "scale:x", 1, GROW_TIME)

	var tw_remove := create_tween().set_trans(Tween.TRANS_QUAD).set_parallel()
	tw_remove.tween_interval(max(duration, GROW_TIME))
	tw_remove.chain()
	tw_remove.tween_property(Beam, "modulate:a", 0, GROW_TIME).set_trans(Tween.TRANS_LINEAR)
	tw_remove.tween_property(Collision, "scale:x", 0, GROW_TIME)
	tw_remove.tween_callback(Collision.queue_free).set_delay(GROW_TIME / 2.0)
	tw_remove.tween_property(Beam, "scale:x", 0, GROW_TIME)
	tw_remove.chain().tween_callback(queue_free).set_delay(1.8)


	var tw := create_tween().set_trans(Tween.TRANS_QUAD).set_parallel()
	tw.tween_property(Beam, "scale:x", 1, GROW_TIME)
	tw.tween_property(Collision, "scale:x", 1, GROW_TIME)
	tw.tween_property(Beam, "modulate:a", 1, GROW_TIME).set_trans(Tween.TRANS_LINEAR)
	tw.tween_callback(tween_beam.play)

	var tw_move := create_tween().set_trans(Tween.TRANS_SINE)
	tw_move.tween_property(self, "position", Vector2.UP.rotated(rotation) * SPEED, 1.0).as_relative()
```

</details>

<details>
<summary>C++</summary>

```C++
#include "blaster.h"
#include<godot_cpp/variant/utility_functions.hpp>
#include<godot_cpp/classes/scene_tree.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
using namespace godot;

Blaster::Blaster() {
    beam = nullptr;
    rect = nullptr;
    anim_player = nullptr;
    tween_trans = Tween::TRANS_QUAD;
    tween_ease = Tween::EASE_IN_OUT;
}

Blaster::~Blaster() {}

void Blaster::_bind_methods() {
    ClassDB::bind_method(D_METHOD("fire", "target", "size", "delay", "duration"), &Blaster::fire, 
                         DEFVAL(1.0f), DEFVAL(0.5f), DEFVAL(0.5f));
    ClassDB::bind_method(D_METHOD("_blast", "duration"), &Blaster::_blast);
}

void Blaster::ready() {
    beam = Object::cast_to<Control>(get_node_internal("Sprite/Beam"));
    rect = Object::cast_to<NinePatchRect>(get_node_internal("Sprite/Beam/NinePatchRect"));
    anim_player = Object::cast_to<AnimationPlayer>(get_node_internal("AnimationPlayer"));
    collision = Object::cast_to<CollisionShape2D>(get_node_internal("Area2D/CollisionShape2D"));
    
    Ref<RectangleShape2D> shape = memnew(RectangleShape2D);
    shape->set_size(Vector2(0, 0));
    collision->set_shape(shape);
}

Blaster* Blaster::fire(const Vector2& target, float size, float delay, float duration) {
    Object::cast_to<AudioStreamPlayer>(get_node_internal("load"))->play();
    set_scale(Vector2(Math::max(size, 1.0f), Math::max(size, 1.5f)));
    
    target_position = target;
    Vector2 distance;
    if(get_parent()->is_class("Node2D"))
        distance = target_position - get_global_position();
    else distance = target_position - get_position();
    
    velocity_tween = create_tween()->set_ease(tween_ease)->set_trans(tween_trans)->set_parallel(true);
    velocity_tween->tween_property(this, "position", distance, TIME)->as_relative();
    velocity_tween->tween_property(this, "rotation", Math_TAU, TIME)->as_relative();
    velocity_tween->chain()->tween_interval(delay - TIME);
    velocity_tween->chain()->tween_callback(Callable(anim_player, "play").bind("prepare"));
    velocity_tween->tween_interval(0.15);
    velocity_tween->chain()->tween_callback(Callable(Object::cast_to<AudioStreamPlayer>(get_node_internal("fire")), "play"));
    velocity_tween->tween_callback(Callable(this, "_blast").bind(duration));
    
    return this;
}

void Blaster::_blast(float duration) {
    Ref<RectangleShape2D> shape = collision->get_shape();
    
    Vector2 beam_size = beam->get_size();
    shape->set_size(Vector2(beam_size.x, beam_size.y - Math::max(BEAM_COLLISION_MARGIN, 0.0f)));
    collision->set_scale(Vector2(0, 1));
    
    emit_signal("shake_camera", 0.06f);
    
    Vector2 collision_pos = collision->get_position();
    collision_pos.y += beam_size.y / 2.0f;
    collision->set_position(collision_pos);
    beam->show();
    
    anim_player->play("fire");
    
    Ref<Tween> tween_beam = create_tween()->set_trans(Tween::TRANS_QUAD)->set_loops();
    tween_beam->pause();
    tween_beam->tween_property(rect, "scale:x", 0.75f, GROW_TIME);
    tween_beam->tween_property(rect, "scale:x", 1.0f, GROW_TIME);
    
    Ref<Tween> tw_remove = create_tween()->set_trans(Tween::TRANS_QUAD)->set_parallel(true);
    tw_remove->tween_interval(Math::max(duration, GROW_TIME));
    tw_remove->chain();
    tw_remove->tween_property(beam, "modulate:a", 0, GROW_TIME)->set_trans(Tween::TRANS_LINEAR);
    tw_remove->tween_property(collision, "scale:x", 0, GROW_TIME);
    tw_remove->tween_callback(Callable(collision, "set_disabled").bind(true))->set_delay(GROW_TIME / 2.0f);
    tw_remove->tween_property(beam, "scale:x", 0, GROW_TIME);
    tw_remove->chain()->tween_callback(Callable(this, "queue_free"))->set_delay(1.8);

    Ref<Tween> tw = create_tween()->set_trans(Tween::TRANS_QUAD)->set_parallel(true);
    tw->tween_property(beam, "scale:x", 1.0f, GROW_TIME);
    tw->tween_property(collision, "scale:x", 1.0f, GROW_TIME);
    tw->tween_property(beam, "modulate:a", 1, GROW_TIME)->set_trans(Tween::TRANS_LINEAR);
    tw->tween_callback(Callable(tween_beam.ptr(), "play"));
    
    Ref<Tween> tw_move = create_tween()->set_trans(Tween::TRANS_SINE);
    Vector2 up_vector = Vector2(0, -1);
    Vector2 relative_pos = up_vector.rotated(get_rotation()) * SPEED;
    tw_move->tween_property(this, "position", relative_pos, 1.0f)->as_relative();
}
```

</details>

## 주의사항

- 이 노드에서는 `_physics_process` 함수를 선언할수 없습니다