#include <iostream>
#include <array>

#include "Network.h"
#include "Player.h"
#include "../Protocol/protocol.h"

#pragma comment(lib, "ws2_32")
using namespace std;


int Cnt_Player = 0;
array<Player*, 3> CLIENTS;

void send_move_packet(int c_id, int mover)
{
	sc_packet_move packet;
	packet.id = mover;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_MOVE;
	packet.x = CLIENTS[mover]->x;
	packet.y = CLIENTS[mover]->y;
	packet.state = CLIENTS[mover]->state;
	packet.dir = CLIENTS[mover]->dir;
	packet.hp = CLIENTS[mover]->hp;
	packet.stealth = CLIENTS[mover]->stealth;

	CLIENTS[c_id]->do_send(sizeof(packet), &packet);

}

DWORD WINAPI func(LPVOID arg) //클라이언트 스레드 함수
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
		//TODO
		//CLIENTS[c_id]->ProcessPacket();
		CLIENTS[c_id]->move();
		for (auto& cl : CLIENTS)
		{
			send_move_packet(cl->c_id, c_id);
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

		//player 초기화
		CLIENTS[i]->initPos();
	}
	
	while (1)
	{
		;
	}

	Network::GetNetwork()->~Network();
	return 0;
}