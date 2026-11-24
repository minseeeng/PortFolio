#include "GameScene.h"
#include "WindowSize.h"
#include "GraphicsCore.h"
#include "CNetClient.h"

CGameScene::CGameScene(CNetClient* netClient)
	:CScene(netClient)
{
}

CGameScene::~CGameScene()
{
}

bool CGameScene::Init()
{
	return true;
}

void CGameScene::Render()
{
	GraphicsCore::GetInstance().Clear(D2D1::ColorF(D2D1::ColorF::LightGray));

	D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
	GraphicsCore::GetInstance().DrawText(L"게임 씬", (int)wcslen(L"게임 씬"), rect, D2D1::ColorF(D2D1::ColorF::Black));
}


SCENE_TYPE CGameScene::OnPacket(unsigned short type, const char* payload)
{
	return SCENE_NONE;
}
