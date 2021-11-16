#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>

#include "Network.h"
#include "CLIENT/Client.h"
#include "CLIENT/LoginClient.h"
#include "CLIENT/LobbyClient.h"
#include "CLIENT/GameClient.h"
#include "Object.h"
#include "Map.h"
#include "../Protocol/protocol.h"
#include <time.h>
#pragma comment(lib, "ws2_32")
using namespace std;

Map* mainMap;
int Cnt_Player = 0;
LARGE_INTEGER Frequency;
LARGE_INTEGER BeginTime;
LARGE_INTEGER Endtime;
float elapsed_time;
float change_time;
bool do_once_change = true;
int Fps = 0;
std::array<class Client*, 3> CLIENTS;


void ChangeLoginToRobby(const int& c_id)
{
	int my_id = c_id;
	auto p = reinterpret_cast<LoginClient*>(CLIENTS[my_id]);
	LobbyClient* willbe_changed = new LobbyClient();
	auto Upcasting_changed = reinterpret_cast<Client*>(willbe_changed);
	auto Upcasted_original = reinterpret_cast<Client*>(p);
	*Upcasting_changed = *Upcasted_original;
	willbe_changed->elapsedtime = 0;
	willbe_changed->mStageNum = 0;
	willbe_changed->initBitPos();
	willbe_changed->initPos();
	CLIENTS[my_id] = willbe_changed;
	delete p;

	//send_ok_packet
	sc_packet_login_ok packet;
	packet.size = sizeof(sc_packet_login_ok);
	packet.type = SC_PACKET_LOGIN_OK;
	packet.id = my_id;
	packet.x = CLIENTS[my_id]->x;
	packet.y = CLIENTS[my_id]->y;
	packet.stage = 0;
	CLIENTS[my_id]->do_send(&packet, sizeof(packet));
}
void ChangeRobbyToGame(const int& c_id)
{
	int my_id = c_id;
	auto p = reinterpret_cast<LobbyClient*>(CLIENTS[my_id]);
	GameClient* willbe_changed = new GameClient();
	auto Upcasting_changed = reinterpret_cast<Client*>(willbe_changed);
	auto Upcasted_original = reinterpret_cast<Client*>(p);
	*Upcasting_changed = *Upcasted_original;
	willbe_changed->elapsedtime = 0;
	willbe_changed->mStageNum = 1;
	willbe_changed->mMap = mainMap;
	willbe_changed->initBitPos();
	willbe_changed->initPos();
	CLIENTS[my_id] = willbe_changed;
	delete p;

	sc_packet_gamestart packet;
	packet.size = sizeof(sc_packet_gamestart);
	packet.type = SC_PACKET_GAMESTART;
	packet.dir = CLIENTS[my_id]->dir;
	packet.h = CLIENTS[my_id]->h;
	packet.stage = 0;
	packet.state = CLIENTS[my_id]->state;
	packet.stealth = CLIENTS[my_id]->stealth;
	packet.x = CLIENTS[my_id]->x;
	packet.y = CLIENTS[my_id]->y;
	CLIENTS[my_id]->do_send(&packet, sizeof(packet));

}
void send_move_process(int c_id, int mover)
{
	sc_packet_move_process packet;
	packet.size = sizeof(sc_packet_move_process);
	packet.type = SC_PACKET_MOVE_PROCESS;
	packet.id = mover;
	packet.x = CLIENTS[mover]->x;
	packet.y = CLIENTS[mover]->y;
	packet.h = CLIENTS[mover]->h;
	packet.state = CLIENTS[mover]->state;
	packet.stealth = CLIENTS[mover]->stealth;
	packet.dir = CLIENTS[mover]->dir;
	packet.bx = CLIENTS[mover]->bx;
	CLIENTS[c_id]->do_send(&packet, sizeof(packet));
}


//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735 타임관련

DWORD WINAPI GameLogicThread(LPVOID arg)
{
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&Endtime);
	//time_t current_time = time(NULL);
	while (1) {
		//time_t frame_time = time(NULL) - current_time;
		//cout << "frame_time = "<<frame_time << endl;
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
				//cout << "FPS:" << Fps << endl;
				Fps = 0;
				elapsed_time = 0;
			}

			for (auto& c : CLIENTS)
			{
				if (c->mCss == CSS_LIVE) continue;
				//PlayerInputThread에서 mCss와 mSn을 바꿔주는데
				// mCss가 바뀌고 mSn이 바뀌기 전에(순서가 존재함) 이 아래 코드를 실행하면 동기화문제가 생기니
				// Event를 사용하여 동기화문제를 해결한다.
				WaitForSingleObject(c->SceneChangeTrigger, INFINITE);
				switch (c->mSn)
				{
				case SN_LOBBY:
					ChangeLoginToRobby(c->c_id);
					c->mCss = CSS_LIVE;
					SetEvent(c->SceneChangeIsDone);
					break;
				case SN_INGAME:
					ChangeRobbyToGame(c->c_id);
					c->mCss = CSS_LIVE;
					SetEvent(c->SceneChangeIsDone);
					break;
				}
				//서버에서 플레이어를 옮겨줬다면 SetEvent를 하여 클라이언트에게 바뀌었다고 패킷을 날림
				//1.서버처리 2.클라에게 패킷처리 [ 순서 존재 ]
			}


			
			for (int i = 0; i < Cnt_Player; ++i)
			{
				CLIENTS[i]->update(deltatime);
			}

			//현재 문제
			//1. 이동시 뚝뚝 끊긴다. v
			//2. 점프 시 obj와 충돌처리가 되어야 함. v
			//3. initpos?
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

		for (int i = 0; i < Cnt_Player; ++i)
		{
			send_move_process(i, c_id);

		}
	}
}


int main()
{
	wcout.imbue(locale("korean"));
	for (int i = 0; i < 3; ++i) //오브젝트 풀링
	{
		CLIENTS[i] = new LoginClient();
	}
	//맵 정보 All Loading
	mainMap = new Map();
	mainMap->LoadAllObjects();
	//


	auto mNet = Network::GetNetwork();
	mNet->InitServer();
	HANDLE hThread;
	HANDLE LogicThread;
	LogicThread = CreateThread(NULL, 0, GameLogicThread, 0, 0, NULL);
	for (int i = 0; i < 3; ++i,++Cnt_Player)
	{
		CLIENTS[i]->c_socket = mNet->AcceptClient(CLIENTS[i]->c_addr);
		CLIENTS[i]->c_id = i;
		//send_login_ok(i);
		hThread = CreateThread(NULL, 0, ClientInputThread, (LPVOID)i, 0, NULL);
		if (hThread == NULL) closesocket(CLIENTS[i]->c_socket);
	}
	while (1)
	{
		;
	}
	
	delete Network::GetNetwork();
	delete mainMap;
	return 0;
}


