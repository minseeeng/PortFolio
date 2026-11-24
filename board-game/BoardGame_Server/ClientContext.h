#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "PacketType.h"

#pragma comment(lib,"ws2_32.lib")

class Server;

//클라이언트 하나와 통신한다
class ClientContext
{
public:
	ClientContext(SOCKET sock, Server* server);
	~ClientContext();

	bool Send(unsigned short type, const char* msg);
	bool Send(const char* packet, int size); 
	bool Recv();
	bool ProcessPacket(unsigned short type, const char* payload, int len);
	SOCKET GetSocket() { return m_clientSocket; }
	const char* GetNickName() { return m_nickname; }
	int GetRoomId() { return m_roomId; }
	void SetRoomId(int roomId) { m_roomId = roomId; }

private:
	SOCKET m_clientSocket;
	Server* m_server;

	char m_buffer[BUF_SIZE];
	char m_nickname[NICK_SIZE + 1];
	int m_recvSize;
	int m_roomId;
};
