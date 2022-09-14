#pragma comment (lib, "Msimg32.lib")
#include "player.h"
#include "Load.h"
#include <iostream>

using namespace std;
//원래 3이었는데 50배 해줌 
int ROWSPEED = 150; 	//가로 이동속도
int COLSPEED = 500;	//세로 이동속도
int ROPESPEED = 150;
bool LEFTkey = 0;//왼쪽키 눌렀는지 1이면 누름 0이면 안누름
bool RIGHTkey = 0;//오른쪽키 눌렀는지 1이면 누름 0이면 안누름
bool UPkey = 0;	//위쪽키 눌렀는지 1이면 누름 0이면 안누름
bool DOWNkey = 0;//아래쪽키 눌렀는지 1이면 누름 0이면 안누름
bool LRkey = 0;//왼쪽오른쪽키 동시에 눌렀는지 1이면눌림 0이면 안눌림
bool UDkey = 0;
int jumpcount = 0;
int diecount = 0;
PLAYER::PLAYER()
	:is_active(false)
{
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	x = 80; //100 캐릭터의 중심x좌표
	y = 655; //3800 캐릭터의 중심y좌표
	bw = 62;
	bh = 50;
	savey = 3700;	//처음 캐릭터의 중심좌표
	w = 14; //캐릭터 width의 절반
	h = 25;	//캐릭터 hegiht의 절반
	charw = 31;	//캐릭터이펙트 width의 절반
	charh = 25;	//캐릭터이펙트 height의 절반
	hp = 100;	//캐릭터 hp는 100
	state = 1; //캐릭터의 state
	dir = 2;
	adjustspd = 0;
	stealth = 0;
	spike_hurt = 0;
	COMMAND_move = false;
	COMMAND_hurt = false;
	COMMAND_die = false;
	COMMAND_ropehurt = false;
	
}
PLAYER::~PLAYER()
{
}
void PLAYER::initPos()
{
	x = 80;
	y = 3700;
	savey = 3700;
	w = 14;
	h = 25;
	state = 7;
	dir = 2;
	adjustspd = 0;
	stealth = 0;
	spike_hurt = 0;
	COMMAND_move = false;
	COMMAND_hurt = false;
	COMMAND_die = false;
}
void PLAYER::setx(int i)
{
	x = i;
}

void PLAYER::sety(int i)
{
	y = i;
}

void PLAYER::setsavey(int i)
{
	savey = i;
}

void PLAYER::setw(int i)
{
	w = i;
}

void PLAYER::seth(int i)
{
	h = i;
}

void PLAYER::setcharw(int i)
{
	charw = i;
}

void PLAYER::setcharh(int i)
{
	charh = i;
}

void PLAYER::sethp(int i)
{
	hp = i;
}

void PLAYER::setadjspd(int i)
{
	adjustspd = i;
}

void PLAYER::setstate(int i)
{
	state = i;
}

void PLAYER::setdir(int i)
{
	dir = i;
}
void PLAYER::setCMD_move(int i)
{
	COMMAND_move = i;
}
void PLAYER::setCMD_hurt(bool i)
{
	COMMAND_hurt = i;
}
void PLAYER::setCMD_ropehurt(bool i)
{
	COMMAND_ropehurt = i;
}
void PLAYER::setCMD_die(bool i)
{
	COMMAND_die = i;
}
void PLAYER::setGamemode(bool i)
{
	Gamemode = i;
}
void PLAYER::setstealth(int i)
{
	stealth = i;
}
void PLAYER::setspike_hurt(int i)
{
	spike_hurt = i;
}
void PLAYER::setRanking(int i)
{
	rank = i;
}

void PLAYER::setBit(HINSTANCE g_hinst)
{
	hbitwalk = LoadWalk(g_hinst);
	hbitex = LoadEx(g_hinst);
}

void PLAYER::initBitPos()
{
	bx = 0;
	by = 0;
	bw = 0;
	bh = 0;
}



//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ세터
int PLAYER::getx()
{
	return x;
}

int PLAYER::gety()
{
	return y;
}

int PLAYER::getsavey()
{
	return savey;
}

int PLAYER::getw()
{
	return w;
}

int PLAYER::geth()
{
	return h;
}

int PLAYER::getcharw()
{
	return charw;
}

int PLAYER::getcharh()
{
	return charh;
}

int PLAYER::gethp()
{
	return hp;
}

int PLAYER::getadjspd()
{
	return adjustspd;
}

int PLAYER::getstate()
{
	return state;
}
int PLAYER::getdir()
{
	return dir;
}
int PLAYER::getCMD_move()
{
	return COMMAND_move;
}
bool PLAYER::getCMD_hurt()
{
	return COMMAND_hurt;
}
bool PLAYER::getCMD_ropehurt()
{
	return COMMAND_ropehurt;
}
bool PLAYER::getCMD_die()
{
	return COMMAND_die;
}
bool PLAYER::getGamemode()
{
	return Gamemode;
}
int PLAYER::getstealth()
{
	return stealth;
}
int PLAYER::getjumpignore()
{
	return jumpignore;
}
int PLAYER::getspike_hurt()
{
	return spike_hurt;
}
int PLAYER::getRanking()
{
	return rank;
}

//비트맵을 바꿔주는함수 (애니메이션)
void PLAYER::BitMove()
{
	bx += 1;//인덱스 형식으로 바꿈
	if (state == 4 )
	{
		//std::cout << "state4 비트맵" << std::endl;
		if (bx >= 5) bx = 1;
	}
	if (state == 5 || state == 8)
	{
		//std::cout << "state5,8 비트맵" << std::endl;
		if (bx >= 2) bx = 0;
	}
}

//플레이어를 그려줌
void PLAYER::draw(HDC& mem1dc, HDC& pdc)
{

	if (is_active == false) return;
	BLENDFUNCTION bf;
	bf.AlphaFormat = 0;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;



	pdc = CreateCompatibleDC(mem1dc);
	//피격당했을시에 투명처리 해줄 dc를 mem1dc와 연결
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc의 캐릭터그릴공간만큼만 얻어온다(실제 mem1dc에는 배경이있으므로 0,0 부터 62,50 까지의 비트맵이 들어감)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, 62, 50);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//여기서 0,0 ~62,50 까지의 비트맵을 캐릭터기준으로 바꿔준다 (플레이어가 있는 위치의 비트맵을 복사함)
	BitBlt(gdidc, 0, 0, charw * 2, h * 2, mem1dc, x - charw, y - h, SRCCOPY);
	//기본 움직임
	//SelectObject(pdc, hbitcur);
	//pdc는 hbitcur 즉 sprite가 들어있음
	if (state == 1) // 정지상태 
	{
		SelectObject(pdc, hbitwalk);
		if (dir == 1)//왼쪽
		{
			//TransparentBlt(gdidc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 0, 62, 50, RGB(255, 255, 255));
			//gdidc는 0,0~ 62,50 이니까 이 위치에 투명한 캐릭터를 복사시켜주고 GdialphaBlend 를 통해 투명화처리 해준다.
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 0, 62, 50, RGB(255, 255, 255));

			//2021-10-04 주석 추가
			//좀더 풀어서 말하면, 결국 최종적으로는 우리는 mem1dc(더블버퍼링용 hdc)에 그려줘야한다. 
			//pdc에는 sprite이미지가 있고(857line에서 해준다), sprite에서 필요한 부분만 짤라서 gdidc에 그려준 다음
			//gdialphablend로 gdidc에서 alpha값을 조절한다음 mem1dc에 최종적으로 그려주는것이다.

			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//오른쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
			if (stealth > 0)
			{
				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
			{
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
			}
		}

	}
	else if (state == 4) //이동상태
	{
		SelectObject(pdc, hbitwalk);
		if (dir == 1)//왼쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by, bw, bh, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 68, by, bw, bh, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//오른쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by + 50, bw, bh, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 68, by + 50, bw, bh, RGB(255, 255, 255));
			if (stealth > 0)
			{
				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
			{
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
			}
		}


	}
	else if (state == 2 || state == 7) //점프하거나 떨어질때
	{
		SelectObject(pdc,hbitex);
		if (dir == 1)//왼쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 107, 62, 48, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 107, 62, 50, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//오른쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
	}
	else if (state == 3) //숙이기
	{
		SelectObject(pdc, hbitex);
		//h는 줄고 y는 늘고 

		BitBlt(gdidc, 0, 0, charw * 2, 26, mem1dc, x - charw, y - h, SRCCOPY);
		if (dir == 1)//왼쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 161, 62, 26, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 26, pdc, 0, 161, 62, 26, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - 12 - h + 12, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - 12 - h + 12, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
		}
		else if (dir == 2)//오른쪽
		{
			//	TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 26, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
		}
	}
	else if (state == 5 || state == 8)	//줄에 매달린상태
	{
		SelectObject(pdc, hbitex);
		TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 77, 54, 62, 50, RGB(255, 255, 255));

		if (stealth > 0)
		{

			bf.SourceConstantAlpha = 155;
			GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
			bf.SourceConstantAlpha = 255;

		}
		else GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);

	}

	//닉네임 그려주기
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 bold"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
	SetBkMode(mem1dc, TRANSPARENT);
	SetTextColor(mem1dc, RGB(255, 108, 168));
	RECT rt{ x - 60,y + 25,x + 60,y + 65 };
	DrawText(mem1dc, mPlayerwname.c_str(), lstrlenW(mPlayerwname.c_str()), &rt, DT_CENTER);

	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);


	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
	DeleteObject(pdc);

}

void PLAYER::move(float deltatime)
{
	if (false == is_active) return;


	if (state == 2)
	{
		x += ((vx * deltatime) + ((0 * deltatime * deltatime) / 2));
		y += ((vy * deltatime) + ((-25 * deltatime * deltatime) / 2));
	}
	else 
	{
		x += ((vx * deltatime) + ((0 * deltatime * deltatime) / 2));
		y += ((vy * deltatime) + ((0 * deltatime * deltatime) / 2));
	}
}
