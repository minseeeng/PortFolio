#pragma once
#include <vector>
#include "MouseEvent.h"
#include "SceneType.h"
#include "UIObject.h"

class CNetClient;

class CScene
{
public:
	CScene(CNetClient* netClient);
	virtual ~CScene();

	virtual bool Init() { return true; }
	virtual void Render();
	virtual void Update() {};
	virtual void CleanUp();
	virtual void OnChar(wchar_t ch);
	virtual void OnMouse(MouseEvent ev);
	virtual SCENE_TYPE OnPacket(unsigned short type, const char* payload) { return SCENE_NONE; }

protected:
	virtual void OnClick(int id) {}       
	CUIObject* FindUI(int id);

	CNetClient* m_netClient;
	std::vector<CUIObject*> m_uiList;
};

