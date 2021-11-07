#pragma once
#include <iostream>
#include <windows.h>
#include "../../Protocol/protocol.h"


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
	void ConnectServer();
	void C_Send(void* packet, int bytes);
	int C_Recv();
	void ProcessPacket(unsigned char* p);
	unsigned char* buf[MAX_BUF_SIZE];
};

