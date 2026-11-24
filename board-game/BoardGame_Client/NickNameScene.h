#pragma once
#include "Scene.h"
#include "PacketType.h"
#include "StaticImage.h"
#include "InputBox.h"
#include "Button.h"
#include "Sprite.h"
#include "StaticText.h"

class CNetClient;

enum NICK_UI_ID 
{ 
	NICK_UI_INPUT,
	NICK_UI_OK,
	NICK_UI_FAIL
};

class CNickNameScene : public CScene
{
public:
	CNickNameScene(CNetClient* netClient);
	~CNickNameScene();

protected:
	bool Init() override;
	void Render() override;
	void Update() override;
	void OnChar(wchar_t ch) override;
	SCENE_TYPE OnPacket(unsigned short type, const char* payload) override;

private:
	bool SendNickName();
	void OnClick(int id)override;

};
