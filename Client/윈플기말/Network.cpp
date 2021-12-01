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
		std::cout << "연결오류" << std::endl;
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

void Network::test()
{
	/*occur_button = 0;
	mMap->setblack_t(50);
	mMap->setmapnum(10);
	for (int j = 0; j < ocount; j++)
		obj[j].ResetObject();

	ocount = initObject(obj, mMap->getmapnum(), g_hinst);
	mMap->CreateMap(g_hinst);

	LoadBK(hbit1, g_hinst, 10);*/

	//hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//Sound::GetSelf()->setindex(Sound::GetSelf()->getindex() + 1);
	//Sound::GetSelf()->Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
	//Sound::GetSelf()->Sound_Play(BGMSOUND, FIRSTMAPBGM, BGMVOL);
	//mPlayer->initPos();
	//mPlayer->sethp(5);
	//mCamera->setx(0);
	//mCamera->sety(3232);
	//mMap->mStartui->closeUI();

	//mUI.emplace_back(mMap->mGameUi);

	/*auto it = find(mUI.begin(), mUI.end(), mMap->mGameUi);
	if (it == mUI.end()) {
		cout << mMap->mGameUi << "은 찾을 수 없습니다.\n";
	}
	else {
		cout << mMap->mGameUi << "는 존재하며 인덱스는 " << it - mUI.begin() << " 입니다.\n";
	}*/
	cout << "게임 스타트!" << endl;
	
}

void Network::ProcessPacket(unsigned char* p)
{
	unsigned char packet_type = p[1];
	switch (packet_type) {
	case SC_PACKET_LOGIN_OK: {
		isLogin = false;
		sc_packet_login_ok* packet = reinterpret_cast<sc_packet_login_ok*>(p);
		mPlayer->stage = packet->stage;
		mPlayer->player_cid = packet->id;
		//임계영역 자리
		mPlayer->x = packet->x;
		mPlayer->y = packet->y;
		mPlayer->oldY = packet->y;
		mPlayer->oldX = packet->x;
		//임계영역 자리
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
		mOthers[id].rank = packet->rank;
		USES_CONVERSION;
		mOthers[id].mPlayerwname = wstring(A2W(mOthers[id].mPlayername.c_str()));
		mOthers[id].w = packet->w;
		//임계영역 자리 
		mOthers[id].x=packet->x;
		mOthers[id].y=packet->y;
		mOthers[id].oldX = packet->x;
		mOthers[id].oldY = packet->y;
		//임계영역 자리
		mOthers[id].rank = packet->rank;
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


		//hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Sound::GetSelf()->setindex(Sound::GetSelf()->getindex() + 1);
		Sound::GetSelf()->Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
		Sound::GetSelf()->Sound_Play(BGMSOUND, FIRSTMAPBGM, BGMVOL);
		//mPlayer->initPos();
		//mPlayer->oldY = mPlayer->y;
		//mPlayer->oldX = mPlayer->x;
		//---sethp를 패킷으로 넘겨받으면 이 부분 꼭 수정해주세요 
		mPlayer->sethp(mPlayer->hp); //(jpark 확인)
		//---
		break;
	}
	case SC_PACKET_ROBBY: {
		sc_packet_robby* packet = reinterpret_cast<sc_packet_robby*>(p);
		
		if (countdown != packet->countdown)
			cntdown_controller = true;

			//init_x += 20;
		countdown = packet->countdown;

		//std::cout << packet->countdown << std::endl;
		break;
	}
	case SC_PACKET_MOVE_PROCESS: {
		if (isLogin == true) break;
		sc_packet_move_process* packet = reinterpret_cast<sc_packet_move_process*>(p);

		//std::cout << packet->x << "," << packet->y << std::endl;
		//std::cout << (int)packet->bx << std::endl;
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
			//속도구하는 공식 = 거리 /시간 => (지금패킷위치 - 예전패킷위치) / 걸린시간  
			mPlayer->velocityX = (packet->x - mPlayer->oldX) / (packet->senddeltatime);
			mPlayer->velocityY = (packet->y - mPlayer->oldY) / (packet->senddeltatime);

			mPlayer->oldX = packet->x;
			mPlayer->oldY = packet->y;

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
			//------
			//rank = packet->rank;
			//------
		}
		else {
			auto& other = mOthers[packet->id];
			other.h = packet->h;
			other.state = packet->state;
			other.stealth = packet->stealth;
			other.dir = packet->dir;
			other.hp = packet->hp;
			other.rank = packet->rank;
			//임계영역 자리 ----
			other.x = packet->x;
			other.y = packet->y;
			//속도구하는 공식 = 거리 /시간 => (지금패킷위치 - 예전패킷위치) / 걸린시간  
			other.velocityX = (packet->x - other.oldX) / (packet->senddeltatime);
			other.velocityY = (packet->y - other.oldY) / (packet->senddeltatime);
			//mPlayer->reckoningY = (mPlayer->y - mPlayer->oldY) / 2;
			other.oldX = packet->x;
			other.oldY = packet->y;
			//임계영역 자리 ----
			//std::cout << "아덜좌표 y : " << other.y << std::endl;
			//other.bx = packet->bx;
		}

		break;
	}
	case SC_PACKET_GAMESTART:{
		sc_packet_gamestart* packet = reinterpret_cast<sc_packet_gamestart*>(p);
		//임계영역 자리
		mPlayer->dir = packet->dir;
		mPlayer->h = packet->h;
		mPlayer->stage = packet->stage;
		mPlayer->state = packet->state;
		mPlayer->stealth = packet->stealth;
		mPlayer->x = packet->x;
		mPlayer->y = packet->y;
		mPlayer->oldX = packet->x;
		mPlayer->oldY = packet->y;
		//임계영역 자리
		mPlayer->COMMAND_die = packet->COMMAND_die;

		//auto gameui = make_shared<GameHUD>(1, *mPlayer);

		occur_button = 0;
		mMap->setblack_t(100);
		mMap->setmapnum(mPlayer->stage + 1);
		for (int j = 0; j < *mOcount; j++)
			mObj[j].ResetObject();

		*mOcount = initObject(mObj, mMap->getmapnum(), g_hinst);
		mMap->CreateMap(g_hinst);
		LoadBK(hbit1, g_hinst, mMap->getmapnum());


		//hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Sound::GetSelf()->setindex(Sound::GetSelf()->getindex() + 1);
		Sound::GetSelf()->Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
		Sound::GetSelf()->Sound_Play(BGMSOUND, FIRSTMAPBGM, BGMVOL);
		//mPlayer->initPos();
		//mPlayer->sethp(5);
		//mPlayer->oldX = mPlayer->x;
		//mPlayer->oldY = mPlayer->y;
		//cout << mCamera->getx() << ", " << mCamera->gety() << endl;
		
		//cout << mPlayer->x <<", "<< mPlayer->y<<endl;
		//cout << mCamera->getx() << ", " << mCamera->gety() << endl;

		mCamera->setx(0);
		mCamera->sety(3232);

		mMap->mStartui->closeUI();
		mUI.emplace_back(mMap->mGameUi);
		//auto gameui = make_shared<GameHUD>(1, *mPlayer);
		//gameui->LoadUiBitmap(g_hinst, "img/NoNameUi.bmp", 400, 700, 199, 65, RGB(0, 255, 0), *mCamera);
		//gameui->addText(mPlayer->mPlayerwname, "NickName", L"메이플스토리 light", RGB(255, 255, 255), 14, 475, 705, true, 100, 65, *mCamera);
		//gameui->LoadHpUiBitmap(g_hinst, "img/Ui_HP.bmp", 421, 728, 100, 65, RGB(0, 0, 255), *mCamera);

		//startui->closeUI();
		//mMap->mGameUi = make_shared<GameHUD>(1, *mPlayer);
		//BitBlt(hdc, 0, 0, 1024, 768, mem1dc, camera.getx(), camera.gety(), SRCCOPY);

		//mMap->mGameUi = gameui;
		
		/*auto it = find(mUI.begin(), mUI.end(), mMap->mGameUi);
		if (it == mUI.end()) {
			cout << mMap->mGameUi << "은 찾을 수 없습니다.\n";
		}
		else {
			cout << mMap->mGameUi << "는 존재하며 인덱스는 " << it - mUI.begin() << " 입니다.\n";
		}*/
		cout << "게임 스타트!" << endl;
		
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
			//obj.degree = packet->degree;
			//obj.velocityDegree = (packet->degree - obj.oldDegree) / (packet->senddeltatime);
			//if (obj.velocityDegree < 0)
			//{
			//	//각도는 0 = 360인데 old는 360에서 362로 갈것으로 기대하고있지만, 실제 degree는 0으로 가서 생기는 - 값 보정
			//	obj.velocityDegree = (packet->degree + 360 - obj.oldDegree) / (packet->senddeltatime);
			//}
			obj.mx = packet->mx;
			obj.my = packet->my;
			//obj.oldDegree = packet->degree;

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