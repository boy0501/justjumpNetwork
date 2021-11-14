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

//http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735 Ÿ�Ӱ���

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
			
			//�̷������� �α��ο��� �κ�Ŭ��� �ٲ۴�. Scene Change���� ������ �ϴ� ��.
			//���� 1��Ŭ�� �����ϸ� 10�ʵڿ� �α���Ŭ�󿡼� �κ�Ŭ��� ������ ������ �Ѵ�.
			//�����ζ�� �α��� ��ư�� ������ �α��� Ŭ�󿡼� �κ�Ŭ��� ������ �ǰ���.
			if (Cnt_Player > 0) {
				change_time += deltatime;
				//cout << change_time << endl;
				
			}
			if (change_time > 10 && do_once_change)
			{
				do_once_change = false;
				// 1. CLIENT�� ����ִ� Ŭ���������� ĳ������ �� ��, p�� ����ִ´�(�ٿ�ĳ����) 
				// ���� : delete �� �� �Ҵ��� ��ŭ ����������ϱ� ����.
				auto p = reinterpret_cast<LoginClient*>(CLIENTS[0]);
				// 2. ���Ӱ� �� Ŭ������ �Ҵ��Ѵ�.
				LobbyClient* tmp = new LobbyClient();
				// 3. ���Ӱ� �� Ŭ������ ���� p�� ������ �ִ´�.
				// ����: �α���-> �κ�� ������, �÷��̾��� ������ �ٽ� �������� �ȵǰ� �����Ǿ����.
				// �÷��̾��� ������ �θ�Ŭ������ Client�� �� ����Ǿ�����.
				*tmp = *reinterpret_cast<LobbyClient*>(p);
				//4. LobbyClient������ ����ϴ� ������ �ٽ� �ʱ�ȭ
				//�Լ�ȭ�� �ص� ������ �� ����.
				tmp->elapsedtime = 0;
				//5. CLIENTS�� �ٲ� class�� ����
				CLIENTS[0] = tmp;
				//6. ���� CLIENT�� �Ҵ�Ǿ��ִ� �޸𸮸� ����
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
		
		//���� ����
		//1. �̵��� �Ҷ� �����.
		//2. ���� �� obj�� �浹ó���� �Ǿ�� ��.
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
		//CLIENTS[i]->initPos(); //�÷��̾� ���� �ʱ�ȭ
		CLIENTS[i]->initBitPos();
		send_login_ok(i);
		hThread = CreateThread(NULL, 0, ClientInputThread, (LPVOID)i, 0, NULL);
		if (hThread == NULL) closesocket(CLIENTS[i]->c_socket);
	}
	//CLIENTS[0]->initPos(); //�÷��̾� ���� �ʱ�ȭ
	//CLIENTS[0]->bx = 0;
	//CLIENTS[0]->initBitPos();
	while (1)
	{
		;
	}

	Network::GetNetwork()->~Network();
	return 0;
}