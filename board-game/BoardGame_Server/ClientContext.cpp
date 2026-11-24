#include "ClientContext.h"
#include <stdio.h>
#include "Server.h"

ClientContext::ClientContext(SOCKET sock, Server* server)
	: m_clientSocket(sock), m_server(server),m_recvSize(0), m_roomId(-1)
{
	m_buffer[0] = '\0';
	m_nickname[0] = '\0';
}

ClientContext::~ClientContext()
{
	closesocket(m_clientSocket);
}


bool ClientContext::Send(unsigned short type, const char* msg)
{
	char packet[BUF_SIZE];
	unsigned short dataSize = (unsigned short)strlen(msg);
	unsigned short size = HEADER_SIZE + dataSize;

	*(unsigned short*)packet = size;
	*(unsigned short*)(packet + 2) = type;
	memcpy(packet + HEADER_SIZE, msg, dataSize);

	if (send(m_clientSocket, packet, size, 0) == SOCKET_ERROR)
	{
		printf("send failed! \n");
		return false;
	}
	return true;
}

bool ClientContext::Send(const char* packet, int size)
{
	if (send(m_clientSocket, packet, size, 0) == SOCKET_ERROR)
	{
		printf("send failed! \n");
		return false;
	}

	return true;
}

bool ClientContext::Recv()
{
	char payload[BUF_SIZE];

	int len = recv(m_clientSocket, m_buffer + m_recvSize, sizeof(m_buffer) - m_recvSize, 0);
	if (len <= 0)
		return false;

	m_recvSize += len;

	while (true)
	{
		if (m_recvSize < HEADER_SIZE)
			break;

		unsigned short size = *(unsigned short*)m_buffer;
		unsigned short type = *(unsigned short*)(m_buffer + 2);

		if (size < HEADER_SIZE || size > BUF_SIZE)
		{
			printf("잘못된 패킷 \n");
			return false;
		}

		if (m_recvSize < size)
			break;

		// gpm : 왜 memcpy 하는지?
		memcpy(payload, m_buffer + HEADER_SIZE, size - HEADER_SIZE);
		payload[size - HEADER_SIZE] = '\0';

		if ((ProcessPacket(type, payload, size)) == false)
		{
			// gpm : 나가면 문제 생길듯
			return false;
		}

		m_recvSize -= size;
		memmove(m_buffer, m_buffer + size, m_recvSize);
	}

	return true;
}

bool ClientContext::ProcessPacket(unsigned short type, const char* payload, int len)
{
	int nickLen = len - HEADER_SIZE;

	if(type==TYPE_NICK)
	{
		if (nickLen <= 0)
			return true;

		if (nickLen >= NICK_SIZE)
		{
			Send(TYPE_NICK_FAIL, "too long");
			return true;
		}

		if (m_nickname[0] != '\0')
			return true;

		memcpy(m_nickname, payload, nickLen);
		m_nickname[nickLen] = '\0';
		printf("%s : 입장 \n", m_nickname);

		Send(type, m_nickname);
		m_server->SendLobbyList(this);
		m_server->SendRoomList(this);
		m_server->BroadCastLobby(TYPE_ENTRY_LOBBY, m_nickname, "", 0);
	}
	else if (type == TYPE_CHAT_SEND)
	{
		if (CHAT_MAX * 2 < nickLen)
			return true;

		if (m_roomId != -1)
			return true;

		if (m_nickname[0] == '\0')
			return true;

		printf("%s : %.*s \n", m_nickname, nickLen, payload);
		m_server->BroadCastLobby(TYPE_CHAT_RECV, m_nickname, payload, nickLen);
	}
	else if (type == TYPE_ROOM_CREATE)
	{
		if (m_nickname[0] == '\0' || m_roomId != -1)
			return true;

		int roomId = m_server->CreateRoom(this);
		if (roomId == -1)
		{
			Send(TYPE_ROOM_ENTER_FAIL, "");
			return true;
		}

		unsigned short id = (unsigned short)roomId;
		Send(TYPE_ROOM_ENTER_OK, "");
		m_server->BroadCastLobby(TYPE_ROOM_ADD, m_nickname, (const char*)&id, sizeof(id));
		m_server->BroadCastLobby(TYPE_EXIT_LOBBY, m_nickname, "", 0);
	}
	return true;
}
