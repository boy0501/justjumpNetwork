#include <windows.h>
#include <tchar.h>
#include <random>
#include <chrono>
#include <iostream>
#include <timeapi.h>
#include "resource.h"
#include "Load.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Sound.h"
#include "UI.h"
#include "LoginHUD.h"
#include "GameHUD.h"
#include "StartHUD.h"
#include "DieHUD.h"
#include "Button.h"
#include "Text.h"
#include "Network.h"
#include "../../Protocol/protocol.h"
#include <string>

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib, "ws2_32")
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


LPCTSTR lpszClass = L"Just Jump";
LPCTSTR lpszWinodwName = L"Just Jump";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
static PAINTSTRUCT ps;
static HDC /*hdc, mem1dc,*/ mem2dc, loaddc, playerdc, odc, /*pdc,*/ ui_dc, hp_dc, die_dc, start_dc, help_dc, login_dc; // odc = ������Ʈ dc, pdc = player dc,ui_Dc : �Ʒ� ��ü���� ui hp_Dc: hp�븸 �����°� dic_dc : ��� ui 
/*static RECT rectview;*/
static HBITMAP /*hbit1,*/ loadbit, oldload, oldbit1, hbitobj[100];
static PLAYER player;
PLAYER others[3];

bool keyboard[256];

static MAP map;
//static CAMERA camera;
static OBJECT obj[150];
static BLENDFUNCTION loadbf;
bool isComposit = false;

/*HWND hWnd;*/
static int nCaretPosx, nCaretPosy;	//��Ʈ x,yũ�� , ĳ�� x y ��ġ
/*static int obj_t = 0;*/ //������Ʈ �ִϸ��̼��� 1��Ÿ�̸ӿ� �ֱ����� �߰��� ����
static int ocount;		//obj ������ ���ִ� ����
static int help_button = 0, start_button = 0; //���۹� �¿���
//static bool occur_button = 0;	//����������� button�� Ȱ��ȭ�Ǿ����� 
static bool gamemode = 0;	//0�̸� �⺻ 1�̸� �������
static float deltatime = 0;
static float elapsedtime = 0;
static int Fps = 0;
static DWORD oldtime;

//static vector<shared_ptr<UI>> mUI;
//extern int ROWSPEED;

//extern int COLSPEED;

using namespace std;
//float elapsedt;
//int Nameunsigan= 10;


DWORD WINAPI ClientRecvThread(LPVOID arg)
{
	while (1)
	{
		Network::GetNetwork()->C_Recv();
	}
}
bool IsKeyPressed(char key)
{
	return keyboard[key];
}
void robby_waiting();
void player_keyProcess();
bool bRobby_full = false;

void update(float delta_time)
{

	player_keyProcess();
	robby_waiting();

	//Network::GetNetwork()->C_Recv();

	//����� �� Ui�� �ִٸ� Ui ����
	auto iter = Network::GetNetwork()->mUI.begin();
	while (iter != Network::GetNetwork()->mUI.end())
	{
		if ((*iter)->getState() == UI::UIState::Dead)
		{
			iter = Network::GetNetwork()->mUI.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	//UiClear ��

	//Sound������Ʈ
	FMOD_System_Update(Sound::GetSelf()->System);
	//Sound������Ʈ ��
	if (map.getmapnum() == LOGINBG)
		return;
	obj_t += 1;
	if (map.getmapnum() != LOGINBG)	//�α������϶� ĳ���� ��ȣ�ۿ� x 
	{
		//�ִϸ��̼�=============================
		if (player.state == 4) {
			if (obj_t % 5 == 0)
			{
				player.BitMove();
			}
		}
		if (player.state == 8) {
			if (obj_t % 10 == 0)
				player.BitMove();
		}

		for (auto& o : others) {
			if (o.state == 4) {
				if (obj_t % 5 == 0)
				{
					o.BitMove();
				}
			}
			if (o.state == 8) {
				if (obj_t % 10 == 0)
					o.BitMove();
			}
		}
		//=======================================
		
		player.move(delta_time);
		adjustPlayer(player, obj, map, ocount, g_hinst);
		//cout << player.y << endl;
		for (auto& other : others)
		{
			other.move(delta_time);
			//adjustPlayer(other, obj, map, ocount, g_hinst);
		}
		//�ΰ� �� ������ �Ű���� ������, ���� �ʿ䰡 ����.
		if (player.getCMD_die())
		{
			if(player.WhenPlayerDied==false)
				Network::GetNetwork()->mUI.emplace_back(map.mDieUi);
			player.WhenPlayerDied = true;
		}
	}
	map.movemap();

	if (map.BlackTime())
	{
		if (loadbf.SourceConstantAlpha + 40 > 255)
			loadbf.SourceConstantAlpha = 255;
		else {
			loadbf.SourceConstantAlpha += 40;
		}
	}
	else {
		//ĳ���Ͱ� �ε����϶� ī�޶� �̵� ���� , �Ϲݸ���϶��� ī�޶� ������
		if (player.getGamemode() == 0)
			adjustCamera(camera, player);
	}

	player.selectBit();
	player.stealthtime();
	player.spike_hurttime();
	for (auto& o : others)
		o.selectBit();

	// �̰Ÿ� ���� �ִ°� ������ ������Ʈ ����Լ��δٰ�
	for (int i = 0; i <= ocount; i++)
	{
		if (obj[i].getType() == 0)
		{
			if (obj_t % 10 == 0)
			{
				obj[i].IndexChange();

			}

		}
		if (obj[i].getType() == 4)
		{
			if (obj_t % 8 == 0)
			{
				obj[i].IndexChange();

			}

		}
		if (obj[i].getType() == 6)
		{
			if (obj_t % 8 == 0)
			{
				obj[i].IndexChange();

			}

		}
		if (obj[i].getType() == 103)
		{
			//if (obj_t % 30 == 0)
			//{
			//	obj[i].IndexChange();
			//
			//}

		}
		if (obj[i].getType() == 106 || obj[i].getType() == 107)
		{
			if (obj_t % 5 == 0)
			{
				obj[i].IndexChange();

			}
			obj[i].move(delta_time);
		}
		else if (obj[i].getType() == 201)
		{
			if (obj_t % 20 == 0)
			{
				obj[i].IndexChange();

			}
		}
	}
	if (obj_t >= 27000) obj_t = 0;

	//�ٲ� ��ŷ�� �� �Ѿ������ Ȯ��---
	//cout << player.rank << endl;
	//----------------------------
}
void render()
{
	hdc = GetDC(hWnd);

	mem1dc = CreateCompatibleDC(hdc);
	if (hbit1 == NULL)
	{
		hbit1 = CreateCompatibleBitmap(hdc, rectview.right, rectview.bottom);
	}
	
	
	SelectObject(mem1dc, hbit1);
	
	FillRect(mem1dc, &rectview, (HBRUSH)COLOR_BACKGROUND);
	
	if (0 >= map.getblack_t())
	{
		map.DrawBK(mem1dc, mem2dc, rectview);
	
	}	
	for (int i = 0; i <= ocount; i++)
		obj[i].DrawObj(mem1dc, odc);
	player.draw(mem1dc, pdc);
	for (auto& other : others)
		other.draw(mem1dc, pdc);
	for (const auto& ui : Network::GetNetwork()->mUI)
		ui->draw(mem1dc);
	//������ ������------
	if (map.getmapnum() == 13) {
		for (const auto& ui : Network::GetNetwork()->mUI)
			ui->drawExit(mem1dc);
	}
	//-------------------

	if (map.getblack_t() > 0) map.DrawLoadBK(mem1dc, mem2dc, loadbf);


	BitBlt(hdc, 0, 0, 1024, 768, mem1dc, camera.getx(), camera.gety(), SRCCOPY);



	DeleteObject(mem1dc);
	ReleaseDC(hWnd, hdc);
}
void ProcessingLoop()
{

	auto newtime = timeGetTime();
	deltatime = (newtime - oldtime) / 1000.f;
	if (deltatime >= 0.016f)
	{
		oldtime = newtime;
		elapsedtime += deltatime;
		Fps++;
		if (elapsedtime > 1.0f)
		{
			//cout << "FPS:" << Fps << endl;
			Fps = 0;
			elapsedtime = 0;
		}
		HideCaret(hWnd);
		update(deltatime);
		render();
		ShowCaret(hWnd);
	}
}

HIMC m_hIMC = NULL;   // IME �ڵ�
wchar_t wszComp[256] = { 0, };
wchar_t wsz1Comp[256] = { 0, };

int GetText(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int len;
	switch (msg)
	{
	case WM_IME_COMPOSITION:
		m_hIMC = ImmGetContext(hWnd);	// ime�ڵ��� ��°�
		if (lparam & GCS_RESULTSTR)
		{
			if ((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0)) > 0)
			{
				// �ϼ��� ���ڰ� �ִ�.
				ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, wszComp, len);

				if (map.LoginInputFlag == false)
				{
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getTextLen() < 10)
					{
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->changewChar(*wszComp);
						isComposit = false;
					}
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hWnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
				}
				else {
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getTextLen() < 10)
					{
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->changewChar(*wszComp);
						isComposit = false;
					}
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->UpdateFontSize(hWnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getFontLen().cx;
				}
			}

		}
		else if (lparam & GCS_COMPSTR)
		{
			// ���� ���ڸ� ���� ���̴�.

			// �������� ���̸� ��´�.
			// str��  �������� ���ڸ� ��´�.
			len = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);
			ImmGetCompositionString(m_hIMC, GCS_COMPSTR, wsz1Comp, len);
			wsz1Comp[len] = 0;
			if (map.LoginInputFlag == false)
			{
				if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getTextLen() < 10)
				{
					if (!isComposit)
					{
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->pushwChar(NULL);
						isComposit = true;
					}
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->changewChar(*wsz1Comp);
				}
				Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hWnd);
				nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
			}
			else {
				if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getTextLen() < 10)
				{
					if (!isComposit)
					{
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->pushwChar(NULL);
						isComposit = true;
					}
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->changewChar(*wsz1Comp);
				}
				Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->UpdateFontSize(hWnd);
				nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getFontLen().cx;
			}

			
		}

		ImmReleaseContext(hWnd, m_hIMC);	// IME �ڵ� ��ȯ!!
		return 0;


	case WM_CHAR:				// 1byte ���� (ex : ����)
		return 1;
	case WM_IME_NOTIFY:			// �����Է�...
		return 0;
	case WM_KEYDOWN:			// Ű�ٿ�..
		return 1;
	}
	return 1;
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	wcout.imbue(locale("korean"));
	MSG Message;
	WNDCLASSEX WndClass;
	g_hinst = hinstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hinstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursorFromFile(TEXT("cursor/cursor2.cur"));
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);


	hWnd = CreateWindow
	(
		lpszClass, lpszWinodwName,
		WS_OVERLAPPEDWINDOW,
		100, 50, 1040, 807,
		NULL, (HMENU)NULL,
		hinstance, NULL
	);


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	memset(&Message, 0, sizeof(Message));
	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			//cout << "�޼�����" << endl;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			ProcessingLoop();
		}
	}

	return Message.wParam;


}

void send_move_packet(char dr)
{
	cs_packet_move packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MOVE;
	packet.dir = dr;

	Network::GetNetwork()->C_Send(&packet, sizeof(packet));
	//cout << "send��Ŷ ����" << endl;
}

void send_keyup_packet(char vk_key)
{
	cs_packet_keyup packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_KEYUP;
	packet.vk_key = vk_key;
	Network::GetNetwork()->C_Send(&packet, sizeof(packet));
}
void send_robby_in_packet()
{
	cs_packet_robby packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_ROBBY;
	

	Network::GetNetwork()->C_Send(&packet, sizeof(packet));
}
//void send_logout_packet(char button)
//{
//	cs_packet_logout packet;
//	packet.size = sizeof(packet);
//	packet.type = CS_PACEKT_LOGOUT;
//	packet.out = button;
//
//	Network::GetNetwork()->C_Send(&packet, sizeof(packet));
//}

static bool only_once = false;
void robby_waiting()
{
	
	if (only_once == false) {
		map.mStartui->addText("Ready", "countdown", L"메이플스토리 bold", RGB(255, 255, 0), 18, 300, 200, false, 0, 0, camera);
		only_once = true;
	}
	if (Network::GetNetwork()->countdown <= 10) {
		map.mStartui->addText(to_string(Network::GetNetwork()->countdown), "countdown", L"�����ý��丮 bold", RGB(255, 255, 255), 18, Network::GetNetwork()->init_x, 200, false, 0, 0, camera);
	}
	
	//cout << Network::GetNetwork()->countdown << endl;

	/*if (Network::GetNetwork()->countdown == 1)
	{
		bRobby_full = true;
	}*/
}



void player_keyProcess()
{
	if (IsKeyPressed(VK_LEFT))
	{
		send_move_packet(VK_LEFT);
	}
	if (IsKeyPressed(VK_RIGHT))
	{
		send_move_packet(VK_RIGHT);
	}
	if (IsKeyPressed(VK_UP))
	{
		send_move_packet(VK_UP);
	}
	if (IsKeyPressed(VK_DOWN))
	{
		send_move_packet(VK_DOWN);
	}
	if (IsKeyPressed(VK_SPACE))
	{
		send_move_packet(VK_SPACE);
	}
	if (IsKeyPressed(VK_END))
	{
		send_move_packet(VK_END);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (GetText(hwnd, iMessage, wParam, lParam) == 0)
	{
		return 0;
	}

	switch (iMessage)
	{
	case WM_CREATE: {
		AddFontResourceA("font/Maplestory Bold.ttf");
		AddFontResourceA("font/Maplestory Light.ttf");
		GetClientRect(hwnd, &rectview);
		oldtime = timeGetTime();
		map.CreateMap(g_hinst);

		player.is_active = true;
		Network::GetNetwork()->mPlayer = &player;
		Network::GetNetwork()->mMap = &map;
		Network::GetNetwork()->mOcount = &ocount;
		Network::GetNetwork()->mObj = obj;
		Network::GetNetwork()->mCamera = &camera;
		Network::GetNetwork()->mOthers = others;
		Network::GetNetwork()->ConnectServer("127.0.0.1");

		HANDLE hThread = CreateThread(NULL, 0, ClientRecvThread, (LPVOID)0, 0, NULL);
		if (hThread == NULL)
		{
			cerr << "�� �������� ������ ����" << endl;
			exit(-1);
		}

		auto ui = make_shared<LoginHUD>(1);
		ui->LoadUiBitmap(g_hinst, "img/idpassword.bmp", 340, 250, 332, 282, RGB(255, 0, 0));
		ui->addText("kk", "id", L"�����ý��丮 bold", RGB(255, 108, 168), 18, 380, 330,false,0,0,camera);
		ui->addText("", "pass", L"�����ý��丮 bold", RGB(255, 108, 168), 18, 380, 380,false,0,0,camera);
		ui->addButton([hwnd,ui]() {

			//
			player.mPlayername = ui->FindTextByNameTag("id")->getTextForString();
			player.mPlayerwname = ui->FindTextByNameTag("id")->getText();
			cs_packet_login packet;
			strcpy_s(packet.username, 20, ui->FindTextByNameTag("id")->getTextForString().c_str());
			packet.size = sizeof(cs_packet_login);
			packet.type = CS_PACKET_LOGIN;
			strcpy_s(packet.username, 20, player.mPlayername.c_str());
			Network::GetNetwork()->C_Send(&packet, sizeof(packet));



			//�κ� ī��Ʈ start===================================================
			send_robby_in_packet();

			//====================================================================

			//player.setx(80);
			//player.sety(655);
			ui->closeUI();
			Network::GetNetwork()->mUI.emplace_back(map.mStartui);

			//gameui�� �α����� ������ UserID�� �ʿ��ϹǷ� �α��� ��ư�� ������ �� ó���Ѵ�.
			//���� �α�����Ŷ���� �´ٰ� ����������, �α�����Ŷ ok�ÿ� ui�� ���� ����.
			auto gameui = make_shared<GameHUD>(1,player);
			gameui->LoadUiBitmap(g_hinst, "img/NoNameUi.bmp", 400, 700, 199, 65, RGB(0, 255, 0), camera);
			gameui->addText(player.mPlayerwname, "NickName", L"�����ý��丮 light", RGB(255, 255, 255), 14, 475, 705, true, 100, 65, camera);
			gameui->LoadHpUiBitmap(g_hinst, "img/Ui_HP.bmp", 421, 728, 100, 65, RGB(0, 0, 255), camera);
			map.mGameUi = gameui;
			//gameUi���� �� 

			

			HideCaret(hwnd);
		}, g_hinst, "img/LoginButton", 365, 440, 278, 53, RGB(255, 0, 0));
		
		auto startui = make_shared<StartHUD>(0);
		//hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/NoNameUi.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //����η� ����
		startui->addButton([startui]() {
			//Network::GetNetwork()->test();
			//cout << "����" << endl;

			//bool occur_button = 0;
			//map.setblack_t(50);
			//map.setmapnum(player.stage + 1);
			//for (int j = 0; j < ocount; j++)
			//	obj[j].ResetObject();
			//ocount = initObject(obj, map.getmapnum(), g_hinst);
			//map.CreateMap(g_hinst);
			//LoadBK(hbit1, g_hinst, map.getmapnum());
			//Sound::GetSelf()->setindex(Sound::GetSelf()->getindex() + 1);
			//Sound::GetSelf()->Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
			//Sound::GetSelf()->Sound_Play(BGMSOUND, FIRSTMAPBGM, BGMVOL);
			//player.initPos();
			//player.sethp(5);
			//camera.setx(0);
			//camera.sety(3232);
			//map.mStartui->closeUI();
			//Network::GetNetwork()->mUI.emplace_back(map.mGameUi);
		    //Network::GetNetwork()->mUI.emplace_back(map.mGameUi);
			map.mStartui->addText("5252~ 3p game", "countdown", L"�����ý��丮 bold", RGB(255, 0, 0), 18, 120, 250, false, 0, 0, camera);


		}, g_hinst, "img/start", 292, 490, 138, 82, RGB(255, 0, 0));
		startui->addButton([]() {}, g_hinst, "img/help", 215, 300, 400, 200, RGB(60, 60, 60));
		map.mStartui = startui;

		//startui->addButton([]() {

		//	}, g_hinst, "img/Exit", 800, 400, 138, 82, RGB(255, 0, 0));// g_hinst, "img/help", 215, 300, 400, 200, RGB(60, 60, 60));
		//map.mStartui = startui;


		
		auto dieui = make_shared<DieHUD>(1,player,camera);
		
		dieui->addButton([dieui]() {
			player.initPos();
			player.sethp(100);
			player.WhenPlayerDied = false;
			dieui->closeUI();
		}, g_hinst, "img/notice", 380, 240, 260, 130, RGB(255, 0, 0));
		map.mDieUi = dieui;

		Network::GetNetwork()->mUI.emplace_back(ui);
		player.setBit(g_hinst);
		for (auto& other : others)
			other.setBit(g_hinst);
		//player.initBitPos();
		Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
		nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
		nCaretPosy = 330;
		if (map.getmapnum() == 1)
		{
			camera.setx(0);
			camera.sety(0);
		}


		Sound::GetSelf()->Sound_Setup();
		loadbf.AlphaFormat = 0;
		loadbf.BlendFlags = 0;
		loadbf.BlendOp = AC_SRC_OVER;
		loadbf.SourceConstantAlpha = 0;
		Sound::GetSelf()->Sound_Play(BGMSOUND, LOGINBGM, BGMVOL);


	}
	break;
	case WM_KEYDOWN:
		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0) {
			keyboard[wParam] = true;
			player.PlayerSetting(wParam);
			//send_move_packet(wParam);
		}	
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		break;
	case WM_KEYUP:
		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0) {
			player.PlayerWaiting(wParam);
			keyboard[wParam] = false;
			send_keyup_packet(wParam);
		}
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		break;
	case WM_MOUSEMOVE:
		Network::GetNetwork()->mUI.back()->processInput(LOWORD(lParam), HIWORD(lParam), WM_MOUSEMOVE);
		break;
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursorFromFile(TEXT("cursor/cursor4.cur")));
		Network::GetNetwork()->mUI.back()->processInput(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONDOWN);
		
		if (map.getmapnum() == LOGINBG)
		{
			if (LOWORD(lParam) > 365 && LOWORD(lParam) < 565)
			{
				if (HIWORD(lParam) > 320 && HIWORD(lParam) < 360)
				{
					map.LoginInputFlag = false;
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
					nCaretPosy = 330;
					SetCaretPos(nCaretPosx, nCaretPosy);
				}
				if (HIWORD(lParam) > 360 && HIWORD(lParam) < 400)
				{

					map.LoginInputFlag = true;
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getFontLen().cx;
					nCaretPosy = 380;
					SetCaretPos(nCaretPosx, nCaretPosy);
				}
			}

			break;
		}
		
		//std::cout << LOWORD(lParam) << endl;
		//std::cout << HIWORD(lParam) + camera.gety() << endl;
		break;
	case WM_LBUTTONUP:
		
		Network::GetNetwork()->mUI.back()->processInput(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONUP);
		
		break;
	case WM_CHAR:
	if (map.getmapnum() == LOGINBG)
		{
			//��~���� map ���� ���� �Լ��� �Ѱܼ� Ű�����Է�ó�� ���� ���ְ������,,, ���߿� ������ ���� �ű�� ���� �ϴ� ����
			//wParam 0x08 - �齺���̽� 
			//0x09 - �� , 0x0A - Line Feed , 0x0D - ����, 0x1B - esc �̰Ż��� ������ �� �Է°����� ��. ���߿� ä��â ���� ����ϵ��� 
			HideCaret(hwnd);
			switch (wParam)
			{
			case 0x08:
				if (map.LoginInputFlag == false)
				{
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getTextLen() > 0)
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->popChar();
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
				}
				else
				{
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getTextLen() > 0)
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->popChar();
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getFontLen().cx;
				}
				break;
			case 0x09:
			case 0x0A:
			case 0x0D:
			case 0x1B:
				break;
			default:
				if (map.LoginInputFlag == false)
				{
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getTextLen() < 10)
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->pushChar(wParam);
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
				}
				else {
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getTextLen() < 10)
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->pushChar(wParam);
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("pass")->getFontLen().cx;
				}
				isComposit = false;
				break;
			}
			SetCaretPos(nCaretPosx, nCaretPosy);
			ShowCaret(hwnd);
			break;
		}
		/*if (wParam == 'r')
		{
			cout << "rŰ ����" << endl;
			player.setx(obj[ocount - 1].getX() + 10);
			player.sety(obj[ocount - 1].getY() - 25);
			break;
		}*/
		if (wParam == 'c')
		{
			player.setCMD_move(0);
			if (player.getGamemode() == 0)
				player.setGamemode(1);
			else
				player.setGamemode(0);
			break;
		}
		break;
	case WM_SETFOCUS:
		if (map.getmapnum() == LOGINBG)
		{
			CreateCaret(hwnd, NULL, 2, 20);
			SetCaretPos(nCaretPosx, nCaretPosy);
			ShowCaret(hwnd);
		}
		return 0;
	case WM_KILLFOCUS:
		DestroyCaret();
		return 0;
	case WM_DESTROY:
		//send_logout_packet(WM_DESTROY);

		for (int i = 0; i < 5; ++i)	FMOD_Sound_Release(Sound::GetSelf()->effectSound[i]);
		for (int i = 0; i < 5; ++i)	FMOD_Sound_Release(Sound::GetSelf()->bgmSound[i]);
		FMOD_System_Release(Sound::GetSelf()->System);
		KillTimer(hwnd, 1);
		hbit1, loadbit, hbitobj[99];
		if (hbit1) DeleteObject(hbit1);
		if (loadbit) DeleteObject(loadbit);
		for (int i = 0; i < 100; ++i)
			if (hbitobj[i]) DeleteObject(hbitobj[i]);		
		RemoveFontResourceA("font/Maplestory Bold.ttf");
		RemoveFontResourceA("font/Maplestory Light.ttf");
		delete Sound::GetSelf();
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}