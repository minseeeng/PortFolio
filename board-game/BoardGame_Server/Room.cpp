#include "Room.h"
#include "ClientContext.h"

CRoom::CRoom()
	:m_usingRoom(false), m_userCount(0)
{
	for (int i = 0; i < ROOM_USER_MAX; i++)
		m_player[i] = nullptr;
}

CRoom::~CRoom()
{
}

void CRoom::Open(ClientContext* owner)
{ 
	m_usingRoom = true;
	m_player[0] = owner;
	m_userCount = 1;
}

void CRoom::Leave(ClientContext* client)
{
	for (int i = 0; i < m_userCount; i++)
	{
		if (m_player[i] == client)
		{
			for (int j = i; j < m_userCount - 1; j++)
				m_player[j] = m_player[j + 1];
			m_player[m_userCount - 1] = nullptr;
			m_userCount--;
			break;
		}
	}

	if (m_userCount == 0)
		m_usingRoom = false;
}

const char* CRoom::GetOwnerNick()
{
	return m_player[0]->GetNickName();
}
