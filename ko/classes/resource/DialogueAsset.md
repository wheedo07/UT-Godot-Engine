# DialogueAsset

`DialogueAsset` 리소스는 에디터에서 대화 내용을 정의할때 사용됩니다. 이 리소스는 [`DialogueLocalization`](DialogueLocalization.md) 리소스 또는 [`Dialogues`](Dialogues.md) 리소스를 선택해 대화 내용을 불러올 수 있습니다.

---

## 속성

### `is_locale`
- 타입: `bool`
- 기본값: `false`
- 설명: 대화 내용을 로드할 때 [`DialogueLocalization`](DialogueLocalization.md) 리소스를 사용할지 여부를 지정합니다. `false`로 설정하면 [`Dialogues`](Dialogues.md) 리소스를 사용합니다.

### `dialogues`
- 타입: `Array[Dialogues]`
- 설명: [`is_locale`](#is_locale) 속성이 `false`로 설정된 경우 사용할 대화 리소스의 배열입니다. 여기서 일반적인 대화 내용을 정의할 수 있습니다.

### `locale_path`
- 타입: `String`
- 설명: [`is_locale`](#is_locale) 속성이 `true`로 설정된 경우 사용할 JSON 파일의 경로입니다. 이 파일에 설정은 [`DialogueLocalization`](DialogueLocalization.md) 리소스의 형식을 따라야 합니다.

### `locale_keys`
- 타입: `PackedStringArray`
- 설명: [`is_locale`](#is_locale) 속성이 `true`로 설정된 경우 사용할 로케일 키의 배열입니다. 인덱스에 따라 JSON 파일에서 대화 내용을 불러옵니다.

---

## 🔗 관련 문서
- [DialogueLocalization](DialogueLocalization.md)
- [Dialogues](Dialogues.md)