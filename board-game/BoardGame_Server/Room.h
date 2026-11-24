#pragma once
#include "PacketType.h"

class ClientContext;

class CRoom
{
public:
	CRoom();
	~CRoom();

	void Open(ClientContext* owner);
	void Leave(ClientContext* client);
	bool IsUsing() { return m_usingRoom; }
	const char* GetOwnerNick();

private:
	bool m_usingRoom;
	ClientContext* m_player[ROOM_USER_MAX];
	int m_userCount;
};

