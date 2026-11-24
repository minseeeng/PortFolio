#pragma once
#include "PacketType.h"

class CNetCore;

enum PACKET_RESULT
{
	PACKET_ERROR,  
	PACKET_NONE,    
	PACKET_OK,      
};

class CNetClient
{
public:
	CNetClient(CNetCore* cNetCore);
	~CNetClient();

	bool Recv();
	PACKET_RESULT GetPacket(unsigned short& type, char* buffer);
	bool Send(unsigned short type, const char* payload, unsigned short payloadSize);
	
private:
	char m_buffer[BUF_SIZE];
	int m_recvSize;
	CNetCore* m_netCore;
};

