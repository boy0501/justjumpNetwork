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
const char SC_PACKET_MOVE_PROCESS = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_LOGOUT_OBJECT = 4;
const char SC_PACKET_ROBBY = 5;
const char SC_PACKET_EMPTY = 6;

#pragma pack (push, 1)
//---------------------client packet
struct cs_packet_login {
	unsigned char size;
	char	type;
	char	username[MAX_NAME_SIZE];
};

struct cs_packet_move {
	unsigned char size;
	char	type;
	char	dir;			// 0 : up,  1: down, 2:left, 3:right, 4:jump
};

struct cs_packet_test {
	unsigned char size;
	char type;
	float zPos;
};

//-----------------------server packet
struct sc_packet_login_ok {
	unsigned char size;
	char type;
	int		id;
	short	x, y;
};

struct sc_packet_move_process {
	unsigned char size;
	char type;
	int		id;
	short  x, y, h;
	short state;
	short dir;
	short stealth;
	//char hp;
	//char rank;
}; 

struct sc_packet_put_object {
	unsigned char size;
	char type;
	int id;
	short x, y;
	char	username[MAX_NAME_SIZE];
	char state;
	char w, h;
	char hp;
	char dir;
	bool stealth;
};

struct sc_packet_logout_object {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_robby {
	unsigned char size;
	char type;
	char count_start; // 1: start
	int player_cnt;
	//float countdownTime;
};

struct sc_packet_empty {
	unsigned char size;
	char type;
};
#pragma pack(pop)
