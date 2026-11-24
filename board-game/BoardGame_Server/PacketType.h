#pragma once

// 패킷
#define HEADER_SIZE 4
#define BUF_SIZE 255
#define SERVER_PORT 30002

// 닉네임씬
#define TYPE_NICK 1
#define TYPE_NICK_FAIL 2
#define NICK_MAX 10
#define NICK_SIZE (NICK_MAX * 2 + 1)

// 로비씬
#define TYPE_CHAT_SEND 3
#define TYPE_CHAT_RECV 4
#define TYPE_ENTRY_LOBBY 5
#define TYPE_EXIT_LOBBY 6
#define CHAT_MAX 100
#define CLIENT_MAX 63

// 룸씬
#define TYPE_ROOM_CREATE 11
#define TYPE_ROOM_ADD 12
#define TYPE_ROOM_REMOVE 13
#define TYPE_ROOM_ENTER 14
#define TYPE_ROOM_ENTER_OK 15
#define TYPE_ROOM_ENTER_FAIL 16
#define ROOM_USER_MAX 2
#define ROOM_MAX 10