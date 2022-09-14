#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <atlconv.h>
#include "Network.h"

#include "../../Protocol/protocol.h"
#include "Load.h"
#include "player.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "Map.h"
#include "StartHUD.h"

Network* Network::mNetwork = nullptr;

HDC mem1dc, pdc, hdc;
HWND hWnd;
RECT rectview;
HBITMAP hbit1;
HINSTANCE g_hinst;
int obj_t = 0;
CAMERA camera;

void error_display(int err_no)
{
	wchar_t* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf << std::endl;
	//while (true);
	LocalFree(lpMsgBuf);
}
Network::Network()
	: mPlayer(nullptr)
	, prev_size(0)
	, mOcount(nullptr)
	, countdown(11)
{
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	BOOL optval = true;
	setsockopt(s_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));

}


Network::~Network()
{
	closesocket(s_socket);
	WSACleanup();
}


Network* Network::GetNetwork()
{
	if (mNetwork == nullptr)
		mNetwork = new Network();
	return mNetwork;
}


int Network::ConnectServer(const char* server_ip)
{
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	int ret = connect(s_socket, (SOCKADDR*)(&server_addr), sizeof(server_addr));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "�������" << std::endl;
		return -1;
		//exit(0);
		
	}
}

int Network::C_Recv()
{
	int received;
	
	char* ptr = reinterpret_cast<char*>(buf + prev_size);
	received = recv(s_socket, ptr, MAX_BUF_SIZE - prev_size, 0);
	if (received == SOCKET_ERROR)
	{
		return SOCKET_ERROR;
	}
	int remain_data = received + prev_size;
	unsigned char* packet_start = reinterpret_cast<unsigned char*>(buf);
	int packet_size = packet_start[0];

	while (packet_size <= remain_data)
	{
		ProcessPacket(packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}
	prev_size = remain_data;
	if (0 < remain_data)
	{
		memcpy(&buf, packet_start, remain_data);
	}
	return 0;
}


void Network::ProcessPacket(unsigned char* p)
{
	unsigned char packet_type = p[1];
	switch (packet_type) {
	case SC_PACKET_LOGIN_OK: {
		sc_packet_login_ok* packet = reinterpret_cast<sc_packet_login_ok*>(p);
		mPlayer->stage = packet->stage;
		mPlayer->player_cid = packet->id;
		//�Ӱ迵�� �ڸ�
		mPlayer->x = packet->x;
		mPlayer->y = packet->y;
		//�Ӱ迵�� �ڸ�
		mMap->setmapnum(9);
		*mOcount = initObject(mObj, mMap->getmapnum(), g_hinst);
		mMap->CreateMap(g_hinst);
		LoadBK(hbit1, g_hinst, 9);
		mCamera->setx(0);
		mCamera->sety(0);
		//player.setx(80);
		//player.sety(655);
		Sound::GetSelf()->Sound_Play(BGMSOUND, MAINMENUBGM, BGMVOL);

		break;
	}
	case SC_PACKET_PUT_OBJECT: {
		sc_packet_put_object* packet = reinterpret_cast<sc_packet_put_object*>(p);
		auto id = packet->id;
		mOthers[id].is_active = true;
		mOthers[id].dir=packet->dir;
		mOthers[id].h=packet->h;
		mOthers[id].hp=packet->hp;
		mOthers[id].player_cid=packet->id;
		mOthers[id].state=packet->state;
		mOthers[id].stealth=packet->stealth;
		mOthers[id].mPlayername = packet->username;
		USES_CONVERSION;
		mOthers[id].mPlayerwname = wstring(A2W(mOthers[id].mPlayername.c_str()));
		mOthers[id].w = packet->w;
		mOthers[id].x=packet->x;
		mOthers[id].y=packet->y;
		break;
	}
	case SC_PACKET_LOGOUT_OBJECT: {
		sc_packet_logout_object* packet = reinterpret_cast<sc_packet_logout_object*>(p);
		mOthers[packet->id].is_active = false;
		
		break;
	}
	case SC_PACKET_PORTAL: {
		sc_packet_portal* packet = reinterpret_cast<sc_packet_portal*>(p);
		mPlayer->stage = packet->stagenum;
		occur_button = 0;
		mMap->setblack_t(100);
		mMap->setmapnum(mPlayer->stage + 9);
		for (int j = 0; j < *mOcount; j++)
			mObj[j].ResetObject();

		*mOcount = initObject(mObj, mMap->getmapnum(), g_hinst);
		mMap->CreateMap(g_hinst);
		LoadBK(hbit1, g_hinst, mMap->getmapnum());

		Sound::GetSelf()->setindex(Sound::GetSelf()->getindex() + 1);
		Sound::GetSelf()->Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
		Sound::GetSelf()->Sound_Play(BGMSOUND, FIRSTMAPBGM, BGMVOL);
		//---sethp�� ��Ŷ���� �Ѱܹ����� �� �κ� �� �������ּ��� 
		mPlayer->sethp(mPlayer->hp); //(jpark Ȯ��)
		//---
		break;
	}
	case SC_PACKET_LOBBY: {
		sc_packet_lobby* packet = reinterpret_cast<sc_packet_lobby*>(p);
		
		if (countdown != packet->countdown)
			cntdown_controller = true;

		countdown = packet->countdown;

		break;
	}
	case SC_PACKET_MOVE_PROCESS: {
		sc_packet_move_process* packet = reinterpret_cast<sc_packet_move_process*>(p);

		if (packet->id == mPlayer->player_cid)
		{
			mPlayer->h = packet->h;
			mPlayer->state = packet->state;
			mPlayer->stealth = packet->stealth;
			mPlayer->dir = packet->dir;
			mPlayer->hp = packet->hp;
			mPlayer->rank = packet->rank;

			mPlayer->x = packet->x;
			mPlayer->y = packet->y;
			mPlayer->vx = packet->vx;
			mPlayer->vy = packet->vy;
			if (packet->hp <= 0)
			{
				if (mPlayer->WhenPlayerDied == false)
				{
					mPlayer->WhenPlayerDied = true;
					mMap->mDieUi->activeUI();
					mUI.emplace_back(mMap->mDieUi);
				}
			}
			else {
				mPlayer->WhenPlayerDied = false;
			}
		}
		else {
			auto& other = mOthers[packet->id];
			other.h = packet->h;
			other.state = packet->state;
			other.stealth = packet->stealth;
			other.dir = packet->dir;
			other.hp = packet->hp;
			other.rank = packet->rank;
			other.x = packet->x;
			other.y = packet->y;
		}

		break;
	}
	case SC_PACKET_GAMESTART:{
		sc_packet_gamestart* packet = reinterpret_cast<sc_packet_gamestart*>(p);
		mPlayer->dir = packet->dir;
		mPlayer->h = packet->h;
		mPlayer->stage = packet->stage;
		mPlayer->state = packet->state;
		mPlayer->stealth = packet->stealth;
		mPlayer->x = packet->x;
		mPlayer->y = packet->y;
		mPlayer->COMMAND_die = packet->COMMAND_die;

		occur_button = 0;
		mMap->setblack_t(100);
		mMap->setmapnum(mPlayer->stage + 1);
		for (int j = 0; j < *mOcount; j++)
			mObj[j].ResetObject();

		*mOcount = initObject(mObj, mMap->getmapnum(), g_hinst);
		mMap->CreateMap(g_hinst);
		LoadBK(hbit1, g_hinst, mMap->getmapnum());

		Sound::GetSelf()->setindex(Sound::GetSelf()->getindex() + 1);
		Sound::GetSelf()->Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
		Sound::GetSelf()->Sound_Play(BGMSOUND, FIRSTMAPBGM, BGMVOL);

		mCamera->setx(0);
		mCamera->sety(3232);

		mMap->mStartui->closeUI();
		mUI.emplace_back(mMap->mGameUi);
		cout << "���� ��ŸƮ!" << endl;
		
		break;

	}
	case SC_PACKET_OBJECT_SYNC: {
		sc_packet_object_sync* packet = reinterpret_cast<sc_packet_object_sync*>(p);
		auto& obj = mObj[packet->objnum];
		switch (obj.type)
		{
		case 103: {
			obj.index = packet->index;
			break;
		}
		case 106: 
		case 107: {
			obj.mx = packet->mx;
			obj.my = packet->my;
			obj.degree = packet->degree;
			obj.velocityDegree = packet->vd;
			break;
		}
		}
		break;//
	}
	}
	
}

void Network::C_Send(void* packet, int bytes)
{
	unsigned char buf[256];
	memcpy(buf, packet, bytes);
	send(s_socket, reinterpret_cast<char*>(buf), bytes, 0);
}