#include <iostream>
#include <array>

#include "Network.h"
#include "Player.h"
#include "../Protocol/protocol.h"

#pragma comment(lib, "ws2_32")
using namespace std;

array<Player*, 3> CLIENTS;
int Cnt_Player = 0;

DWORD WINAPI func(LPVOID arg)
{
	int c_id = (int)arg;
	while (1)
	{
		int ret = CLIENTS[c_id]->do_recv();
		if (ret == SOCKET_ERROR)
		{
			int err_no = WSAGetLastError();
			error_display(err_no);
			closesocket(CLIENTS[c_id]->c_socket);
			return 0;
		}
	}
}

int main()
{
	wcout.imbue(locale("korean"));
	for (int i = 0; i < 3; ++i)
	{
		CLIENTS[i] = new Player();
	}
	auto mNet = Network::GetNetwork();
	mNet->InitServer();
	HANDLE hThread;
	for (int i = 0; i < 3; ++i,++Cnt_Player)
	{
		CLIENTS[i]->c_socket = mNet->AcceptClient(CLIENTS[i]->c_addr);
		hThread = CreateThread(NULL, 0, func, (LPVOID)i, 0, NULL);
		if (hThread == NULL) closesocket(CLIENTS[i]->c_socket);
	}
	while (1)
	{
		;
	}

	Network::GetNetwork()->~Network();
	return 0;
}