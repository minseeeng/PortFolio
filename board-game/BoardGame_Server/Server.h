#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

#include "Room.h"
#include "PacketType.h"

class ClientContext;


typedef struct fd_set_ex {
    u_int fd_count;              
    SOCKET  fd_array[FD_SETSIZE]; 
    ClientContext* fd_client[FD_SETSIZE];
} fd_set_ex;

#define FD_CLR_EX(fd, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set FAR *)(set))->fd_count ; __i++) { \
        if (((fd_set_ex FAR *)(set))->fd_array[__i] == fd) { \
            while (__i < ((fd_set_ex FAR *)(set))->fd_count-1) { \
                ((fd_set_ex FAR *)(set))->fd_array[__i] = \
                    ((fd_set_ex FAR *)(set))->fd_array[__i+1]; \
                ((fd_set_ex FAR *)(set))->fd_client[__i] = \
                    ((fd_set_ex FAR *)(set))->fd_client[__i+1]; \
                __i++; \
            } \
            ((fd_set_ex FAR *)(set))->fd_count--; \
            break; \
        } \
    } \
} while(0)

#define FD_SET_EX(fd, client, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set FAR *)(set))->fd_count; __i++) { \
        if (((fd_set_ex FAR *)(set))->fd_array[__i] == (fd)) { \
            break; \
        } \
    } \
    if (__i == ((fd_set_ex FAR *)(set))->fd_count) { \
        if (((fd_set_ex FAR *)(set))->fd_count < FD_SETSIZE) { \
            ((fd_set_ex FAR *)(set))->fd_array[__i] = (fd); \
            ((fd_set_ex FAR *)(set))->fd_client[__i] = (client); \
            ((fd_set_ex FAR *)(set))->fd_count++; \
        } \
    } \
} while(0)

class Server
{
public:
	Server();
	~Server();

	bool CreateSocket();
	void SetAddress(const char* ip, unsigned short port);
	bool Bind();
	bool Listen();
	void Run();
	void CleanUp();
    void BroadCastLobby(unsigned short type, const char* nickname, const char* msg, int len);
    void SendLobbyList(ClientContext* client);
    void SendRoomList(ClientContext* client);
    int CreateRoom(ClientContext* owner);
    void LeaveRoom(ClientContext* client);

private:
	bool Accept(SOCKET& clientSocket, SOCKADDR_IN& clientAddr);
    int MakeLobbyPacket(char* packet, unsigned short type, const char* nickname, const char* msg, int len);

private:
	SOCKET m_listenSocket;
	SOCKADDR_IN m_serverAddr;
	fd_set_ex m_reads;
    CRoom m_rooms[ROOM_MAX];
};
