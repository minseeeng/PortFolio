#pragma once
#include "Scene.h"
#include "GraphicsCore.h"
#include "PacketType.h"
#include "Button.h"
#include "InputBox.h"
#include <vector>

#define CHAT_LOG_MAX 10                          
#define CHAT_LINE_SIZE (NICK_MAX + 3 + CHAT_MAX + 1)

class CNetClient;

enum LOBBY_UI_ID
{	LOBBY_UI_CHAT,
	LOBBY_UI_OK,
	LOBBY_UI_EXIT,
	LOBBY_UI_ROOM
};

struct RoomInfo
{
	unsigned short id;
	wchar_t owner[NICK_MAX + 1];
};

class CLobbyScene : public CScene
{
public:
	CLobbyScene(CNetClient* netClient);
	~CLobbyScene();

	bool Init() override;
	void Render() override;
	void OnChar(wchar_t ch) override;
	SCENE_TYPE OnPacket(unsigned short type, const char* payload) override;

private:
	void AddChatLog(const wchar_t* line);
	bool SendChat();
	void OnChat(const char* nick, const char* msg); 
	void OnClick(int id) override;

private:
	wchar_t m_chatLog[CHAT_LOG_MAX][CHAT_LINE_SIZE];      
	wchar_t m_userList[CLIENT_MAX][NICK_MAX + 1];
	RoomInfo m_roomList[ROOM_MAX];
	int m_roomCount;
	int m_userCount;
	int m_chatCount;
};

