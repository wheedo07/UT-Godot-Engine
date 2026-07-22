# Intro

> 🔴 **사용자 생성 불가능**
>> 사용자가 직접 생성할 수 없고 씬을 인스턴스화하거나 시스템에서 사용되는 노드입니다.
>> 이 노드는 `res://Core/Startup/intro.tscn` 씬에서 사용됩니다.

---

## 주요 역할
- 게임 시작 시 인트로 시퀀스를 재생합니다.
- JSON 파일에서 인트로 데이터를 로드하여 이미지와 텍스트를 순차적으로 표시합니다.

---

## JSON 데이터 형식

```json
[
    {
        "image_path": "res://Intro/intro_1.png",
        "text": {
            "ko": "오래 옛날, 두 종족이 지구를 다스렸습니다...",
            "en": "Long ago, two races ruled over Earth...",
            "ja": "むかしむかし、ふたつの種族が地球を支配していた..."
        },
        "speed": {
            "ko": 0.05,
            "en": 0.03,
            "ja": 0.04
        },
        "duration": 3.0
    },
    {
        "image_path": "res://Intro/intro_2.png",
        "text": {
            "ko": "인간과 몬스터입니다.",
            "en": "HUMANS and MONSTERS.",
            "ja": "人間とモンスター。"
        },
        "speed": {
            "ko": 0.05,
            "en": 0.03,
            "ja": 0.04
        },
        "duration": 3.0
    }
]
```

| 필드 | 설명 |
|------|------|
| `image_path` | 해당 단계에서 표시할 이미지 경로 |
| `text` | 언어 코드별 대사 텍스트 |
| `speed` | 언어 코드별 타이핑 속도 (초/문자) |
| `duration` | `enable_auto` 사용 시 다음 단계로 넘어가기까지의 대기 시간 (초) |

---

## 메서드

### `ready()` *(virtual)*
상속받은 클래스에서 오버라이드하여 **게임 자체** 초기화 로직을 구현할 수 있는 가상 메서드입니다.

### `is_intro_completed() → bool` *(const)*
인트로가 완료되었는지 여부를 반환합니다.  
모든 인트로 시퀀스가 끝났거나 사용자가 스킵했을 때 `true`를 반환합니다.

### `next()`
인트로 시퀀스의 다음 단계로 진행합니다.  
[`enable_auto`](#enable_auto-bool-기본값-false)가 `false`로 설정된 경우에만 수동으로 호출하여 다음 인트로 단계를 진행할 수 있습니다.

---

## 멤버 변수

### `intro_completed_path: String` (기본값: `""`)
인트로 완료 후 이동할 씬의 파일 경로입니다.  
설정되지 않은 경우 인트로 완료 후 현재 씬에 머물러 있습니다.

### `intro_json_path: String` (기본값: `"res://Intro/intro_data.json"`)
인트로 시퀀스 데이터가 정의된 JSON 파일의 경로입니다.  
이 파일에는 각 인트로 단계의 이미지, 텍스트, 타이밍 정보가 포함되어야 합니다.

### `music: AudioStream`
인트로 중에 재생될 배경 음악입니다.

### `enable_auto: bool` (기본값: `false`)
인트로 시퀀스가 자동으로 진행될지 여부를 설정합니다.  
`true`로 설정하면 각 인트로 단계가 지정된 `duration` 후에 자동으로 넘어갑니다.  
`false`로 설정하면 [`next()`](#next)를 호출하여 수동으로 다음 단계로 넘어가야 합니다.

---

## 신호

### `intro_completed(skipped: bool)`
인트로 시퀀스가 완료되었을 때 발생합니다.  
`skipped`는 사용자가 인트로를 스킵했는지 여부를 나타냅니다.

### `started_intro(index: int)`
인트로 시퀀스의 각 단계가 시작될 때 발생합니다.  
`index`는 현재 인트로 단계의 인덱스입니다.

---

## 🔗 관련 문서
- [씬 교체 시스템](scene_changer.md)
- [프로젝트 구조](/tutorials/project-structure.md)