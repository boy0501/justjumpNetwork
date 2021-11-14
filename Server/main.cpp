#include <iostream>
#include <array>

#include "Network.h"
#include "CLIENT/Client.h"
#include "CLIENT/LoginClient.h"
#include "CLIENT/LobbyClient.h"
#include "../Protocol/protocol.h"
#include <time.h>

#pragma comment(lib, "ws2_32")
using namespace std;

array<Client*, 3> CLIENTS;
int Cnt_Player = 0;
LARGE_INTEGER Frequency;
LARGE_INTEGER BeginTime;
LARGE_INTEGER Endtime;
float elapsed_time;
float change_time;
bool do_once_change = true;
int Fps = 0;

static int obj_t = 0;


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
			
			//이런식으로 로그인에서 로비클라로 바꾼다. Scene Change같은 역할을 하는 것.
			//현재 1번클라 접속하면 10초뒤에 로그인클라에서 로비클라로 보내는 역할을 한다.
			//예상대로라면 로그인 버튼을 누르면 로그인 클라에서 로비클라로 보내면 되겠지.
			if (Cnt_Player > 0) {
				change_time += deltatime;
				//cout << change_time << endl;
				
			}
			if (change_time > 10 && do_once_change)
			{
				do_once_change = false;
				// 1. CLIENT에 들어있는 클래스명으로 캐스팅을 한 후, p에 집어넣는다(다운캐스팅) 
				// 이유 : delete 할 때 할당한 만큼 해제해줘야하기 때문.
				auto p = reinterpret_cast<LoginClient*>(CLIENTS[0]);
				// 2. 새롭게 들어갈 클래스를 할당한다.
				LobbyClient* tmp = new LobbyClient();
				// 3. 새롭게 들어갈 클래스에 원래 p의 정보를 넣는다.
				// 이유: 로그인-> 로비로 가더라도, 플레이어의 정보가 다시 쓰여지면 안되고 유지되어야함.
				// 플레이어의 정보는 부모클래스인 Client에 다 저장되어있음.
				*tmp = *reinterpret_cast<LobbyClient*>(p);
				//4. LobbyClient에서만 사용하는 변수들 다시 초기화
				//함수화를 해도 괜찮을 것 같다.
				tmp->elapsedtime = 0;
				//5. CLIENTS에 바꿀 class를 연결
				CLIENTS[0] = tmp;
				//6. 원래 CLIENT에 할당되어있던 메모리를 해제
				delete p;
			}
			//

			obj_t++;
			for (int i = 0; i < Cnt_Player; ++i)
			{
				CLIENTS[i]->update(deltatime);

				CLIENTS[i]->move(obj_t);
			}

		}
		
		//현재 문제
		//1. 이동시 뚝뚝 끊긴다.
		//2. 점프 시 obj와 충돌처리가 되어야 함.
		//3. initpos?
		
	}
	return 0;
}

DWORD WINAPI ClientInputThread(LPVOID arg)
{
	int c_id = (int)arg;
	while (1)
	{
		
		cout << CLIENTS[c_id]->bx << endl;
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
		CLIENTS[i] = new LoginClient();
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
		//CLIENTS[i]->initPos(); //플레이어 정보 초기화
		CLIENTS[i]->initBitPos();
		send_login_ok(i);
		hThread = CreateThread(NULL, 0, ClientInputThread, (LPVOID)i, 0, NULL);
		if (hThread == NULL) closesocket(CLIENTS[i]->c_socket);
	}
	//CLIENTS[0]->initPos(); //플레이어 정보 초기화
	//CLIENTS[0]->bx = 0;
	//CLIENTS[0]->initBitPos();
	while (1)
	{
		;
	}

	Network::GetNetwork()->~Network();
	return 0;
}