#include <iostream>
#include <array>

#include "Network.h"
#include "Player.h"
#include "../Protocol/protocol.h"

#pragma comment(lib, "ws2_32")
using namespace std;

array<Player*, 3> CLIENTS;
int Cnt_Player = 0;
LARGE_INTEGER Frequency;
LARGE_INTEGER BeginTime;
LARGE_INTEGER Endtime;
float elapsed_time;
int Fps = 0;

void send_empty_packet(int c_id)
{
	sc_packet_empty packet;
	packet.size = sizeof(sc_packet_empty);
	packet.type = SC_PACKET_EMPTY;
	CLIENTS[c_id]->do_send(&packet, sizeof(packet));
}

//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735 타임관련

DWORD WINAPI GameLogicThread(LPVOID arg)
{
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&Endtime);
	while (1) {

		QueryPerformanceCounter(&BeginTime);
		auto elapsed = BeginTime.QuadPart - Endtime.QuadPart;
		auto deltatime = (double)elapsed / (double)Frequency.QuadPart;
		if (deltatime >= 0.016f)
		{
			Endtime = BeginTime;
			elapsed_time += deltatime;
			Fps++;
			if (elapsed_time > 1.0f)
			{
				cout << "FPS:" << Fps << endl;
				Fps = 0;
				elapsed_time = 0;
			}

			for (int i = 0; i < Cnt_Player; ++i)
			{
				send_empty_packet(i);
			}
		}

		
	}
	return 0;
}

DWORD WINAPI ClientInputThread(LPVOID arg)
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

void send_login_ok(int c_id)
{
	sc_packet_login_ok packet;
	packet.size = sizeof(sc_packet_login_ok);
	packet.type = SC_PACKET_LOGIN_OK;
	packet.id = c_id;
	packet.x = 0;
	packet.y = 0;
	CLIENTS[c_id]->do_send(&packet, sizeof(packet));
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
	HANDLE LogicThread;
	LogicThread = CreateThread(NULL, 0, GameLogicThread, 0, 0, NULL);
	for (int i = 0; i < 3; ++i,++Cnt_Player)
	{
		CLIENTS[i]->c_socket = mNet->AcceptClient(CLIENTS[i]->c_addr);
		CLIENTS[i]->c_id = i;
		send_login_ok(i);
		hThread = CreateThread(NULL, 0, ClientInputThread, (LPVOID)i, 0, NULL);
		if (hThread == NULL) closesocket(CLIENTS[i]->c_socket);
	}
	while (1)
	{
		;
	}

	Network::GetNetwork()->~Network();
	return 0;
}