#pragma once
#include <iostream>
#include <windows.h>
#include "../../Protocol/protocol.h"
#include <vector>
//#include "object.h"
#include "Camera.h"
//#include "Map.h"
#define CONN_FAIL -1
extern HDC mem1dc, pdc, hdc;
extern HWND hWnd;
extern RECT rectview;
extern HBITMAP hbit1;
extern HINSTANCE g_hinst;
extern int obj_t;

void error_display(int err_no);

static CAMERA camera;
//static MAP map;

class Network
{
public:
	Network();
	~Network();

	SOCKET s_socket;
	SOCKADDR_IN server_addr;
	WSADATA WSAData;
	class PLAYER* mPlayer;
	class PLAYER* mOthers;
	class MAP* mMap;
	class CAMERA* mCamera;
	static class Network* mNetwork;
	static class Network* GetNetwork();
	int prev_size;
	int* mOcount;
	class OBJECT* mObj;
	int ConnectServer(const char* server_ip);
	void C_Send(void* packet, int bytes);
	int C_Recv();
	void ProcessPacket(unsigned char* p);
	unsigned char buf[MAX_BUF_SIZE];

	bool occur_button = 0;
	//int ocount;
	//class OBJECT obj[150];
	std::vector<std::shared_ptr<class UI>> mUI;

public:
	bool isLogin = true;
	int net_x, net_y;
	int net_h;
	int net_state;
	int net_stealth;
	int net_dir;
	int net_bx;

	int countdown;
	bool cntdown_controller = false;
	int init_x = 340;

	int rank;

	void test();
};

