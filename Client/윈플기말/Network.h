#pragma once
#include <iostream>
#include <windows.h>


void error_display(int err_no);

class Network
{
public:
	Network();
	~Network();

	SOCKET s_socket;
	SOCKADDR_IN server_addr;
	WSADATA WSAData;
	static class Network* mNetwork;
	static class Network* GetNetwork();
	void ConnectServer();
};

