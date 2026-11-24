#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

class CNetCore
{
public:
	CNetCore();
	~CNetCore();

	bool CreateSocket();
	void SetAddress(const char* ip, unsigned short port);
	bool Connect();
	void CleanUp();

	bool Send(const char* data, int len); 
	int  Recv(char* buffer, int maxLen);
private:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
};

