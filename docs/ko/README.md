<div align="center">
  <img src="https://raw.githubusercontent.com/wheedo07/UT-Godot-Engine/refs/heads/main/image/UT-Godot-Engine_logo.gif" alt="UT-Godot-Engine Logo" width="400"/>
  <br><br>
  Undertale 팬 게임을 위한 Godot Engine 프레임워크
</div>

## 소개

**UT-Godot-Engine**은 Godot Engine 4.5+를 위한 **Undertale 스타일 팬게임 개발 프레임워크 (GDExtension)**입니다.
전투, 대화, 오버월드 흐름을 빠르게 구성할 수 있도록 필요한 시스템과 도구를 제공합니다.

> 이 엔진은 C++(GDExtension)으로 만들어졌으며, 고성능과 유연성을 모두 제공합니다.

### 이 문서는
- 프레임워크 개요와 핵심 기능을 빠르게 이해하고
- 문서와 튜토리얼로 이어지는 첫 관문 역할을 합니다.

---

## 주요 특징

- **턴 기반 전투 시스템**: Undertale 스타일의 전투 메커니즘을 구현
- **대화 연출 도구**: 텍스트 박스, 타이핑 효과, 애니메이션 지원
- **오버월드 시스템**: 맵 이동, NPC 상호작용, 이벤트 트리거 기능
- **세이브/로드 기능**: 게임 진행 상황을 저장하고 불러오는 기능 제공
- **모듈식 설계**: 각 시스템을 독립적으로 사용 가능
- **플랫폼 지원**: Windows 및 Android 지원

---

## 시작하기
### 📥 프로젝트 열기
1. [최신 릴리스 다운로드](https://github.com/wheedo07/UT-Godot-Engine/releases)
2. 압축 해제 후 폴더 이름 확인
3. Godot Engine 4.5+ 실행
4. `가져오기` → `project.godot` 파일 선택
5. 프로젝트 열기

### 🔧 소스 빌드
```bash
git clone https://github.com/wheedo07/UT-Godot-Engine.git
cd UT-Godot-Engine
scons target=template_release
```

### 노드 목록 및 튜토리얼
- [노드 목록](./classes/list.md)
- [튜토리얼](./tutorials/README.md)