#include "Server.h"
#include <stdio.h>
#include "ClientContext.h"

Server::Server()
	: m_listenSocket(INVALID_SOCKET)
{
	FD_ZERO(&m_reads);
}

Server::~Server()
{
	CleanUp();
}

bool Server::CreateSocket()
{
	m_listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_listenSocket == INVALID_SOCKET)
	{
		printf("failed socket\n");
		return false;
	}

	return true;
}

void Server::SetAddress(const char* ip, unsigned short port)
{
	memset(&m_serverAddr, 0, sizeof(m_serverAddr));
	m_serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &m_serverAddr.sin_addr);
	m_serverAddr.sin_port = htons(port);
}

bool Server::Bind()
{
	if (bind(m_listenSocket, (SOCKADDR*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR)
	{
		printf("bind error \n");
		return false;
	}

	return true;
}

bool Server::Listen()
{
	if (listen(m_listenSocket, 5) == SOCKET_ERROR)
	{
		printf("listen error \n");
		return false;
	}

	FD_SET_EX(m_listenSocket, nullptr, &m_reads);
	printf("listen start ... \n");
	return true;
}

void Server::Run()
{
	while (true)
	{
		fd_set_ex cpyreads = m_reads;

		int eventCount = select(0, (fd_set*)&cpyreads, NULL, NULL, NULL);
		if (eventCount == SOCKET_ERROR)
			break;

		for (int i = 0; i < (int)m_reads.fd_count; i++)
		{
			SOCKET sock = m_reads.fd_array[i];

			if (!FD_ISSET(sock, &cpyreads))
				continue;

			if (sock == m_listenSocket)
			{
				SOCKET clientSocket;
				SOCKADDR_IN clientAddr;

				if ((Accept(clientSocket, clientAddr)) == false)
					continue;

				if ((m_reads.fd_count-1) >= CLIENT_MAX)
				{
					printf("client full \n");
					closesocket(clientSocket);
					continue;
				}

				char ip[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
				printf("접속 : %s:%d \n", ip, ntohs(clientAddr.sin_port));

				ClientContext* clientContext = new ClientContext(clientSocket, this);
				FD_SET_EX(clientSocket, clientContext, &m_reads);
			}
			else
			{
				ClientContext* client = m_reads.fd_client[i];
				if ((client->Recv()) == false)
				{
					FD_CLR_EX(sock, &m_reads);
					printf("퇴장 \n");
					if(client->GetNickName()[0]!='\0')
					{
						if (client->GetRoomId() == -1)         
							BroadCastLobby(TYPE_EXIT_LOBBY, client->GetNickName(), "", 0);
						else
						{
							LeaveRoom(client);
						}
					}
					delete client;
					i--;
				}
			}
		}
	}
}

void Server::CleanUp()
{
	for (int i = 0; i < (int)m_reads.fd_count; i++)
	{
		if (m_reads.fd_client[i] != nullptr)   
			delete m_reads.fd_client[i];      
	}

	m_reads.fd_count = 0;  

	if (m_listenSocket != INVALID_SOCKET)
	{
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
	}
}

void Server::BroadCastLobby(unsigned short type, const char* nickname, const char* msg, int len)
{
	char packet[BUF_SIZE];
	int size = MakeLobbyPacket(packet, type, nickname, msg, len);

	for (int i = 0; i < (int)m_reads.fd_count; i++)
	{
		ClientContext* client = m_reads.fd_client[i];   

		if (client == nullptr)                          
			continue;

		if (client->GetNickName()[0] == '\0')          
			continue;

		if (client->GetRoomId() != -1)
			continue;

		client->Send(packet, size);
	}
}

void Server::SendLobbyList(ClientContext* client)
{
	for (int i = 0; i < (int)m_reads.fd_count; i++)
	{
		ClientContext* others = m_reads.fd_client[i];

		if (others == nullptr)
			continue;

		if (others == client)
			continue;

		if (others->GetNickName()[0] == '\0')
			continue;

		if (others->GetRoomId() != -1)
			continue;

		client->Send(TYPE_ENTRY_LOBBY, others->GetNickName());
	}
}

void Server::SendRoomList(ClientContext* client)
{
	for (int i = 0; i < ROOM_MAX; i++)
	{
		if (m_rooms[i].IsUsing() == false)
			continue;
		else
		{
			unsigned short id = i;
			char packet[BUF_SIZE];
			int size = MakeLobbyPacket(packet, TYPE_ROOM_ADD, m_rooms[i].GetOwnerNick(), (const char*)&id, sizeof(id));
			client->Send(packet, size);
		}

	}
}

int Server::CreateRoom(ClientContext* owner)
{
	for (int i = 0; i < ROOM_MAX; i++)
	{
		if (m_rooms[i].IsUsing() == false)
		{
			m_rooms[i].Open(owner);
			owner->SetRoomId(i);
			return i;
		}
	}

	return -1;
}

void Server::LeaveRoom(ClientContext* client)
{
	int roomId = client->GetRoomId();
	m_rooms[roomId].Leave(client);
	client->SetRoomId(-1);
	if (m_rooms[roomId].IsUsing() == false)
	{
		unsigned short id = (unsigned short)roomId;
		BroadCastLobby(TYPE_ROOM_REMOVE, client->GetNickName(), (const char*)&id, sizeof(id));
	}
}

bool Server::Accept(SOCKET& clientSocket, SOCKADDR_IN& clientAddr)
{
	int sizeClientAddr = sizeof(clientAddr);
	clientSocket = accept(m_listenSocket, (SOCKADDR*)&clientAddr, &sizeClientAddr);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("accept error \n");
		return false;
	}

	return true;
}

int Server::MakeLobbyPacket(char* packet, unsigned short type, const char* nickname, const char* msg, int len)
{
	unsigned short size = HEADER_SIZE + NICK_SIZE + len;

	*(unsigned short*)packet = size;
	*(unsigned short*)(packet + 2) = type;

	memcpy(packet + HEADER_SIZE, nickname, NICK_SIZE);
	memcpy(packet + HEADER_SIZE + NICK_SIZE, msg, len);

	return size;
}
