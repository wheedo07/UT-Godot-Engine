<div align="center">
  <img src="https://raw.githubusercontent.com/wheedo07/UT-Godot-Engine/refs/heads/main/image/UT-Godot-Engine_logo.gif" alt="UT-Godot-Engine Logo" width="400"/>
  <br><br>
  Undertale 스타일 게임을 위한 Godot Engine 프레임워크
</div>

이 프로젝트는 Godot Engine 4.5+를 위한 **Undertale 프레임워크 (GDExtension)**입니다.
팬게임 개발에 필요한 다양한 시스템과 도구를 제공하여, 개발자가 보다 쉽게 Undertale 스타일의 게임을 만들 수 있도록 지원합니다.

---

## ✨ 주요 특징

- **턴 기반 전투 시스템**: Undertale 스타일의 전투 메커니즘을 구현
- **대화 연출 도구**: 텍스트 박스, 타이핑 효과, 애니메이션 지원
- **오버월드 시스템**: 맵 이동, NPC 상호작용, 이벤트 트리거 기능
- **세이브/로드 기능**: 게임 진행 상황을 저장하고 불러오는 기능 제공
- **모듈식 설계**: 각 시스템을 독립적으로 사용 가능
- **사용자 친화적 API**: 직관적인 노드 기반 개발 환경
- **플랫폼 지원**: Windows 및 Android 지원

---

## 🚀 시작하기
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

---

## 📚 문서
- [프로젝트 구조](tutorials/project-structure.md) - 프레임워크의 디렉토리 및 파일 구조 설명
- [노드 및 리소스 목록](classes/list.md) - 제공되는 모든 노드와 리소스