#pragma once

const unsigned short SERVER_PORT = 4000;
const int MAX_BUF_SIZE = 256;
const int WORLD_HEIGHT = 8;
const int WORLD_WIDTH = 8;
const int  MAX_NAME_SIZE = 20;
const int  MAX_USER = 10;
const int GEARCYCLE = 180;
const int GEARSPEED = 150;
const float CHARROWSPD = 150;
const float CHARCOLSPD = 0.05;
const float PI = 3.14159265;

const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_MOVE = 2;
const char CS_PACKET_KEYUP = 5;
const char CS_PACKET_DIEOK = 6;


const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_MOVE_PROCESS = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_LOGOUT_OBJECT = 4;
const char SC_PACKET_LOBBY = 5;
const char SC_PACKET_GAMESTART = 7;
const char SC_PACKET_PORTAL = 9;
const char SC_PACKET_OBJECT_SYNC = 10;

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
	char	vk_key;			
};

struct cs_packet_keyup {		
	unsigned char size;
	char type;
	char vk_key;
};

struct cs_packet_die_ok {		
	unsigned char size;
	char type;

};

//-----------------------server packet
struct sc_packet_login_ok {		
	unsigned char size;
	char type;
	int		id;
	short	x, y;
	char stage;
};

struct sc_packet_move_process {		
	unsigned char size;
	char type;
	int		id;
	int  x, y, h;
	short state;
	short dir;
	short stealth;
	short hp;
	char rank;
	float vx, vy;
}; 

struct sc_packet_put_object {		
	unsigned char size;
	char type;
	int id;
	short x, y;
	char	username[MAX_NAME_SIZE];
	char state;
	char w, h;
	short hp;
	char dir;
	bool stealth;
};

struct sc_packet_logout_object {	
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_lobby {			
	unsigned char size;
	char type;
	//int player_cnt;
	int countdown;
};

struct sc_packet_gamestart {		
	unsigned char size;
	char type;
	char stage;
	short  x, y, h;
	short state;
	short dir;
	short stealth;
	bool COMMAND_die;
};

struct sc_packet_portal {		
	unsigned char size;
	char type;
	char stagenum;
};

struct sc_packet_object_sync {		
	unsigned char size;
	char type;
	char objnum;
	short mx, my;
	char index;
	float degree,vd;
};
#pragma pack(pop)
