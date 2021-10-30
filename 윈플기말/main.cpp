#include <windows.h>
#include <tchar.h>
#include <random>
#include <iostream>
#include "resource.h"
#include "Load.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Sound.h"
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


HINSTANCE g_hinst;
LPCTSTR lpszClass = L"Just Jump";
LPCTSTR lpszWinodwName = L"Just Jump";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//extern int ROWSPEED;

//extern int COLSPEED;

//한줄에 79자까지 입력가능한 메모장
using namespace std;
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
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


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;


}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static HDC hdc, mem1dc, mem2dc, loaddc, playerdc, odc, pdc, ui_dc, hp_dc,die_dc,start_dc,help_dc,login_dc; // odc = 오브젝트 dc, pdc = player dc,ui_Dc : 아래 전체적인 ui hp_Dc: hp통만 나오는거 dic_dc : 사망 ui 
	static RECT rectview;
	static HBITMAP hbit1,loadbit,oldload, oldbit1, hbitobj[100], Uibit, HPbit,Diebit,Startbit,Helpbit,Loginbit;
	static PLAYER player;
	static MAP map;
	static CAMERA camera;
	static OBJECT obj[150];
	static BLENDFUNCTION loadbf;
	static Sound sound;
	
	static int nCaretPosx, nCaretPosy;	//폰트 x,y크기 , 캐럿 x y 위치
	static int obj_t = 0; //오브젝트 애니메이션을 1번타이머에 넣기위해 추가한 변수
	static int ocount;		//obj 개수를 세주는 변수
	static int help_button = 0,start_button = 0,login_button = 0; //조작법 온오프
	static bool occur_button = 0;	//사망했을때의 button이 활성화되었는지 
	static bool gamemode = 0;	//0이면 기본 1이면 자유모드
	switch (iMessage)
	{
	case WM_CREATE:



		AddFontResourceA("font/Maplestory Bold.ttf");
		AddFontResourceA("font/Maplestory Light.ttf");
		GetClientRect(hwnd, &rectview);
		map.CreateMap(g_hinst);
		map.CreateUi(g_hinst);
		map.CreateHP(g_hinst);
		map.CreateDie(g_hinst);
		map.CreateStart(g_hinst);
		map.CreateHelp(g_hinst);
		map.CreateLogin(g_hinst);
		player.setBit(g_hinst);
		player.initBitPos();
		nCaretPosx = 380 + map.id.length()*10;
		nCaretPosy = 330;
		if (map.getmapnum() == 1)
		{
			camera.setx(0);
			camera.sety(0);
		}


		sound.Sound_Setup();
		loadbf.AlphaFormat = 0;
		loadbf.BlendFlags = 0;
		loadbf.BlendOp = AC_SRC_OVER;
		loadbf.SourceConstantAlpha = 0;
		sound.Sound_Play(BGMSOUND, LOGINBGM, BGMVOL);
		
		//if (map.getmapnum() == 9) hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/start_rayer1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//else if (map.getmapnum() == 13) hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/clear.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		Loginbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/LoginButton2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Helpbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/help1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Startbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/start1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Uibit = (HBITMAP)LoadImage(g_hinst, TEXT("img/NoNameUi.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		HPbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui_HP.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Diebit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Notice3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		//ocount = initObject(obj, 1, g_hinst);


		SetTimer(hwnd, 1, 1, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		if (hbit1 == NULL)
		{
			hbit1 = CreateCompatibleBitmap(hdc, rectview.right, rectview.bottom);
		}


		SelectObject(mem1dc, hbit1);
		SelectObject(ui_dc, Uibit);
		SelectObject(hp_dc, HPbit);
		SelectObject(die_dc, Diebit);
		SelectObject(start_dc, Startbit);
		SelectObject(help_dc, Helpbit);
		SelectObject(login_dc, Loginbit);


		if (0 >= map.getblack_t())
		{
			map.DrawBK(mem1dc, mem2dc, rectview);

		}

		for (int i = 0; i <= ocount; i++)
			obj[i].DrawObj(mem1dc, odc);

		if (map.getmapnum() == LOGINBG)	//로그인화면일땐 캐릭터를 안그려주기위해 if - else절로 묶었음.
		{
			map.DrawLogin(mem1dc, login_dc, login_button);
			
		}
		else {
			if (map.getmapnum() == STARTBG)
			{
				map.DrawStart(mem1dc, start_dc, start_button);
				map.DrawHelp(mem1dc, help_dc, help_button);

			}
			player.draw(mem1dc, pdc);
			if (map.getmapnum() >= 10)
			{
				map.DrawUi(mem1dc, ui_dc, camera);
				map.DrawHP(mem1dc, hp_dc, camera, player);
				if (player.getCMD_die() == 1)
					map.DrawDie(mem1dc, die_dc, camera, sound);
			}

		}

		if (map.getblack_t() > 0) map.DrawLoadBK(mem1dc, mem2dc, loadbf);


		BitBlt(hdc, 0, 0, 1024, 768, mem1dc, camera.getx(), camera.gety(), SRCCOPY);



		DeleteObject(mem1dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (map.getmapnum() == LOGINBG)
				break;
			obj_t += 1;

			if (map.getmapnum() != LOGINBG)	//로그인중일땐 캐릭터 상호작용 x 매끄럽게 하려면 Scene changer를 구현해야하지만,,,, 넘어가도록 하자 
			{
				player.move(obj_t);
				adjustPlayer(player, obj, map, ocount, g_hinst, sound);
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
			InvalidateRgn(hwnd, NULL, FALSE);
			break;

		}
		break;
	case WM_KEYDOWN:

		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0)
			player.PlayerSetting(wParam, sound);
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		InvalidateRgn(hwnd, NULL, FALSE);
		break;
	case WM_KEYUP:
		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0)
			player.PlayerWaiting(wParam);
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		InvalidateRgn(hwnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		if (player.getCMD_die() == 1)
		{
			if (584 < LOWORD(lParam) && LOWORD(lParam) < 620)
			{
				if (338 < HIWORD(lParam) && HIWORD(lParam) < 352)
				{
					map.ChangeDieNotice(g_hinst, 1);
					if (occur_button == 0)
					{
						sound.Sound_Play(EFFECTSOUND, MOVEREF, EFVOL);
						occur_button = 1;
					}
					break;
				}
			}
			map.ChangeDieNotice(g_hinst, 0);
			occur_button = 0;
		}
		if (map.getmapnum() == LOGINBG)
		{
			if (LOWORD(lParam) > 365 && LOWORD(lParam) < 643)
			{
				if (HIWORD(lParam) > 440 && HIWORD(lParam) < 493)
				{
					if (login_button == 0)
					{
						sound.Sound_Play(EFFECTSOUND, MOVEREF, EFVOL);
						login_button = 1;
					}
					break;
				}
			}
			login_button = 0;
		}
		if (map.getmapnum() == STARTBG)
		{
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 430)
			{
				if (490 < HIWORD(lParam) && HIWORD(lParam) < 572)
				{
					//map.ChangeStartButton(g_hinst, 1);
					if (start_button == 0)
					{
						sound.Sound_Play(EFFECTSOUND, MOVEREF, EFVOL);
						start_button = 1;
					}
					break;
				}
			}

			if (290 < LOWORD(lParam) && LOWORD(lParam) < 428)
			{
				if (345 < HIWORD(lParam) && HIWORD(lParam) < 427)
				{

					//map.ChangeHelp(g_hinst, help_button);
					if (help_button == 0)
					{

						sound.Sound_Play(EFFECTSOUND, MOVEREF, EFVOL);
						help_button = 1;
					}
					break;
				}
			}
			//map.ChangeStartButton(g_hinst, 0);
			//map.ChangeHelp(g_hinst, 0);
			start_button = 0;
			help_button = 0;
			occur_button = 0;
		}
		break;
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursorFromFile(TEXT("cursor/cursor4.cur")));

		if (player.getCMD_die() == 1)
		{
			if (584 < LOWORD(lParam) && LOWORD(lParam) < 620)
			{
				if (338 < HIWORD(lParam) && HIWORD(lParam) < 352)
				{
					map.ChangeDieNotice(g_hinst, 2);
					sound.Sound_Play(EFFECTSOUND, MCLICKEF, EFVOL);
					break;
				}
			}
		}
		if (map.getmapnum() == LOGINBG)
		{
			if (LOWORD(lParam) > 365 && LOWORD(lParam) < 565)
			{
				if (HIWORD(lParam) > 320 && HIWORD(lParam) < 360)
				{
					map.LoginInputFlag = false;
					map.UpdateFontSize(hwnd);
					nCaretPosx = 380 + map.mFontSize.cx;
					nCaretPosy = 330;
					SetCaretPos(nCaretPosx, nCaretPosy);
					break;
				}
				if (HIWORD(lParam) > 360 && HIWORD(lParam) < 400)
				{

					map.LoginInputFlag = true;
					map.UpdateFontSize(hwnd);
					nCaretPosx = 380 + map.mFontSize.cx;
					nCaretPosy = 380;
					SetCaretPos(nCaretPosx, nCaretPosy);
					break;
				}
			}
			if (LOWORD(lParam) > 365 && LOWORD(lParam) < 643)
			{
				if (HIWORD(lParam) > 440 && HIWORD(lParam) < 493)
				{
					if (login_button == 1)
					{
						sound.Sound_Play(EFFECTSOUND, MCLICKEF, EFVOL);
						login_button = 2;
						break;
					}
				}
			}
		}
		if (map.getmapnum() == STARTBG)
		{
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 430)
			{
				if (490 < HIWORD(lParam) && HIWORD(lParam) < 572)
				{
					if (start_button == 1)
					{

						//map.ChangeStartButton(g_hinst, 2);
						sound.Sound_Play(EFFECTSOUND, MCLICKEF, EFVOL);
						start_button = 2;

						break;
					}

				}
			}

		}
		std::cout << LOWORD(lParam) << endl;
		std::cout << HIWORD(lParam) + camera.gety() << endl;
		break;
	case WM_LBUTTONUP:
		if (player.getCMD_die() == 1)
		{
			if (584 < LOWORD(lParam) && LOWORD(lParam) < 620)
			{
				if (338 < HIWORD(lParam) && HIWORD(lParam) < 352)
				{
					map.ChangeDieNotice(g_hinst, 0);
					player.initPos();
					player.sethp(100);
					break;
				}
			}
		}
		if (map.getmapnum() == LOGINBG)
		{
			if (LOWORD(lParam) > 365 && LOWORD(lParam) < 643)
			{
				if (HIWORD(lParam) > 440 && HIWORD(lParam) < 493)
				{
					map.setmapnum(9);
					ocount = initObject(obj, map.getmapnum(), g_hinst);
					map.CreateMap(g_hinst);
					LoadBK(hbit1, g_hinst, 9);
					camera.setx(0);
					camera.sety(0);
					player.setx(80);
					player.sety(655);
					player.mPlayername = map.id;
					sound.Sound_Play(BGMSOUND, MAINMENUBGM, BGMVOL);
					HideCaret(hwnd);
					InvalidateRgn(hwnd, NULL, FALSE);
					break;
				}
			}
		}
		if (map.getmapnum() == STARTBG)
		{
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 430)
			{
				if (490 < HIWORD(lParam) && HIWORD(lParam) < 572)
				{
					occur_button = 0;
					map.setblack_t(50);
					map.setmapnum(map.getmapnum() + 1);
					for (int j = 0; j < ocount; j++)
						obj[j].ResetObject();
					ocount = initObject(obj, map.getmapnum(), g_hinst);

					map.CreateMap(g_hinst);
					LoadBK(hbit1, g_hinst, 0);
					//hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
					sound.setindex(sound.getindex() + 1);
					sound.Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
					sound.Sound_Play(BGMSOUND, FIRSTMAPBGM, BGMVOL);
					player.initPos();
					player.sethp(100);
					camera.setx(0);
					camera.sety(3232);
					InvalidateRgn(hwnd, NULL, FALSE);
					break;
				}
			}
			//obj[0].getType()
		}
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
					if (map.id.length() > 0)
					{
						map.id.pop_back();
						cout << map.id << endl;
					}
				}
				else
				{
					if (map.pass.length() > 0)
					{
						map.pass.pop_back();
						cout << map.pass << endl;
					}
				}
			}
			else if ((wParam >= 'a' && wParam <= 'z') || (wParam >= '0' && wParam <= '9'))
			{
				if (map.LoginInputFlag == false)
				{
					if (map.id.length() < 10)
						map.id += wParam;
				}
				else {
					if (map.pass.length() < 10)
						map.pass += wParam;
				}
			}
			map.UpdateFontSize(hwnd);
			nCaretPosx = 380 + map.mFontSize.cx;
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
		InvalidateRect(hwnd, NULL, FALSE);
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
		for (int i = 0; i < 5; ++i)	FMOD_Sound_Release(sound.effectSound[i]);
		for (int i = 0; i < 5; ++i)	FMOD_Sound_Release(sound.bgmSound[i]);
		FMOD_System_Release(sound.System);
		KillTimer(hwnd, 1);
		hbit1, loadbit, hbitobj[100], Uibit, HPbit, Diebit, Startbit, Helpbit;
		if (hbit1) DeleteObject(hbit1);
		if (loadbit) DeleteObject(loadbit);
		if (Uibit) DeleteObject(Uibit);
		if (Diebit) DeleteObject(Diebit);
		if (Startbit) DeleteObject(Startbit);
		if (Helpbit) DeleteObject(Helpbit);
		for (int i = 0; i < 100; ++i)
			if (hbitobj[i]) DeleteObject(hbitobj[i]);		
		RemoveFontResourceA("font/Maplestory Bold.ttf");
		RemoveFontResourceA("font/Maplestory Light.ttf");

		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}