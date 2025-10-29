# UT-Godot-Engine

<div align="center">
  <img src="./image/UT-Godot-Engine_logo.gif" alt="UT-Godot-Engine Logo" width="500"/>
</div>

<br>

## 📝 개요

**UT-Godot-Engine**은 **Godot Engine**과 **C++ (GDExtension)**을 기반으로 개발된 **Undertale 스타일의 팬게임 개발 프레임워크**입니다.
Godot의 장점과 C++의 장점을 결합하여 **턴 기반 전투**, **대화 연출**, **오버월드 시스템** 등을 유연하게 구현할 수 있도록 설계되었습니다.

<br>

## ✨ 주요 특징

* **Undertale 스타일** 턴 기반 전투 시스템
* **고급 텍스트 연출** - 대화 상자, 타이핑 효과, 텍스트 애니메이션
* **오버월드 시스템** - 맵 이동, NPC 상호작용, 이벤트 트리거
* **완전한 세이브/로드** 기능
* **모듈식 설계** - 각 시스템을 독립적으로 사용 가능
* **사용자 친화적 API** - 직관적인 노드 기반 개발

<br>

## 🎮 플랫폼 지원

| 플랫폼 | 지원 상태 |
|--------|-----------|
| 🖥️ **Windows** | ✅ 완전 지원 |
| 📱 **Android** | ✅ 지원 |

<br>

## 📦 설치 및 설정

### 🔧 빠른 시작


#### 📥 방법 1: 릴리스 버전 다운로드 (권장)
1. **프로젝트 다운로드**
   - [최신 릴리스 다운로드](https://github.com/wheedo07/UT-Godot-Engine/releases)
   - 압축 해제 후 폴더 이름 확인

2. **Godot으로 프로젝트 열기**
   - Godot Engine 4.5+ 실행
   - `가져오기` → `project.godot` 파일 선택
   - 프로젝트 열기

3. **즉시 사용 가능!**
   - 모든 노드와 시스템이 사전 구성됨
   - 예제 씬들을 통해 바로 시작


#### 🛠️ 방법 2: 소스에서 빌드
```bash
git clone https://github.com/wheedo07/UT-Godot-Engine.git
cd UT-Godot-Engine
scons target=template_release
```

### 💻 개발 언어 지원

이 엔진은 **두 가지 개발 방식**을 모두 지원합니다:

| 언어 | 특징 | 사용 시기 |
|------|------|----------|
| 🐍 **GDScript** | • 빠른 프로토타이핑<br>• 쉬운 학습<br>• Godot 네이티브 | • 게임 로직 구현<br>• 빠른 개발<br> |
| ⚡ **C++ (GDExtension)** | • 고성능<br>• 복잡한 로직<br>• 네이티브 라이브러리 | • 성능 최적화<br>• 복잡한 알고리즘<br> |

> 💡 **추천**: GDScript으로 만들고 필요에 따라 C++로 최적화하는 방식을 권장합니다.

<br>

## 📖 사용 방법

### ✅ 사용 규칙

#### 🎯 노드 사용 규칙
- ✅ **에디터에서 노드 설명에 "사용자 생성 가능"**이라고 표시된 노드만 직접 생성
- ❌ **그 외의 커스텀 노드**는 엔진 내부 로직용으로 직접 생성 금지
- 📚 **각 노드의 상세한 사용법**은 에디터 내 노드 문서 참조

#### 🌐 다국어 지원
이 엔진은 **국제화(i18n)** 시스템을 내장하고 있어 여러 언어를 지원합니다:

```gdscript
# 한국어로 변경
TranslationServer.set_locale("ko")

# Change to English
TranslationServer.set_locale("en")
```

> 💡 **지원 언어**: 한국어(ko), English(en)

#### 📁 폴더 색상 가이드
- 🔴 **빨간색 폴더**: 시스템/엔진 전용 파일
  - **절대 수정/삭제 금지**
  - 엔진 핵심 로직이 포함됨
  
- 🟡 **노란색 폴더**: 진입점/루트 관련
  - **기본 파일들은 삭제 금지**
  - 새 파일 추가는 가능하나 구조 이해 필요
  - 기존 노드의 매개변수만 수정 권장
  
- 🟢 **초록색 폴더**: 사용자 컨텐츠 제작용
  - **자유롭게 수정 가능**
  - 게임 컨텐츠, 리소스, 스크립트 등

#### 🛠 개발 규칙
  - 개발 및 디버깅 시 임베딩을 끈 상태로 실행하세요. 해상도/전체화면 설정이 적용되지 않을 수 있습니다.
  - `res://Main/main_node.tscn`는 프로젝트의 진입점입니다. 이 씬은 절대 삭제하거나 이름을 변경하지 마세요.
  - `res://Main` 폴더 내에서 건들수 있는 파일은 `stagehand.tscn`, `overworld_scene_changer.tscn`, `text_box_head.tres` 뿐입니다.
  - `res://Main/stagehand.tscn`에서 건들면 안돼는 노드는 % 접두사를 붙여놨습니다.

<br>

## 📁 생성 가능한 노드 목록

### 🎯 전투 시스템
- **`AttackBase`** - 공격 패턴 베이스 클래스
- **`Enemy`** - 적 캐릭터 노드
- **`Bullet`** - 기본 탄환 노드
- **`DefaultBullet`** - 기본 발사체 노드
- **`BulletArea`** - 탄환 충돌 감지 영역

### 📝 텍스트 시스템
- **`GenericTextTyper`** - 기본 타이핑 텍스트
- **`AdvancedTextTyper`** - 고급 대화 연출

### 🌍 오버월드 시스템
- **`Overworld`** - 오버월드 맵 베이스
- **`RoomEntranceNode`** - 맵 간 이동 노드

### 기타 시스템
- **`CameraController`** - 프레임워크용 카메라
- **`ShopCreate`** - 상점 제작 노드
- **`OptionSelectableSolo`** - 옵션 선택 제작
- **`CharacterSetting`** - 캐릭터 설정 노드 (`res://Main/stagehand.tscn`에서 사용)

> 💡 **팁**: 각 노드의 상세한 사용법과 예제 코드는 Godot 에디터의 "도움말" → "클래스 참조"에서 확인할 수 있습니다.

<br>

### 📖 공식 문서
- **노드 API 문서**: Godot 에디터 내 "도움말" 탭 (F1)에서 확인

<br>

## 🛠 기술 스택

- **Godot Engine 4.5**
- **GDExtension (C++17)**

<br>

## 🙏 Credits

### 🤝 기여자 (Contributors)

#### [@wheedo07](https://github.com/wheedo07)
- 프로젝트 관리 및 개발

#### [@chqh](https://github.com/chqh0311)
- 엔진 로고 제작
- 엔진 피드백 및 테스트

### 🎵 특별 감사
- **Toby Fox** - Original Undertale 게임 제작자

---

<br>

## 📄 라이선스

이 프로젝트는 [MIT License](LICENSE) 하에 배포됩니다.