#include "Map.h"
#include "Load.h"
#include <iostream>



MAP::~MAP()
{
	mStartui.reset();
	mDieUi.reset();
	mGameUi.reset();
	
}




int MAP::getmapnum() { return mapnum; }

int MAP::getblack_t() { return black_t; }

void MAP::setmapnum(int i) { mapnum = i; }

void MAP::setblack_t(int i) { black_t = i; }



void MAP::CreateMap(HINSTANCE g_hinst)
{
	hbitbk = LoadBK(hbitbk,g_hinst, mapnum);
	if (mapnum == 9) ms = 0;
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
	//FillRect(mem1dc, &rectview,HBRUSH(RGB(0, 0, 0)));
	if (mapnum == 9)
	{
		BitBlt(mem1dc, 0, 0, 3021, 768, mem2dc, ms, 0, SRCCOPY);
		if (ms >= 1997)
		BitBlt(mem1dc, (3021 - ms), 0, rectview.right, 768, mem2dc, 0, 0, SRCCOPY);
	}
	else if (mapnum == 1)
	{
		BitBlt(mem1dc, 0, 0, MAPWIDTH, 768, mem2dc, 0, 0, SRCCOPY);
	}
	else	//�Ϲ�
	{
		BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//�� ��ü ���ΰ�ħ
	}

	if (mapnum == 13)
	{
		HFONT hfont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮 light"));
		HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
		TCHAR count[100];
		TextOut(mem1dc, 100, 3400, L"��ŷ : ", lstrlenW(L"��ŷ : "));
		_itow_s(jumpcount, count, 10);
		TextOut(mem1dc, 300, 3400, count, lstrlenW(count));

		TextOut(mem1dc, 500, 3400, L"���� Ƚ�� : ", lstrlenW(L"���� Ƚ�� : "));
		_itow_s(diecount, count, 10);
		TextOut(mem1dc, 700, 3400, count, lstrlenW(count));
		SelectObject(mem1dc, oldfont);
		DeleteObject(hfont);


	
		//GameHUD &gameui.addButton([gameui]() {}, NULL, "img/Exit", 315, 3400, 138, 82, RGB(255, 0, 0));

		//gameui.drawExit();
		//BitBlt(mem1dc, 292, 490, 138, 82, mem2dc, 0, 0, SRCCOPY);

		
	}
	DeleteObject(mem2dc);
}

void MAP::DrawLoadBK(HDC& mem1dc, HDC& mem2dc, BLENDFUNCTION bf)
{
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc�� ĳ���ͱ׸�������ŭ�� ���´�(���� mem1dc���� ����������Ƿ� 0,0 ���� MAPWIDTH,MAPHEIGHT ������ ��Ʈ���� ��)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, MAPWIDTH, MAPHEIGHT);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//���⼭ 0,0 ~62,50 ������ ��Ʈ���� ĳ���ͱ������� �ٲ��ش� (�÷��̾ �ִ� ��ġ�� ��Ʈ���� ������)
	BitBlt(gdidc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, BLACKNESS);



	if (black_t > 0)	//�� �̵��Ҷ�
	{
		GdiAlphaBlend(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, gdidc, 0, 0, MAPWIDTH, MAPHEIGHT, bf);
		//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, BLACKNESS);
	}


	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
}