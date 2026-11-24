#pragma once
#include "SceneType.h"
#include "MouseEvent.h"
#include <vector>
// manager 는 모든 scene을 알고 있다.. 

class CScene;

class CSceneManager
{
public:
	CSceneManager(int max);
	~CSceneManager();

	bool Add(SCENE_TYPE type, CScene* scene);
	bool ChangeScene(SCENE_TYPE type);
	void Clear();
	void Render();
	void Update();
	void OnChar(wchar_t ch);
	void OnMouse(MouseEvent ev);
	void OnPacket(unsigned short type, const char* payload);
	bool ApplyChange();
	bool IsChangeReserved() { return m_next != SCENE_NONE; }

private:
	std::vector<CScene*> m_sceneList;
	CScene* m_current;
	SCENE_TYPE m_next;
};
