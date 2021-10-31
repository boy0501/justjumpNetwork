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
#ifdef _DEBUG
#pragma comment(lib,"Winmm.lib")
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


HINSTANCE g_hinst;
LPCTSTR lpszClass = L"Just Jump";
LPCTSTR lpszWinodwName = L"Just Jump";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
static PAINTSTRUCT ps;
static HDC hdc, mem1dc, mem2dc, loaddc, playerdc, odc, pdc, ui_dc, hp_dc, die_dc, start_dc, help_dc, login_dc; // odc = 오브젝트 dc, pdc = player dc,ui_Dc : 아래 전체적인 ui hp_Dc: hp통만 나오는거 dic_dc : 사망 ui 
static RECT rectview;
static HBITMAP hbit1, loadbit, oldload, oldbit1, hbitobj[100];
static PLAYER player;
static MAP map;
static CAMERA camera;
static OBJECT obj[150];
static BLENDFUNCTION loadbf;

HWND hWnd;
static int nCaretPosx, nCaretPosy;	//폰트 x,y크기 , 캐럿 x y 위치
static int obj_t = 0; //오브젝트 애니메이션을 1번타이머에 넣기위해 추가한 변수
static int ocount;		//obj 개수를 세주는 변수
static int help_button = 0, start_button = 0; //조작법 온오프
static bool occur_button = 0;	//사망했을때의 button이 활성화되었는지 
static bool gamemode = 0;	//0이면 기본 1이면 자유모드
static float deltatime = 0;
static float elapsedtime = 0;
static int Fps = 0;
static DWORD oldtime;

static vector<shared_ptr<UI>> mUI;
//extern int ROWSPEED;

//extern int COLSPEED;

//한줄에 79자까지 입력가능한 메모장
using namespace std;
void update(float delta_time)
{
	//빼줘야 할 Ui가 있다면 Ui 삭제
	auto iter = mUI.begin();
	while (iter != mUI.end())
	{
		if ((*iter)->getState() == UI::UIState::Dead)
		{
			iter = mUI.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	//UiClear 끝
	
	if (map.getmapnum() == LOGINBG)
		return;
	obj_t += 1;

	if (map.getmapnum() != LOGINBG)	//로그인중일땐 캐릭터 상호작용 x 
	{
		player.move(obj_t);
		adjustPlayer(player, obj, map, ocount, g_hinst);
		if (player.getCMD_die())
			mUI.emplace_back(map.mDieUi);
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

	player.selectBit();
	player.stealthtime();
	player.spike_hurttime();

	// 이거를 따로 넣는게 낳을듯 오브젝트 멤버함수로다가
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
			if (obj_t % 30 == 0)
			{
				obj[i].IndexChange();

			}

		}
		if (obj[i].getType() == 106 || obj[i].getType() == 107)
		{
			if (obj_t % 5 == 0)
			{
				obj[i].IndexChange();

			}
			obj[i].move();
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
	for (const auto& ui : mUI)
		ui->draw(mem1dc);

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	
	switch (iMessage)
	{
	case WM_CREATE: {
		AddFontResourceA("font/Maplestory Bold.ttf");
		AddFontResourceA("font/Maplestory Light.ttf");
		GetClientRect(hwnd, &rectview);
		oldtime = timeGetTime();
		map.CreateMap(g_hinst);
		auto ui = make_shared<LoginHUD>(1);
		ui->LoadUiBitmap(g_hinst, "img/idpassword.bmp", 340, 250, 332, 282, RGB(255, 0, 0));
		ui->addText("kk", "id", L"메이플스토리 bold", RGB(255, 108, 168), 18, 380, 330,false,0,0,camera);
		ui->addText("", "pass", L"메이플스토리 bold", RGB(255, 108, 168), 18, 380, 380,false,0,0,camera);
		ui->addButton([hwnd,ui]() {
			map.setmapnum(9);
			ocount = initObject(obj, map.getmapnum(), g_hinst);
			map.CreateMap(g_hinst);
			LoadBK(hbit1, g_hinst, 9);
			camera.setx(0);
			camera.sety(0);
			player.setx(80);
			player.sety(655);
			player.mPlayername = ui->FindTextByNameTag("id")->getTextForString();
			Sound::GetSelf()->Sound_Play(BGMSOUND, MAINMENUBGM, BGMVOL);
			ui->closeUI();
			mUI.emplace_back(map.mStartui);

			//gameui는 로그인을 했을때 UserID가 필요하므로 로그인 버튼이 눌렸을 때 처리한다.
			//나중 로그인패킷까지 온다고 가정했을때, 로그인패킷 ok시에 ui를 만들어도 좋다.
			auto gameui = make_shared<GameHUD>(1,player);
			gameui->LoadUiBitmap(g_hinst, "img/NoNameUi.bmp", 400, 700, 199, 65, RGB(0, 255, 0), camera);
			gameui->addText(player.mPlayername, "NickName", L"메이플스토리 light", RGB(255, 255, 255), 14, 475, 705, true, 100, 65, camera);
			gameui->LoadHpUiBitmap(g_hinst, "img/Ui_HP.bmp", 421, 728, 100, 65, RGB(0, 0, 255), camera);
			map.mGameUi = gameui;
			//gameUi설정 끝 

			HideCaret(hwnd);
		}, g_hinst, "img/LoginButton", 365, 440, 278, 53, RGB(255, 0, 0));
		auto startui = make_shared<StartHUD>(0);
		//hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/NoNameUi.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
		startui->addButton([startui]() {
			occur_button = 0;
			map.setblack_t(50);
			map.setmapnum(map.getmapnum() + 1);
			for (int j = 0; j < ocount; j++)
				obj[j].ResetObject();
			ocount = initObject(obj, map.getmapnum(), g_hinst);

			map.CreateMap(g_hinst);
			LoadBK(hbit1, g_hinst, 0);
			//hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			Sound::GetSelf()->setindex(Sound::GetSelf()->getindex() + 1);
			Sound::GetSelf()->Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
			Sound::GetSelf()->Sound_Play(BGMSOUND, FIRSTMAPBGM, BGMVOL);
			player.initPos();
			player.sethp(5);
			camera.setx(0);
			camera.sety(3232); 
			startui->closeUI();
			mUI.emplace_back(map.mGameUi);
		}, g_hinst, "img/start", 292, 490, 138, 82, RGB(255, 0, 0));
		startui->addButton([startui]() {}, g_hinst, "img/help", 215, 300, 400, 200, RGB(60, 60, 60));
		map.mStartui = startui;
		
		auto dieui = make_shared<DieHUD>(1,player,camera);
		
		dieui->addButton([dieui]() {
			player.initPos();
			player.sethp(100);
			dieui->closeUI();
		}, g_hinst, "img/notice", 380, 240, 260, 130, RGB(255, 0, 0));
		map.mDieUi = dieui;

		mUI.emplace_back(ui);
		player.setBit(g_hinst);
		player.initBitPos();
		mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
		nCaretPosx = 380 + mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
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


		break;
	}
	case WM_KEYDOWN:
		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0)
			player.PlayerSetting(wParam);
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		break;
	case WM_KEYUP:
		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0)
			player.PlayerWaiting(wParam);
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		break;
	case WM_MOUSEMOVE:
		mUI.back()->processInput(LOWORD(lParam), HIWORD(lParam), WM_MOUSEMOVE);
		break;
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursorFromFile(TEXT("cursor/cursor4.cur")));
		mUI.back()->processInput(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONDOWN);
		
		if (map.getmapnum() == LOGINBG)
		{
			if (LOWORD(lParam) > 365 && LOWORD(lParam) < 565)
			{
				if (HIWORD(lParam) > 320 && HIWORD(lParam) < 360)
				{
					map.LoginInputFlag = false;
					mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
					nCaretPosy = 330;
					SetCaretPos(nCaretPosx, nCaretPosy);
				}
				if (HIWORD(lParam) > 360 && HIWORD(lParam) < 400)
				{

					map.LoginInputFlag = true;
					mUI.back()->FindTextByNameTag("pass")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + mUI.back()->FindTextByNameTag("pass")->getFontLen().cx;
					nCaretPosy = 380;
					SetCaretPos(nCaretPosx, nCaretPosy);
				}
			}

			break;
		}
		
		std::cout << LOWORD(lParam) << endl;
		std::cout << HIWORD(lParam) + camera.gety() << endl;
		break;
	case WM_LBUTTONUP:
		
		mUI.back()->processInput(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONUP);
		
		break;
	case WM_CHAR:
		if (map.getmapnum() == LOGINBG)
		{
			//굉~장히 map 안의 내부 함수로 넘겨서 키보드입력처리 따로 해주고싶은데,,, 나중에 구조를 따로 옮기기 위해 일단 빼둠
			//wParam 0x08 - 백스페이스 
			//0x09 - 탭 , 0x0A - Line Feed , 0x0D - 엔터, 0x1B - esc 이거빼곤 나머지 다 입력가능한 것. 나중에 채팅창 쓸때 사용하도록 
			HideCaret(hwnd);
			if (wParam == 0x08)
			{
				if (map.LoginInputFlag == false)
				{
					if (mUI.back()->FindTextByNameTag("id")->getTextLen() > 0)
						mUI.back()->FindTextByNameTag("id")->popChar();
					mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
				}
				else
				{
					if (mUI.back()->FindTextByNameTag("pass")->getTextLen() > 0)
						mUI.back()->FindTextByNameTag("pass")->popChar();
					mUI.back()->FindTextByNameTag("pass")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + mUI.back()->FindTextByNameTag("pass")->getFontLen().cx;
				}
			}
			else if ((wParam >= 'a' && wParam <= 'z') || (wParam >= '0' && wParam <= '9'))
			{
				if (map.LoginInputFlag == false)
				{

					if(mUI.back()->FindTextByNameTag("id")->getTextLen() < 10)
						mUI.back()->FindTextByNameTag("id")->pushChar(wParam);
					mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
				}
				else {
					if (mUI.back()->FindTextByNameTag("pass")->getTextLen() < 10)
						mUI.back()->FindTextByNameTag("pass")->pushChar(wParam);
					mUI.back()->FindTextByNameTag("pass")->UpdateFontSize(hwnd);
					nCaretPosx = 380 + mUI.back()->FindTextByNameTag("pass")->getFontLen().cx;
				}
			}
			cout << map.mFontSize.cx << endl;
			SetCaretPos(nCaretPosx, nCaretPosy);
			ShowCaret(hwnd);
			break;
		}
		if (wParam == 'r')
		{
			player.setx(obj[ocount - 1].getX() + 10);
			player.sety(obj[ocount - 1].getY() - 25);
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