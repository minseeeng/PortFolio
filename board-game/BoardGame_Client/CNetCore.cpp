#include "CNetCore.h"

CNetCore::CNetCore()
	:m_socket(INVALID_SOCKET)
{
}

CNetCore::~CNetCore()
{
}

bool CNetCore::CreateSocket()
{
	m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
		return false;

	return true;
}

void CNetCore::SetAddress(const char* ip, unsigned short port)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &m_addr.sin_addr);
	m_addr.sin_port = htons(port);
}

bool CNetCore::Connect()
{
	if (connect(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR)
		return false;

	return true;
}

void CNetCore::CleanUp()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

bool CNetCore::Send(const char* data, int len)
{
	if (send(m_socket, data, len, 0) == SOCKET_ERROR)
		return false;

	return true;
}

int CNetCore::Recv(char* buffer, int maxLen)
{
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(m_socket, &reads);
	timeval timeout = { 0, 0 };

	int eventCount = select(0, &reads, NULL, NULL, &timeout);
	if (eventCount == SOCKET_ERROR)
		return -1;        

	if (eventCount == 0)
		return 0;        

	int len = recv(m_socket, buffer, maxLen, 0);
	if (len <= 0)
		return -1;          

	return len;
}
