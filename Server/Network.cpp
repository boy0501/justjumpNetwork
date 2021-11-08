#include <iostream>
#include "Network.h"
#include "CLIENT/Client.h"
#include "../Protocol/protocol.h"

Network* Network::mNetwork = nullptr;

void error_display(int err_no)
{
	wchar_t* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf << std::endl;
	//while (true);
	LocalFree(lpMsgBuf);
}

Network::Network()
{
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

Network::~Network()
{
	closesocket(s_socket);
	WSACleanup();
	delete mNetwork;
}

Network* Network::GetNetwork()
{
	if (mNetwork == nullptr)
		mNetwork = new Network();
	return mNetwork;
}

void Network::InitServer()
{
	bind(s_socket, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr));
	listen(s_socket, SOMAXCONN);
}

SOCKET Network::AcceptClient(SOCKADDR_IN& out_addr)
{
	SOCKADDR_IN c_addr;
	ZeroMemory(&c_addr, sizeof(c_addr));
	int addr_size = sizeof(c_addr);
	SOCKET s = accept(s_socket, (SOCKADDR*)&c_addr, &addr_size);
	memcpy(&out_addr, &c_addr, sizeof(c_addr));
	return s;
}