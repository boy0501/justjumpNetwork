#include "Map.h"
#include "Load.h"
int MAP::getmapnum() { return mapnum; }

int MAP::getblack_t() { return black_t; }

void MAP::setmapnum(int i) { mapnum = i; }

void MAP::setblack_t(int i) { black_t = i; }

//void MAP::CreateBlack(HINSTANCE g_hinst)
//{
//	hbitbk = LoadBlack(hbitbk, g_hinst);
//}

void MAP::CreateMap(HINSTANCE g_hinst)
{
	hbitbk = LoadBK(hbitbk,g_hinst, mapnum);
	if (mapnum == 9) ms = 0;
}

void MAP::CreateUi(HINSTANCE g_hinst)
{
	hbitui = LoadUi(hbitui, g_hinst);
}

void MAP::CreateHP(HINSTANCE g_hinst)
{
	hbithp = LoadHP(hbithp, g_hinst);
}

void MAP::CreateDie(HINSTANCE g_hinst)
{
	hbitdie = LoadDieNotice(g_hinst);
}

void MAP::CreateStart(HINSTANCE g_hinst)
{
	hbitstart[0] = LoadStart(hbitstart[0],g_hinst,0);
	hbitstart[1] = LoadStart(hbitstart[1], g_hinst,1);
	hbitstart[2] = LoadStart(hbitstart[2], g_hinst,2);
}

void MAP::CreateHelp(HINSTANCE g_hinst)
{
	hbithelp[0] = LoadHelp(hbithelp[0],g_hinst,0);
	hbithelp[1] = LoadHelp(hbithelp[1], g_hinst,1);
}


void MAP::ChangeDieNotice(HINSTANCE g_hinst,int i)
{
	hbitdie = LoadDieNoticeChange(g_hinst, i);
}



bool MAP::BlackTime()
{
	if (black_t > 0) {
		black_t--; 
		return true;
	}
	return false;
}

void MAP::movemap()
{
	ms += MAPSPEED;
	if (ms >= 3021) ms = 0;
}

void MAP :: DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview)
{
	
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitbk);
	FillRect(mem1dc, &rectview,HBRUSH(RGB(0, 0, 0)));
	if (mapnum == 9)
	{
		BitBlt(mem1dc, 0, 0, 3021, 768, mem2dc, ms, 0, SRCCOPY);
		if (ms >= 1997)
		BitBlt(mem1dc, (3021 - ms), 0, rectview.right, 768, mem2dc, 0, 0, SRCCOPY);
	}
	else	//일반
	{
		BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//맵 전체 새로고침
	}

	if (mapnum == 13)
	{
		HFONT hfont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
		HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
		TCHAR count[100];
		TextOut(mem1dc, 100, 3400, L"점프 횟수 : ", lstrlenW(L"점프 횟수 : "));
		_itow_s(jumpcount, count, 10);
		TextOut(mem1dc, 300, 3400, count, lstrlenW(count));

		TextOut(mem1dc, 500, 3400, L"죽은 횟수 : ", lstrlenW(L"죽은 횟수 : "));
		_itow_s(diecount, count, 10);
		TextOut(mem1dc, 700, 3400, count, lstrlenW(count));
		SelectObject(mem1dc, oldfont);
		DeleteObject(hfont);
	}


	DeleteObject(mem2dc);
}

void MAP::DrawLoadBK(HDC& mem1dc, HDC& mem2dc, BLENDFUNCTION bf)
{
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc의 캐릭터그릴공간만큼만 얻어온다(실제 mem1dc에는 배경이있으므로 0,0 부터 MAPWIDTH,MAPHEIGHT 까지의 비트맵이 들어감)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, MAPWIDTH, MAPHEIGHT);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//여기서 0,0 ~62,50 까지의 비트맵을 캐릭터기준으로 바꿔준다 (플레이어가 있는 위치의 비트맵을 복사함)
	BitBlt(gdidc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, BLACKNESS);



	if (black_t > 0)	//맵 이동할때
	{
		GdiAlphaBlend(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, gdidc, 0, 0, MAPWIDTH, MAPHEIGHT, bf);
		//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, BLACKNESS);
	}


	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
}
//상태창
void MAP::DrawUi(HDC& mem1dc, HDC& mem2dc,CAMERA camera)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitui);
	TransparentBlt(mem1dc, camera.getx()+400, camera.gety()+700, 199, 65, mem2dc, 0, 0, 199, 65, RGB(0, 255, 0));
	//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//Ui 전체 새로고침
	DeleteObject(mem2dc);
}
//HP바
void MAP::DrawHP(HDC& mem1dc, HDC& mem2dc, CAMERA camera,PLAYER player)
{
	int hp = player.gethp() * 171 / 100;
	TCHAR hpname[100];
	_itow_s(player.gethp(), hpname, 10);
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
	HFONT oldfont =(HFONT)SelectObject(mem1dc, hfont);
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbithp);
	BitBlt(mem1dc, camera.getx() + 421, camera.gety() + 728, hp, 65, mem2dc, 0, 0, SRCCOPY);
	SetBkMode(mem1dc, 1);
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, camera.getx() + 481, camera.gety() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.getx() + 483, camera.gety() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.getx() + 482, camera.gety() + 727, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.getx() + 482, camera.gety() + 729, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(255, 255, 255));			  
	TextOut(mem1dc, camera.getx() + 482, camera.gety() + 728, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(0, 0, 0));					  
	TextOut(mem1dc, camera.getx() + 504, camera.gety() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.getx() + 506, camera.gety() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.getx() + 505, camera.gety() + 727, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.getx() + 505, camera.gety() + 729, L"/100", lstrlenW(L"/100"));
	SetTextColor(mem1dc, RGB(255, 255, 255));					 
	TextOut(mem1dc, camera.getx() + 505, camera.gety() + 728, L"/100", lstrlenW(L"/100"));
	//StretchBlt(mem1dc, camera.getx() + 421, camera.gety() + 728, hp, 65, mem2dc, 0, 0,hp, 65,SRCCOPY);
	//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//HP 전체 새로고침
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
	DeleteObject(mem2dc);
}

void MAP::DrawDie(HDC& mem1dc, HDC& mem2dc, CAMERA camera, Sound& sound)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitdie);
	TransparentBlt(mem1dc, camera.getx() + 380, camera.gety() + 240, 260, 130, mem2dc, 0, 0, 260, 130, RGB(255, 0, 0));
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 bold"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);

	SetTextColor(mem1dc, RGB(32, 108, 168));
	TextOut(mem1dc, camera.getx() + 459, camera.gety() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, camera.getx() + 461, camera.gety() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, camera.getx() + 460, camera.gety() + 259, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, camera.getx() + 460, camera.gety() + 261, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));

	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, camera.getx() + 460, camera.gety() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);

	hfont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
	oldfont = (HFONT)SelectObject(mem1dc, hfont);
	TextOut(mem1dc, camera.getx() + 460, camera.gety() + 285, L"아쉽지만 확인을 누르시면 맵의 맨 처음", lstrlenW(L"아쉽지만 확인을 누르시면 맵의 맨 처음."));
	TextOut(mem1dc, camera.getx() + 460, camera.gety() + 297, L"위치로 돌아가게 됩니다. 포기하지 마세요!", lstrlenW(L"위치로 돌아가게 됩니다. 포기하지 마세요!"));



	DeleteObject(mem2dc);



}

void MAP::DrawStart(HDC& mem1dc, HDC& mem2dc,int i)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitstart[i]);
	TransparentBlt(mem1dc, 292, 490, 138, 82, mem2dc, 0, 0,138,82,RGB(255,0,0));
	

	DeleteObject(mem2dc);



}
void MAP::DrawHelp(HDC& mem1dc, HDC& mem2dc, int i)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbithelp[i]);
	if (i == 0)
	{
		TransparentBlt(mem1dc, 290, 345, 138,82, mem2dc, 0, 0, 138, 82, RGB(60, 60, 60));
	}
	
	else if(i == 1) TransparentBlt(mem1dc, 215,300, 400, 200, mem2dc, 0, 0, 400, 200, RGB(0, 0, 0));

	DeleteObject(mem2dc);

}
