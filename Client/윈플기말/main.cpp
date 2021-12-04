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
#include "OkHUD.h"
#include "DieHUD.h"
#include "Button.h"
#include "Text.h"
#include "Network.h"
#include "../../Protocol/protocol.h"
#include <string>
#include<atlstr.h>

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib, "ws2_32")
//#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif


LPCTSTR lpszClass = L"Just Jump";
LPCTSTR lpszWinodwName = L"Just Jump";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
static PAINTSTRUCT ps;
static HDC mem2dc, loaddc, playerdc, odc, ui_dc, hp_dc, die_dc, start_dc, help_dc, login_dc; // odc = 오브젝트 dc, pdc = player dc,ui_Dc : 아래 전체적인 ui hp_Dc: hp통만 나오는거 dic_dc : 사망 ui 
static HBITMAP loadbit, oldload, oldbit1, hbitobj[100];
static PLAYER player;
PLAYER others[3];

bool keyboard[256];

static MAP map;
//static CAMERA camera;
static OBJECT obj[150];
static BLENDFUNCTION loadbf;
bool isComposit = false;


static int nCaretPosx, nCaretPosy;	//폰트 x,y크기 , 캐럿 x y 위치
static int ocount;		//obj 개수를 세주는 변수
static int help_button = 0, start_button = 0; //조작법 온오프
static bool gamemode = 0;	//0이면 기본 1이면 자유모드
static float deltatime = 0;
static float elapsedtime = 0;
static int Fps = 0;
static DWORD oldtime;



using namespace std;


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

void update(float delta_time)
{
	
	player_keyProcess();
	robby_waiting();

	//빼줘야 할 Ui가 있다면 Ui 삭제
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
	//UiClear 끝

	//Sound업데이트
	FMOD_System_Update(Sound::GetSelf()->System);
	//Sound업데이트 끝
	if (map.getmapnum() == LOGINBG)
		return;
	obj_t += 1;
	if (map.getmapnum() != LOGINBG)	//로그인중일땐 캐릭터 상호작용 x 
	{
		//애니메이션=============================
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
		if (player.state == 5) {
			player.bx = 0;
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
			if (o.state == 5) {
				o.bx = 0;
			}
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
		//캐릭터가 로딩중일땐 카메라 이동 금지 , 일반모드일때만 카메라 움직임
		if (player.getGamemode() == 0)
			adjustCamera(camera, player);
	}

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
		}
		if (obj[i].getType() == 106 || obj[i].getType() == 107)
		{
			if (obj_t % 5 == 0)
			{
				obj[i].IndexChange();

			}
			
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
		
	
	if (map.getmapnum() == 13) {
		for (const auto& ui : Network::GetNetwork()->mUI)
			ui->drawExit(mem1dc);
	}
	

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
			cout << "FPS:" << Fps << endl;
			Fps = 0;
			elapsedtime = 0;

		}
		HideCaret(hWnd);
		update(deltatime);
		render();
		ShowCaret(hWnd);
	}
}

HIMC m_hIMC = NULL;   // IME 핸들
wchar_t wszComp[256] = { 0, };
wchar_t wsz1Comp[256] = { 0, };

int GetText(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (map.getmapnum() != LOGINBG)
		return 1;
	int len;

	switch (msg)
	{
	case WM_IME_COMPOSITION:
		m_hIMC = ImmGetContext(hWnd);	// ime핸들을 얻는것
		if (lparam & GCS_RESULTSTR)
		{
			if ((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0)) > 0)
			{
				// 완성된 글자가 있다.
				ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, wszComp, len);

				if (map.LoginInputFlag == false)
				{
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getTextLen() < 9)
					{
						if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getText().size() == 0) 
							Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->pushwChar(*wszComp);
						else
							Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->changewChar(*wszComp);
						isComposit = false;
					}
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hWnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
					nCaretPosy = 330;
				}
				else {
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getTextLen() < 16)
					{
						if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getText().size() == 0)
							Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->pushwChar(*wszComp);
						else
							Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->changewChar(*wszComp);
						isComposit = false;
					}
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->UpdateFontSize(hWnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getFontLen().cx;
					nCaretPosy = 380;
				}
			}

		}
		else if (lparam & GCS_COMPSTR)
		{
			// 현재 글자를 조합 중이다.

			// 조합중인 길이를 얻는다.
			// str에  조합중인 문자를 얻는다.
			len = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);
			ImmGetCompositionString(m_hIMC, GCS_COMPSTR, wsz1Comp, len);
			wsz1Comp[len] = 0;
			if (map.LoginInputFlag == false)
			{
				if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getTextLen() < 9)
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
				nCaretPosy = 330;
			}
			else {
				if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getTextLen() < 15)
				{
					if (!isComposit)
					{
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->pushwChar(NULL);
						isComposit = true;
					}
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->changewChar(*wsz1Comp);
				}
				Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->UpdateFontSize(hWnd);
				nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getFontLen().cx;
				nCaretPosy = 380;
			}


		}

		ImmReleaseContext(hWnd, m_hIMC);	// IME 핸들 반환!!
		return 0;


	case WM_CHAR:				// 1byte 문자 (ex : 영어)
		return 1;
	case WM_IME_NOTIFY:			// 한자입력...
		return 0;
	case WM_KEYDOWN:			// 키다운..
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
			//cout << "메세지콜" << endl;
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
	packet.vk_key = dr;

	Network::GetNetwork()->C_Send(&packet, sizeof(packet));
}

void send_keyup_packet(char vk_key)
{
	cs_packet_keyup packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_KEYUP;
	packet.vk_key = vk_key;
	Network::GetNetwork()->C_Send(&packet, sizeof(packet));
}
//void send_robby_in_packet()
//{
//	cs_packet_robby packet;
//	packet.size = sizeof(packet);
//	packet.type = CS_PACKET_ROBBY;
//	
//
//	Network::GetNetwork()->C_Send(&packet, sizeof(packet));
//}
void send_die_ok_packet()
{
	cs_packet_die_ok packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_DIEOK;


	Network::GetNetwork()->C_Send(&packet, sizeof(packet));
}

void robby_waiting()
{
	
	if (Network::GetNetwork()->countdown <= 10 && Network::GetNetwork()->cntdown_controller == true) {
		map.mStartui->FindTextByNameTag("countdown")->pushString(to_wstring(Network::GetNetwork()->countdown));
		Network::GetNetwork()->cntdown_controller = false;
	}


}



void player_keyProcess()
{
	bool is_Pressed_Once = false;
	if (player.hp != 0)
	{
		if (IsKeyPressed(VK_LEFT))
		{
			is_Pressed_Once = true;
			send_move_packet(VK_LEFT);
		}
		if (IsKeyPressed(VK_RIGHT))
		{
			is_Pressed_Once = true;
			send_move_packet(VK_RIGHT);
		}
		if (IsKeyPressed(VK_UP))
		{
			is_Pressed_Once = true;
			send_move_packet(VK_UP);
		}
		if (IsKeyPressed(VK_DOWN))
		{
			is_Pressed_Once = true;
			send_move_packet(VK_DOWN);
		}
		if (IsKeyPressed(VK_SPACE))
		{
			is_Pressed_Once = true;
			send_move_packet(VK_SPACE);
		}
		if (IsKeyPressed(VK_END))
		{
			is_Pressed_Once = true;
			send_move_packet(VK_END);
		}
	}
	if (is_Pressed_Once == false)
	{
		send_move_packet(0);
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

		auto ui = make_shared<LoginHUD>(1,&map);
		ui->LoadUiBitmap(g_hinst, "img/idpassword.bmp", 340, 250, 332, 282, RGB(255, 0, 0));
		ui->addText("kk", "id", L"메이플스토리 bold", RGB(255, 108, 168), 18, 380, 330, false, 0, 0, camera);
		ui->addText("127.0.0.1", "ip", L"메이플스토리 bold", RGB(255, 108, 168), 18, 380, 380, false, 0, 0, camera);
		ui->addButton([hwnd, ui]() {
			//
			if (CONN_FAIL == Network::GetNetwork()->ConnectServer(ui->FindTextByNameTag("ip")->getTextForString().c_str()))
			{
				HideCaret(hwnd);
				auto okui = make_shared<OkHUD>(1);
				okui->LoadUiBitmap(g_hinst, "img/NoticeError.bmp", 375, 300, 249, 142, RGB(255, 0, 0));
				okui->addButton([hwnd, okui]() {
					okui->closeUI();
					ShowCaret(hwnd);
				}, g_hinst, "img/LoginOkButton", 475, 400, 50, 23, RGB(255, 0, 0));
				Network::GetNetwork()->mUI.emplace_back(okui);
				return;
			}

			player.mPlayername = ui->FindTextByNameTag("id")->getTextForString();
			player.mPlayerwname = ui->FindTextByNameTag("id")->getText();

			HANDLE hThread = CreateThread(NULL, 0, ClientRecvThread, (LPVOID)0, 0, NULL);
			if (hThread == NULL)
			{
				cerr << "비 정상적인 스레드 생성" << endl;
				exit(-1);
			}

			cs_packet_login packet;
			strcpy_s(packet.username, 20, ui->FindTextByNameTag("id")->getTextForString().c_str());
			packet.size = sizeof(cs_packet_login);
			packet.type = CS_PACKET_LOGIN;
			strcpy_s(packet.username, 20, player.mPlayername.c_str());
			Network::GetNetwork()->C_Send(&packet, sizeof(packet));
			//로비 카운트 start===================================================
			//send_robby_in_packet();

			//====================================================================

			ui->closeUI();
			Network::GetNetwork()->mUI.emplace_back(map.mStartui);

			//gameui는 로그인을 했을때 UserID가 필요하므로 로그인 버튼이 눌렸을 때 처리한다.
			//나중 로그인패킷까지 온다고 가정했을때, 로그인패킷 ok시에 ui를 만들어도 좋다.
			auto gameui = make_shared<GameHUD>(1, player,others);
			gameui->LoadUiBitmap(g_hinst, "img/NoNameUi.bmp", 400, 700, 199, 65, RGB(0, 255, 0), camera);
			gameui->addText(player.mPlayerwname, "NickName", L"메이플스토리 bold", RGB(255, 255, 255), 14, 475, 705, true, 100, 65, camera);
			gameui->LoadHpUiBitmap(g_hinst, "img/Ui_HP.bmp", 421, 728, 100, 65, RGB(0, 0, 255), camera);
			map.mGameUi = gameui;
			//gameUi설정 끝 

			

			HideCaret(hwnd);
		}, g_hinst, "img/LoginButton", 365, 440, 278, 53, RGB(255, 0, 0));
		
		auto startui = make_shared<StartHUD>(0);

		//+
		startui->addText(" ", "countdown", L"메이플스토리 bold", RGB(255, 255, 255), 18, 360, 200, false, 0, 0, camera);
		startui->addText("Ready", "ready", L"메이플스토리 bold", RGB(255, 255, 0), 18, 300, 200, false, 0, 0, camera);
		startui->addText("", "5252", L"메이플스토리 bold", RGB(255, 0, 0), 18, 120, 250, false, 0, 0, camera);


		startui->addButton([startui]() {
			
			//5252~3p game
			startui->FindTextByNameTag("5252")->setText(L"5252~3p game");

		}, g_hinst, "img/start", 292, 490, 138, 82, RGB(255, 0, 0));
		startui->addButton([]() {}, g_hinst, "img/help", 215, 300, 400, 200, RGB(60, 60, 60));
		map.mStartui = startui;




		auto dieui = make_shared<DieHUD>(1, player, camera);

		dieui->LoadUiBitmap(g_hinst, "img/DieNotice.bmp", 380, 240, 260, 130, RGB(255, 0, 0));
		
		dieui->addButton([dieui]() {
			cout << "위치 초기화!" << endl;
			send_die_ok_packet();
			dieui->closeUI();
		}, g_hinst, "img/DieOkButton", 583, 340, 40, 16, RGB(255, 0, 0));
		map.mDieUi = dieui;

		Network::GetNetwork()->mUI.emplace_back(ui)->FindTextByNameTag("id");
		player.setBit(g_hinst);
		for (auto& other : others)
			other.setBit(g_hinst);
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
		}	
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		break;
	case WM_KEYUP:
		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0) {
			keyboard[wParam] = false;
			if (player.hp != 0)
			{
				send_keyup_packet(wParam);

			}
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
		break;
	case WM_LBUTTONUP:		
		Network::GetNetwork()->mUI.back()->processInput(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONUP);		
		break;
	case WM_CHAR:
		if (map.getmapnum() == LOGINBG)
		{
			//wParam 0x08 - 백스페이스 
			//0x09 - 탭 , 0x0A - Line Feed , 0x0D - 엔터, 0x1B - esc 이거빼곤 나머지 다 입력가능한 것. 나중에 채팅창 쓸때 사용하도록 
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
					nCaretPosy = 330;
				}
				else
				{
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getTextLen() > 0)
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->popChar();
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getFontLen().cx;
					nCaretPosy = 380;
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
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getTextLen() < 9)
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->pushChar(wParam);
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
					nCaretPosy = 330;
				}
				else {
					if (Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getTextLen() < 15)
						Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->pushChar(wParam);
					Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getFontLen().cx;
					nCaretPosy = 380;
				}
				isComposit = false;
				break;
			}
			SetCaretPos(nCaretPosx, nCaretPosy);
			ShowCaret(hwnd);
			break;
		}
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