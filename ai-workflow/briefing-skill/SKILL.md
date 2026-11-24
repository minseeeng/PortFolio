---
name: briefing
description: 최근 2일간 수정된 노트를 읽고 현재 상황과 앞으로 할 일을 분석하여 브리핑 노트를 생성하고 Obsidian에서 열어줍니다. Use when the user asks for a briefing, status update, or wants to know what they've been working on and what to do next.
---

# Briefing Skill

최근 2일간 수정된 노트를 분석하여 현재 상황 브리핑 노트를 만들고 Obsidian에서 열어주는 스킬입니다.

## 실행 절차

### 1단계: 최근 수정 파일 목록 가져오기

```bash
obsidian eval code="app.vault.getFiles().filter(f => f.extension === 'md' && (Date.now() - f.stat.mtime) < 2 * 24 * 60 * 60 * 1000).sort((a,b) => b.stat.mtime - a.stat.mtime).map(f => f.path + '|' + new Date(f.stat.mtime).toISOString()).join('\n')"
```

### 2단계: 핵심 파일 선별 및 읽기

가져온 목록에서 **실제 학습/작업 내용**이 담긴 파일을 판단하여 선별합니다:
- 공부일지, 계획표, SMART GOAL, 목표 관련 파일은 반드시 포함
- 같은 타임스탬프가 몰려있는 파일군은 git sync 등 자동 작업일 수 있으므로, 그 중 대표 파일 1~2개만 읽기
- 프로젝트 학습 노트 (코딩테스트, 게임서버 등) 포함

각 선별 파일을 읽을 때:
```bash
obsidian read path="파일경로"
```

### 3단계: 브리핑 분석

읽은 내용을 바탕으로 다음을 파악합니다:
- **현재 진행 중인 학습/프로젝트**: 무엇을 공부하고 있는지
- **최근 완료한 것**: 최근 이틀간 마친 내용
- **진행 중/미완성**: 아직 진행 중인 항목
- **다음 액션**: 노트에서 파악되는 할 일, TODO, 다음 단계
- **주의사항**: 막힌 부분, 고민 중인 것

### 4단계: 브리핑 노트 생성

오늘 날짜로 브리핑 노트를 생성합니다. 노트 이름 형식: `브리핑_YYYY-MM-DD`
저장 경로: `Resource/Road to __/브리핑_YYYY-MM-DD.md`

노트 내용 구조:
```
---
date: YYYY-MM-DD
type: briefing
---

# 브리핑 - YYYY년 MM월 DD일

## 현재 상황 요약
[한 문단으로 현재 상황 요약]

## 최근 2일 활동
[프로젝트/토픽별로 정리]

## 진행 중인 것
[현재 손에 잡고 있는 것들]

## 다음에 할 일
[우선순위 순으로 정리된 액션 아이템]

## 참고 노트
[분석에 사용된 주요 노트 wikilink]
```

노트 생성 명령:
```bash
obsidian create path="Resource/Road to __/브리핑_YYYY-MM-DD.md" content="..." overwrite silent
```

### 5단계: 노트 열기

```bash
obsidian open path="Resource/Road to __/브리핑_YYYY-MM-DD.md" newtab
```

## 주의사항

- 날짜는 항상 오늘 날짜(시스템 날짜)를 사용
- 내용이 없는 빈 노트나 템플릿 파일은 건너뜀
- 파일이 너무 많을 경우 최대 15개까지만 상세 읽기 (나머지는 파일명으로 유추)
- 브리핑은 **한국어**로 작성
- 분석 내용을 먼저 사용자에게 말로 설명한 후, 노트를 생성하고 열기
