#pragma once
#include <iostream>
#include <windows.h>
#include "../../Protocol/protocol.h"
#include <vector>
//#include "object.h"

static HDC mem1dc, pdc, hdc;
static HWND hWnd;
static HBITMAP hbit1;
static RECT rectview;
static HINSTANCE g_hinst;
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
	class MAP* mMap;
	class CAMERA* mCamera;
	static class Network* mNetwork;
	static class Network* GetNetwork();
	int prev_size;
	int* mOcount;
	class OBJECT* mObj;
	void ConnectServer(const char* server_ip);
	void C_Send(void* packet, int bytes);
	int C_Recv();
	void ProcessPacket(unsigned char* p);
	unsigned char* buf[MAX_BUF_SIZE];

	bool occur_button = 0;
	//int ocount;
	//class OBJECT obj[150];
	std::vector<std::shared_ptr<class UI>> mUI;

public:
	int net_x, net_y;
	int net_h;
	int net_state;
	int net_stealth;
	int net_dir;
	int net_bx;

	int countdown;
	int init_x = 300;

	void test();
};

