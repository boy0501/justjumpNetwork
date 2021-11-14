#include <iostream>
#include <array>

#include "Network.h"
#include "CLIENT/Client.h"
#include "CLIENT/LoginClient.h"
#include "CLIENT/LobbyClient.h"
#include "../Protocol/protocol.h"

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
				cout << "FPS:" << Fps << endl;
				Fps = 0;
				elapsed_time = 0;
			}

			//�̷������� �α��ο��� �κ�Ŭ��� �ٲ۴�. Scene Change���� ������ �ϴ� ��.
			//���� 1��Ŭ�� �����ϸ� 10�ʵڿ� �α���Ŭ�󿡼� �κ�Ŭ��� ������ ������ �Ѵ�.
			//�����ζ�� �α��� ��ư�� ������ �α��� Ŭ�󿡼� �κ�Ŭ��� ������ �ǰ���.
			if (Cnt_Player > 0)
				change_time += deltatime;
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


			for (int i = 0; i < Cnt_Player; ++i)
			{
				CLIENTS[i]->update(deltatime);
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