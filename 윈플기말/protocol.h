#pragma once

const char SC_PACKET_MOVE_PROCESS = 2;
//const char CS_PACKET_MOVE = 2;


struct sc_packet_move_process {
	unsigned char size;
	char type;
	int id;
	int x, y;
	int state;
	int hp;
	int dir;
	int stealth;
};

//struct cs_packet_move {
//	unsigned char size;
//	char	type;
//	int state;
//};