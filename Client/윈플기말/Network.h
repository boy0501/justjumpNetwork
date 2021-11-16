#pragma once
#include <iostream>
#include <windows.h>
#include "../../Protocol/protocol.h"

static HDC mem1dc, pdc, hdc;
static HWND hWnd;
static HBITMAP hbit1;
static RECT rectview;
static int obj_t = 0;
void error_display(int err_no);

class Network
{
public:
	Network();
	~Network();

	SOCKET s_socket;
	SOCKADDR_IN server_addr;
	WSADATA WSAData;
	class PLAYER* mPlayer;
	static class Network* mNetwork;
	static class Network* GetNetwork();
	int prev_size;
	void ConnectServer(const char* server_ip);
	void C_Send(void* packet, int bytes);
	int C_Recv();
	void ProcessPacket(unsigned char* p);
	unsigned char* buf[MAX_BUF_SIZE];

public:
	int net_x, net_y;
	int net_h;
	int net_state;
	int net_stealth;
	int net_dir;
	int net_bx;
};

