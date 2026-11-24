#include "CNetClient.h"
#include "CNetCore.h"
#include <string.h>

CNetClient::CNetClient(CNetCore* netCore)
	:m_recvSize(0), m_netCore(netCore)
{
}

CNetClient::~CNetClient()
{
}


bool CNetClient::Recv()
{
	int size = BUF_SIZE - m_recvSize;                        
	if (size <= 0)
		return true;                                        

	int len = m_netCore->Recv(m_buffer + m_recvSize, size);
	if (len < 0)
		return false;                                       

	m_recvSize += len;                                    
	return true;
}

PACKET_RESULT CNetClient::GetPacket(unsigned short& type, char* buffer)
{
	if (m_recvSize < HEADER_SIZE)
		return PACKET_NONE;

	unsigned short size = *(unsigned short*)m_buffer;

	if (size < HEADER_SIZE || size > BUF_SIZE)
	{
		return PACKET_ERROR;
	}

	if (m_recvSize < size)
		return PACKET_NONE;

	type = *(unsigned short*)(m_buffer + 2);
	memcpy(buffer, m_buffer + HEADER_SIZE, size - HEADER_SIZE);
	buffer[size - HEADER_SIZE] = '\0';

	m_recvSize -= size;
	memmove(m_buffer, m_buffer + size, m_recvSize);

	return PACKET_OK;
}

bool CNetClient::Send(unsigned short type, const char* payload, unsigned short payloadSize)
{
	unsigned short size = HEADER_SIZE + payloadSize;
	if (size > BUF_SIZE)
		return false;

	char packet[BUF_SIZE];
	*(unsigned short*)packet = size;
	*(unsigned short*)(packet + 2) = type;
	memcpy(packet + HEADER_SIZE, payload, payloadSize);

	return m_netCore->Send(packet, size);
}
