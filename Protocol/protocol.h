#pragma once

const unsigned short SERVER_PORT = 4000;
const int MAX_BUF_SIZE = 256;
const int WORLD_HEIGHT = 8;
const int WORLD_WIDTH = 8;
const int  MAX_NAME_SIZE = 20;
const int  MAX_USER = 10;

const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_MOVE = 2;
const char CS_PACKET_TEST = 99;

const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_MOVE = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_REMOVE_OBJECT = 4;

#pragma pack (push, 1)
struct cs_packet_login {
	unsigned char size;
	char	type;
	char	name[MAX_NAME_SIZE];
};

struct cs_packet_move {
	unsigned char size;
	char	type;
	char	key;			// 38 : up,  40: down, 37:left, 39:right
};

struct cs_packet_test {
	unsigned char size;
	char type;
	float zPos;
};

struct sc_packet_login_ok {
	unsigned char size;
	char type;
	int		id;
	short	x, y;
};

struct sc_packet_move {
	unsigned char size;
	char type;
	int id;
	int x, y;
	int state;
	int hp;
	int dir;
	int stealth;
};

struct sc_packet_put_object {
	unsigned char size;
	char type;
	int id;
	short x, y;
	char object_type;
	char	name[MAX_NAME_SIZE];
};

struct sc_packet_remove_object {
	unsigned char size;
	char type;
	int id;
};
#pragma pack(pop)
