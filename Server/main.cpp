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

	//login Button ���� �÷��̾�� ���� �ͼ� ��μ� active�� �ȴ�.
	CLIENTS[my_id]->is_ingame = true;
	//send_ok_packet me and other
	for (auto& c : CLIENTS)
	{
		if (c->is_ingame == false) continue;
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
			packet.rank = CLIENTS[my_id]->rank;
			//
			//packet.bx = CLIENTS[my_id]->bx;
			
			c->do_send(&packet, sizeof(packet));
		}
	}
	//send_ok_packet ���沬 ������
	for (auto& c : CLIENTS)
	{
		if (c->is_ingame == false) continue;
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
		packet.rank = c->rank;
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




//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735 Ÿ�Ӱ���

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
				if (c->mCss == CSS_LIVE) continue;
				//PlayerInputThread���� mCss�� mSn�� �ٲ��ִµ�
				// mCss�� �ٲ�� mSn�� �ٲ�� ����(������ ������) �� �Ʒ� �ڵ带 �����ϸ� ����ȭ������ �����
				// Event�� ����Ͽ� ����ȭ������ �ذ��Ѵ�.
				WaitForSingleObject(c->SceneChangeTrigger, INFINITE);

				switch (c->mSn)
				{
				case SN_LOBBY:
				{
					ChangeLoginToRobby(c->c_id);
					c->mCss = CSS_LIVE;
					auto lc = reinterpret_cast<LobbyClient*>(c);
					//WaitForSingleObject(lc->CountSendController, INFINITE);

					robby_cnt += 1;
					//cout << "lc�κ�ī��Ʈ��"<<robby_cnt << endl;

					SetEvent(c->SceneChangeIsDone);
 					break;
				}
				case SN_INGAME: 
				{
					//�κ񿡼� �ΰ������� �ٰ��� ����
					
					ChangeRobbyToGame(c->c_id);
					c->mCss = CSS_LIVE;
					//SetEvent(c->SceneChangeIsDone);

					break;
				}
				}
				//�������� �÷��̾ �Ű���ٸ� SetEvent�� �Ͽ� Ŭ���̾�Ʈ���� �ٲ���ٰ� ��Ŷ�� ����
				//1.����ó�� 2.Ŭ�󿡰� ��Ŷó�� [ ���� ���� ]
			}


			
			for (int i = 0; i < Cnt_Player; ++i)
			{			
				CLIENTS[i]->update(deltatime);	
				//send packet			
				for (int j = 0; j < Cnt_Player; ++j)
				{
					//���� ���� �ٸ��� ���ʿ� �������� ����.
					//if (CLIENTS[i]->mStageNum != CLIENTS[j]->mStageNum) continue;
					// 
				
					//WaitForSingleObject(c->key_seperate, 10);
					send_move_process(i, j);
					//SetEvent(c->key_seperate);
				}
			}

			for (int i = 0; i < Cnt_Player; ++i)
			{
				auto& c = CLIENTS[i];
				if (c->mSn == Scene_Name::SN_INGAME)
				{
					auto game = reinterpret_cast<GameClient*>(CLIENTS[i]);
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
							packet.degree = moveobj->degree;
							game->do_send(&packet, sizeof(packet));
							break;
						}
						}
						objNum++;
					}
				}
			}

			//���� ����
			//1. �̵��� �Ҷ� �����. v
			//2. ���� �� obj�� �浹ó���� �Ǿ�� ��. v
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
			else
				SetEvent(Client2Event);
			break;
		}
		case 1: {
			if (CLIENTS[2]->is_active == true)
				SetEvent(Client2Event);
			else
				SetEvent(Client0Event);

			break;
		}
		case 2: {
			if (CLIENTS[0]->is_active == true)
				SetEvent(Client0Event);
			else
				SetEvent(Client1Event);
			break;
		}
		}

	}
}


int main()
{
	wcout.imbue(locale("korean"));
	for (int i = 0; i < 3; ++i) //������Ʈ Ǯ��
	{
		CLIENTS[i] = new LoginClient();
	}
	//�� ���� All Loading
	mainMap = new Map();
	mainMap->LoadAllObjects();
	//
	Client0Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	Client1Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	Client2Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	Client0SceneChangeTrigger = CreateEvent(NULL, FALSE, FALSE, NULL);
	Client1SceneChangeTrigger = CreateEvent(NULL, FALSE, FALSE, NULL);
	Client2SceneChangeTrigger = CreateEvent(NULL, FALSE, FALSE, NULL);
	Client0SceneChangeIsDone = CreateEvent(NULL, FALSE, FALSE, NULL);
	Client1SceneChangeIsDone = CreateEvent(NULL, FALSE, FALSE, NULL);
	Client2SceneChangeIsDone = CreateEvent(NULL, FALSE, FALSE, NULL);
	CLIENTS[0]->SceneChangeTrigger = Client0SceneChangeTrigger;
	CLIENTS[0]->SceneChangeIsDone = Client0SceneChangeIsDone;
	CLIENTS[1]->SceneChangeTrigger = Client1SceneChangeTrigger;
	CLIENTS[1]->SceneChangeIsDone = Client1SceneChangeIsDone;
	CLIENTS[2]->SceneChangeTrigger = Client2SceneChangeTrigger;
	CLIENTS[2]->SceneChangeIsDone = Client2SceneChangeIsDone;
	auto mNet = Network::GetNetwork();
	mNet->InitServer();
	HANDLE hThread;
	HANDLE LogicThread;
	LogicThread = CreateThread(NULL, 0, GameLogicThread, 0, 0, NULL);


	for (int i = 0; i < 3; ++i,++Cnt_Player)
	{
		CLIENTS[i]->c_socket = mNet->AcceptClient(CLIENTS[i]->c_addr);
		CLIENTS[i]->c_id = i;
		CLIENTS[i]->is_ingame = false;
		CLIENTS[i]->is_active = true;
		//send_login_ok(i);
		hThread = CreateThread(NULL, 0, ClientInputThread, (LPVOID)i, 0, NULL);
		if (hThread == NULL) closesocket(CLIENTS[i]->c_socket);
	}
	while (1)
	{
		;
	}
	
	for (int i = 0; i < 3; ++i)
	{
		if (CLIENTS[i] != nullptr)
			delete CLIENTS[i];
	}
	CloseHandle(Client0Event);
	CloseHandle(Client1Event);
	CloseHandle(Client2Event);
	delete Network::GetNetwork();
	delete mainMap;

	return 0;
}


