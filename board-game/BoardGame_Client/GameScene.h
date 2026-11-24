#pragma once
#include "Scene.h"

class CNetClient;

class CGameScene : public CScene
{
public:
	CGameScene(CNetClient* netClient);
	~CGameScene();

	bool Init() override;
	void Render() override;

	SCENE_TYPE OnPacket(unsigned short type, const char* payload) override;
};

