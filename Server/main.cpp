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
LARGE_INTEGER Frequency;
LARGE_INTEGER BeginTime;
LARGE_INTEGER Endtime;
float elapsed_time;
int Fps = 0;



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
	willbe_changed->mMap = mainMap;
	willbe_changed->initBitPos();
	willbe_changed->initPos();
	CLIENTS[my_id] = willbe_changed;
	delete p;

	//send_ok_packet me and other
	for (auto& c : CLIENTS)
	{
		if (c->is_active == false) continue;
		if (c->c_id == my_id)
		{
			sc_packet_login_ok packet;
			packet.size = sizeof(sc_packet_login_ok);
			packet.type = SC_PACKET_LOGIN_OK;
			packet.id = my_id;
			packet.x = CLIENTS[my_id]->x;
			packet.y = CLIENTS[my_id]->y;
			packet.stage = 1;
			c->do_send(&packet, sizeof(packet));
		}
		else {
			sc_packet_put_object packet;
			packet.size = sizeof(sc_packet_put_object);
			packet.type = SC_PACKET_PUT_OBJECT;
			packet.dir = CLIENTS[my_id]->dir;
			packet.h = CLIENTS[my_id]->h;
			packet.hp = CLIENTS[my_id]->hp;
			packet.id = my_id;
			packet.state = CLIENTS[my_id]->state;
			packet.stealth = CLIENTS[my_id]->stealth;
			strcpy_s(packet.username, 20, CLIENTS[my_id]->playername);
			packet.x = CLIENTS[my_id]->x;
			packet.y = CLIENTS[my_id]->y;
			packet.w = CLIENTS[my_id]->w;
			//packet.rank = CLIENTS[my_id]->rank;
			//
			//packet.bx = CLIENTS[my_id]->bx;
			c->do_send(&packet, sizeof(packet));
		}
	}
	//send_ok_packet 상대방껄 나에게
	for (auto& c : CLIENTS)
	{
		if (c->is_active == false) continue;
		if (c->c_id == my_id)continue;

		sc_packet_put_object packet;
		packet.size = sizeof(sc_packet_put_object);
		packet.type = SC_PACKET_PUT_OBJECT;
		packet.dir = c->dir;
		packet.h = c->h;
		packet.hp = c->hp;
		packet.id = c->c_id;
		packet.state = c->state;
		packet.stealth = c->stealth;
		strcpy_s(packet.username, 20, c->playername);
		packet.x = c->x;
		packet.y = c->y;
		packet.w = c->w;
		//packet.rank = c->rank;
		CLIENTS[my_id]->do_send(&packet, sizeof(packet));
	}
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
	packet.stage = 9;
	packet.state = CLIENTS[my_id]->state;
	packet.stealth = CLIENTS[my_id]->stealth;
	packet.x = CLIENTS[my_id]->x;
	packet.y = CLIENTS[my_id]->y;
	packet.COMMAND_die = CLIENTS[my_id]->COMMAND_die;
	CLIENTS[my_id]->do_send(&packet, sizeof(packet));

}
void send_move_process(int c_id)
{
	sc_packet_move_process packet;
	packet.size = sizeof(sc_packet_move_process);
	packet.type = SC_PACKET_MOVE_PROCESS;
	packet.id = c_id;
	packet.x = CLIENTS[c_id]->x;
	packet.y = CLIENTS[c_id]->y;
	packet.h = CLIENTS[c_id]->h;
	packet.state = CLIENTS[c_id]->state;
	packet.stealth = CLIENTS[c_id]->stealth;
	packet.dir = CLIENTS[c_id]->dir;
	packet.rank = CLIENTS[c_id]->rank;
	//packet.bx = CLIENTS[mover]->bx;
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
				//cout << "FPS:" << Fps << endl;
				Fps = 0;
				elapsed_time = 0;
				
			}

			// Scene Changer
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
				{
					ChangeLoginToRobby(c->c_id);
					c->mCss = CSS_LIVE;
					auto lc = reinterpret_cast<LobbyClient*>(c);
					lc->robby_cnt = Cnt_Player;
					//cout << "lc로비카운트는"<<lc->robby_cnt << endl;

					SetEvent(c->SceneChangeIsDone);
 					break;
				}
				case SN_INGAME: 
				{
					//로비에서 인게임으로 다같이 가자
					for (int i = 0; i < Cnt_Player; ++i) {
						ChangeRobbyToGame(i);
						
					}
					
					c->mCss = CSS_LIVE;
					SetEvent(c->SceneChangeIsDone);

					break;
				}
				}
				//서버에서 플레이어를 옮겨줬다면 SetEvent를 하여 클라이언트에게 바뀌었다고 패킷을 날림
				//1.서버처리 2.클라에게 패킷처리 [ 순서 존재 ]
			}


			
			for (int i = 0; i < Cnt_Player; ++i)
			{
				CLIENTS[i]->update(deltatime);

				
				auto& c = CLIENTS[i];
				//send packet
				sc_packet_move_process packet;
				packet.size = sizeof(sc_packet_move_process);
				packet.type = SC_PACKET_MOVE_PROCESS;
				packet.dir = c->dir;
				packet.h = c->h;
				packet.id = c->c_id;
				packet.state = c->state;
				packet.stealth = c->stealth;
				packet.x = c->x;
				packet.y = c->y;
				packet.rank = c->rank;
				for (int j = 0; j < Cnt_Player; ++j)
				{
					//맵이 서로 다르면 애초에 보내주질 않음.
					if (CLIENTS[i]->mStageNum != CLIENTS[j]->mStageNum) continue;

					CLIENTS[j]->do_send(&packet, sizeof(packet));
					
				}
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

		/*for (int i = 0; i < Cnt_Player; ++i)
		{
			send_move_process(i);

		}*/
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
		CLIENTS[i]->is_active = true;
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


