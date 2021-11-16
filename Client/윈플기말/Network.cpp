
#include "Network.h"

#include "../../Protocol/protocol.h"
#include "Load.h"
#include "player.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "Map.h"

Network* Network::mNetwork = nullptr;

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
{
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	s_socket = socket(AF_INET, SOCK_STREAM, 0);

}


Network::~Network()
{
	closesocket(s_socket);
	WSACleanup();
	delete mNetwork;
}


Network* Network::GetNetwork()
{
	if (mNetwork == nullptr)
		mNetwork = new Network();
	return mNetwork;
}


void Network::ConnectServer(const char* server_ip)
{
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	int ret = connect(s_socket, (SOCKADDR*)(&server_addr), sizeof(server_addr));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "연결오류" << std::endl;
		exit(0);
		
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
	unsigned char* packet_start = reinterpret_cast<unsigned char*>(ptr);
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
	cout << (int)packet_type << endl;
	switch (packet_type) {
	case SC_PACKET_LOGIN_OK: {
		sc_packet_login_ok* packet = reinterpret_cast<sc_packet_login_ok*>(p);
		mPlayer->stage = packet->stage;
		mPlayer->player_cid = packet->id;
		mPlayer->x = packet->x;
		mPlayer->y = packet->y;

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
	case SC_PACKET_ROBBY: {
		sc_packet_robby* packet = reinterpret_cast<sc_packet_robby*>(p);
		/*if ((int)(packet->count_start) == 1) {
			mPlayer->ready_to_go = true;
		}*/
		if (packet->player_cnt == 1) {
			mPlayer->ready_to_go = true;

		}
		break;
	}
	case SC_PACKET_MOVE_PROCESS: 
	{
		sc_packet_move_process* packet = reinterpret_cast<sc_packet_move_process*>(p);
		
		//std::cout << packet->x << "," << packet->y << std::endl;
		//std::cout << (int)packet->bx << std::endl;

		mPlayer->x = packet->x;
		mPlayer->y = packet->y;
		mPlayer->h = packet->h;
		mPlayer->state = packet->state;
		mPlayer->stealth = packet->stealth;
		mPlayer->dir = packet->dir;
		mPlayer->bx = packet->bx;

		break;
	}
	case SC_PACKET_GAMESTART:
	{
		sc_packet_gamestart* packet = reinterpret_cast<sc_packet_gamestart*>(p);
		mPlayer->dir = packet->dir;
		mPlayer->h = packet->h;
		mPlayer->stage = packet->stage;
		mPlayer->state = packet->state;
		mPlayer->stealth = packet->stealth;
		mPlayer->x = packet->x;
		mPlayer->y = packet->y;

		break;

	}
	}
	
}

void Network::C_Send(void* packet, int bytes)
{
	unsigned char buf[256];
	memcpy(buf, packet, bytes);
	send(s_socket, reinterpret_cast<char*>(buf), bytes, 0);
}