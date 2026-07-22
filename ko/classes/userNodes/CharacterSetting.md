# CharacterSetting

> 🟢 **사용자 생성 가능**
>> 사용자가 노드를 생성하고 스크립트를 작성할 수 있습니다.

`CharacterSetting`은 [`AudioStreamPlayer`](https://docs.godotengine.org/en/stable/classes/class_audiostreamplayer.html)를 상속하며, 캐릭터별 대화 폰트, 말풍선 머리 아이콘, 타이핑 효과 사운드 등 대화 관련 설정을 관리하는 노드입니다.

---

## 주요 역할
- 캐릭터별 대화 스타일 정의
    - 폰트, 폰트 크기, BBCode 스타일 적용
    - 말풍선 머리 스프라이트 지정
- 타이핑 사운드 관리 (`AudioStreamPlayer` 상속)
    - 글자를 출력할 때마다 재생되는 클릭 사운드 담당
    - 특정 문자에서 소리 미재생 또는 딜레이 추가 가능

---

## 필수 노드 구조

`CharacterSetting` 노드는 반드시 `res://Core/autoload/stagehand.tscn` 파일의 `Character` 노드 아래에 자식으로 추가해야 합니다.
**노드의 이름이 그대로 캐릭터 이름으로 사용됩니다.**

```
Stagehand
└── Character
    ├── DEFAULT          # 기본 캐릭터 (필수)
    ├── SANS             # 커스텀 캐릭터 예시
    └── MY_CHARACTER     # 사용자가 추가한 캐릭터
```

> ⚠️ `DEFAULT` 캐릭터는 반드시 존재해야 합니다. [`TextBox`](../systemNodes/Stagehand.md)의 `generic()` 메서드는 `DEFAULT` 캐릭터의 설정을 사용합니다.

---

## 캐릭터 이름 사용

노드 이름을 문자열로 사용하여 캐릭터를 지정합니다.

**오버월드 NPC 대화 시**
```gdscript
# OverworldCharacterNode 에서 character 속성에 이름 지정
character = "SANS"
```

**전투 말풍선 (DialogueControl) 에서**
```gdscript
# DialogueControl의 character_name 속성에 이름 지정
$Dialogue.character_name = "SANS"
```

---

## 멤버 변수

### 에디터에서 설정 가능한 변수

#### `font: Font`
캐릭터 대화에 사용되는 폰트입니다.
설정하지 않으면 기본 폰트가 사용됩니다.

```gdscript
# .tscn 파일에서 설정 예시
font = preload("res://assets/Fonts/sans.ttf")
```

#### `text_size: float` (기본값: `28`)
캐릭터 대화 폰트 크기입니다.

```gdscript
text_size = 28.0
```

#### `entire_text_bbcode: String` (기본값: `""`)
캐릭터의 전체 대화 텍스트에 일괄 적용되는 BBCode 스타일입니다.
개별 대화줄에 별도 BBCode를 추가할 수도 있으며, 이 설정은 모든 줄에 공통으로 적용됩니다.

```gdscript
entire_text_bbcode = "[b]"         # 전체 텍스트 굵게
entire_text_bbcode = "[i]"         # 전체 텍스트 기울임
entire_text_bbcode = "[wave amp=10 freq=5]"  # 흔들리는 텍스트
```

#### `text_box_head: SpriteFrames`
캐릭터 대화 말풍선 옆에 표시되는 머리 아이콘 스프라이트입니다.
`SpriteFrames`는 **50x50 크기** 프레임으로 구성되어야 하며, 각 프레임이 아이콘의 애니메이션을 나타냅니다.

> 📝 설정하지 않으면 말풍선 머리 없이 표시됩니다. ([`TextBox.character()`](../systemNodes/Stagehand.md)의 `head_hide`가 `true`인 경우와 동일)

#### `extra_delay: String` (기본값: `@#$%^&+=_-~\`<>"|\\*{}()[].,!?`)
타이핑 중 이 문자열에 포함된 글자를 만나면 추가 딜레이(긴 일시 정지)가 적용됩니다.
자연스러운 리듬을 위해 구두점과 특수문자가 기본으로 포함되어 있습니다.

```gdscript
extra_delay = ".,!?"   # 마침표, 쉼표, 느낌표, 물음표에서만 딜레이
```

#### `no_sound: String` (기본값: `@#$%^&+=_-~\`<>"|\\*{}()[].,!? `)
타이핑 중 이 문자열에 포함된 글자에서는 클릭 사운드가 재생되지 않습니다.
공백, 특수문자, 구두점이 기본으로 포함되어 자연스러운 사운드 효과를 제공합니다.

```gdscript
no_sound = " "   # 공백에서만 소리 미재생
```

### AudioStreamPlayer 상속 속성

`CharacterSetting`은 `AudioStreamPlayer`를 상속하므로, `stream`, `bus`, `max_polyphony` 등 `AudioStreamPlayer`의 속성도 사용됩니다.

| 속성 | 용도 |
|------|------|
| `stream` | 타이핑 클릭 사운드 파일 |
| `bus` | 오디오 버스 (`"SFX"` 권장) |
| `max_polyphony` | 동시 재생 가능 수 (기본값: `3` 권장) |

---

## 사용 예시

### stagehand.tscn에서 캐릭터 추가

```
# Godot 에디터에서 스테이지핸드 씬 열기
# res://Core/autoload/stagehand.tscn

Stagehand
└── Character
    ├── DEFAULT
    └── MY_CHAR   ← CharacterSetting 노드 추가 후 이름 지정
```

에디터에서 속성 설정:
```
font          = res://assets/Fonts/my_font.ttf
text_size     = 28
entire_text_bbcode = ""
text_box_head = [MY_CHAR의 SpriteFrames]
stream        = res://sfx/Clicks/my_click.wav
max_polyphony = 3
bus           = "SFX"
```

### 오버월드 NPC에 적용

```gdscript
# OverworldCharacterNode 에서
extends OverworldCharacterNode

func _ready():
    character = "MY_CHAR"  # CharacterSetting 노드 이름과 일치
```

### 전투 말풍선에 적용

```gdscript
# Enemy 에서
extends Enemy

func _on_get_turn():
    $Dialogue.character_name = "MY_CHAR"
    play_dialogue(0)
    await finished_dialogue
    emit_signal("...")
```

---

## ⚠️ 주의사항 & 팁

1. **`DEFAULT` 캐릭터는 필수**
   ```
   # DEFAULT가 없으면 generic 텍스트박스 사용 시 에러가 발생합니다.
   Character
   └── DEFAULT  ← 반드시 존재해야 함
   ```

2. **노드 이름 대소문자 구분**
   ```gdscript
   # ❌ 잘못된 예
   character = "Sans"   # 노드 이름이 "SANS"라면 찾지 못함
   
   # ✅ 올바른 예
   character = "SANS"
   ```

3. **text_box_head 프레임 크기**
   ```
   # SpriteFrames의 각 프레임은 반드시 50x50 픽셀이어야 합니다.
   ```

---

## 🔗 관련 문서

- [Stagehand - 스테이지핸드](../systemNodes/Stagehand.md)
- [Dialogues - 대화 리소스](../resource/Dialogues.md)