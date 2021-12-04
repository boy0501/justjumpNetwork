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
#include "OBSTACLE/AttackObstacle.h"
#include "OBSTACLE/MoveObstacle.h"
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

void ChangeRobbyToGame(const int& c_id)
{
	int my_id = c_id;
	//auto p = reinterpret_cast<LobbyClient*>(CLIENTS[my_id]);
	//GameClient* willbe_changed = new GameClient();
	//auto Upcasting_changed = reinterpret_cast<Client*>(willbe_changed);
	//auto Upcasted_original = reinterpret_cast<Client*>(p);
	//*Upcasting_changed = *Upcasted_original;
	//willbe_changed->elapsedtime = 0;
	//willbe_changed->mStageNum = 1;
	//willbe_changed->mMap = mainMap;
	//willbe_changed->initBitPos();
	//willbe_changed->initPos();
	//CLIENTS[my_id] = willbe_changed;
	//delete p;

	CLIENTS[my_id]->elapsedtime = 0;
	CLIENTS[my_id]->mStageNum = 1;
	CLIENTS[my_id]->initBitPos();
	CLIENTS[my_id]->initPos();

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
void send_move_process(int player_id, int movePlayer_id)
{
	sc_packet_move_process packet;
	packet.size = sizeof(sc_packet_move_process);
	packet.type = SC_PACKET_MOVE_PROCESS;
	packet.id = movePlayer_id;
	packet.x = CLIENTS[movePlayer_id]->x;
	packet.y = CLIENTS[movePlayer_id]->y;
	packet.h = CLIENTS[movePlayer_id]->h;
	packet.state = CLIENTS[movePlayer_id]->state;
	packet.stealth = CLIENTS[movePlayer_id]->stealth;
	packet.dir = CLIENTS[movePlayer_id]->dir;
	packet.rank = CLIENTS[movePlayer_id]->rank;
	packet.hp = CLIENTS[movePlayer_id]->hp;
	//packet.bx = CLIENTS[mover]->bx;
	CLIENTS[player_id]->do_send(&packet, sizeof(packet));
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

			for (auto& c : mainMap->mObjects)
			{
				for (auto& t : c)
				{
					t->update(deltatime);
				}
			}

			// Scene Changer
			for (auto& c : CLIENTS)
			{
				if (c->SceneChangeTrigger == true)
				{
					c->SceneName = Scene_Name::SN_INGAME;
					c->SceneChangeTrigger = false;
					ChangeRobbyToGame(c->c_id);
				}
				//if (c->mCss == CSS_LIVE) continue;
				////PlayerInputThread에서 mCss와 mSn을 바꿔주는데
				//// mCss가 바뀌고 mSn이 바뀌기 전에(순서가 존재함) 이 아래 코드를 실행하면 동기화문제가 생기니
				//// Event를 사용하여 동기화문제를 해결한다.
				//WaitForSingleObject(c->SceneChangeTrigger, INFINITE);
				//
				//switch (c->mSn)
				//{
				//case SN_LOBBY:
				//{
				//	ChangeLoginToRobby(c->c_id);
				//	c->mCss = CSS_LIVE;
				//	auto lc = reinterpret_cast<LobbyClient*>(c);
				//	//WaitForSingleObject(lc->CountSendController, INFINITE);
				//
				//	robby_cnt += 1;
				//	//cout << "lc로비카운트는"<<robby_cnt << endl;
				//
				//	SetEvent(c->SceneChangeIsDone);
 				//	break;
				//}
				//case SN_INGAME: 
				//{
				//	//로비에서 인게임으로 다같이 가자
				//	
				//	ChangeRobbyToGame(c->c_id);
				//	c->mCss = CSS_LIVE;
				//	//SetEvent(c->SceneChangeIsDone);
				//
				//	break;
				//}
				//}
				//서버에서 플레이어를 옮겨줬다면 SetEvent를 하여 클라이언트에게 바뀌었다고 패킷을 날림
				//1.서버처리 2.클라에게 패킷처리 [ 순서 존재 ]
			}


			
			for (int i = 0; i < Cnt_Player; ++i)
			{			
				CLIENTS[i]->update(deltatime);	
				//send packet			
				for (int j = 0; j < Cnt_Player; ++j)
				{
					//맵이 서로 다르면 애초에 보내주질 않음.
					//if (CLIENTS[i]->mStageNum != CLIENTS[j]->mStageNum) continue;

					send_move_process(i, j);
					
				}
			}

			for (int i = 0; i < Cnt_Player; ++i)
			{
				auto& c = CLIENTS[i];
				if (c->SceneName == Scene_Name::SN_INGAME)
				{
					auto game = CLIENTS[i];
					int objNum = 0;
					for (auto& obj : game->mMap->mObjects[game->mStageNum])
					{
						switch (obj->type)
						{
						case 103: {
							auto steamobj = reinterpret_cast<AttackObstacle*>(obj);
							sc_packet_object_sync packet;
							packet.size = sizeof(sc_packet_object_sync);
							packet.type = SC_PACKET_OBJECT_SYNC;
							packet.objnum = objNum;
							packet.index = steamobj->index;
							game->do_send(&packet, sizeof(packet));
							break;
						}
						case 106:
						case 107: {
							sc_packet_object_sync packet;
							packet.size = sizeof(sc_packet_object_sync);
							packet.type = SC_PACKET_OBJECT_SYNC;
							auto moveobj = reinterpret_cast<MoveObstacle*>(obj);
							packet.objnum = objNum;
							packet.mx = moveobj->mx;
							packet.my = moveobj->my;
							game->do_send(&packet, sizeof(packet));
							break;
						}
						}
						objNum++;
					}
				}
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
		switch (c_id)
		{
		case 0: {
			if (CLIENTS[1]->is_active || CLIENTS[2]->is_active)
				WaitForSingleObject(Client0Event, INFINITE);
			break;
		}
		case 1: {
			if (CLIENTS[0]->is_active || CLIENTS[2]->is_active)
				WaitForSingleObject(Client1Event, INFINITE);
			break;
		}
		case 2: {
			if (CLIENTS[0]->is_active || CLIENTS[1]->is_active)
				WaitForSingleObject(Client2Event, INFINITE);
			break;
		}
		}
		int ret = CLIENTS[c_id]->do_recv();		
		if (ret == SOCKET_ERROR)
		{
			int err_no = WSAGetLastError();
			error_display(err_no);
			closesocket(CLIENTS[c_id]->c_socket);
			return 0;
		}
		switch (c_id)
		{
		case 0: {
			if (CLIENTS[1]->is_active == true)
				SetEvent(Client1Event);
			else if(CLIENTS[2]->is_active == true)
				SetEvent(Client2Event);
			break;
		}
		case 1: {
			if (CLIENTS[2]->is_active == true)
				SetEvent(Client2Event);
			else if(CLIENTS[0]->is_active == true)
				SetEvent(Client0Event);

			break;
		}
		case 2: {
			if (CLIENTS[0]->is_active == true)
				SetEvent(Client0Event);
			else if (CLIENTS[1]->is_active == true)
				SetEvent(Client1Event);
			break;
		}
		}

	}
}


int main()
{
	wcout.imbue(locale("korean"));

	//맵 정보 All Loading
	mainMap = new Map();
	mainMap->LoadAllObjects();

	for (int i = 0; i < 3; ++i) //오브젝트 풀링
	{
		CLIENTS[i] = new Client();
		CLIENTS[i]->mMap = mainMap;
	}
	//
	Client0Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	Client1Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	Client2Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	auto mNet = Network::GetNetwork();
	mNet->InitServer();
	HANDLE hThread;
	HANDLE LogicThread;
	LogicThread = CreateThread(NULL, 0, GameLogicThread, 0, 0, NULL);


	for (int i = 0; i < 3; ++i)
	{
		CLIENTS[i]->c_socket = mNet->AcceptClient(CLIENTS[i]->c_addr);
		CLIENTS[i]->c_id = i;
		CLIENTS[i]->is_ingame = false;
		CLIENTS[i]->is_active = true;
		hThread = CreateThread(NULL, 0, ClientInputThread, (LPVOID)i, 0, NULL);
		if (hThread == NULL) closesocket(CLIENTS[i]->c_socket);
	}
	while (1)
	{
		int flag = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (CLIENTS[i]->is_active == false)
				flag++;
		}

		if (flag == 3)
			break;
	}

	for (int i = 0; i < 3; ++i)
	{
		if (CLIENTS[i] != nullptr)
			delete CLIENTS[i];
		CLIENTS[i] = nullptr;
	}

	CloseHandle(Client0Event);
	CloseHandle(Client1Event);
	CloseHandle(Client2Event);
	delete Network::GetNetwork();
	delete mainMap;

	return 0;
}


