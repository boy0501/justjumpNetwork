#pragma comment (lib, "Msimg32.lib")
#include "player.h"
#include "Load.h"
#include <iostream>

using namespace std;
//���� 3�̾��µ� 50�� ���� 
int ROWSPEED = 150; 	//���� �̵��ӵ�
int COLSPEED = 500;	//���� �̵��ӵ�
int ROPESPEED = 150;
bool LEFTkey = 0;//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool RIGHTkey = 0;//������Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool UPkey = 0;	//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool DOWNkey = 0;//�Ʒ���Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool LRkey = 0;//���ʿ�����Ű ���ÿ� �������� 1�̸鴭�� 0�̸� �ȴ���
bool UDkey = 0;
int jumpcount = 0;
int diecount = 0;
PLAYER::PLAYER()
	:is_active(false)
{
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	x = 80; //100 ĳ������ �߽�x��ǥ
	y = 655; //3800 ĳ������ �߽�y��ǥ
	bw = 62;
	bh = 50;
	savey = 3700;	//ó�� ĳ������ �߽���ǥ
	w = 14; //ĳ���� width�� ����
	h = 25;	//ĳ���� hegiht�� ����
	charw = 31;	//ĳ��������Ʈ width�� ����
	charh = 25;	//ĳ��������Ʈ height�� ����
	hp = 100;	//ĳ���� hp�� 100
	state = 1; //ĳ������ state
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



//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѼ���
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

//��Ʈ���� �ٲ��ִ��Լ� (�ִϸ��̼�)
void PLAYER::BitMove()
{
	bx += 1;//�ε��� �������� �ٲ�
	if (state == 4 )
	{
		//std::cout << "state4 ��Ʈ��" << std::endl;
		if (bx >= 5) bx = 1;
	}
	if (state == 5 || state == 8)
	{
		//std::cout << "state5,8 ��Ʈ��" << std::endl;
		if (bx >= 2) bx = 0;
	}
}

//�÷��̾ �׷���
void PLAYER::draw(HDC& mem1dc, HDC& pdc)
{

	if (is_active == false) return;
	BLENDFUNCTION bf;
	bf.AlphaFormat = 0;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;



	pdc = CreateCompatibleDC(mem1dc);
	//�ǰݴ������ÿ� ����ó�� ���� dc�� mem1dc�� ����
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc�� ĳ���ͱ׸�������ŭ�� ���´�(���� mem1dc���� ����������Ƿ� 0,0 ���� 62,50 ������ ��Ʈ���� ��)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, 62, 50);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//���⼭ 0,0 ~62,50 ������ ��Ʈ���� ĳ���ͱ������� �ٲ��ش� (�÷��̾ �ִ� ��ġ�� ��Ʈ���� ������)
	BitBlt(gdidc, 0, 0, charw * 2, h * 2, mem1dc, x - charw, y - h, SRCCOPY);
	//�⺻ ������
	//SelectObject(pdc, hbitcur);
	//pdc�� hbitcur �� sprite�� �������
	if (state == 1) // �������� 
	{
		SelectObject(pdc, hbitwalk);
		if (dir == 1)//����
		{
			//TransparentBlt(gdidc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 0, 62, 50, RGB(255, 255, 255));
			//gdidc�� 0,0~ 62,50 �̴ϱ� �� ��ġ�� ������ ĳ���͸� ��������ְ� GdialphaBlend �� ���� ����ȭó�� ���ش�.
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 0, 62, 50, RGB(255, 255, 255));

			//2021-10-04 �ּ� �߰�
			//���� Ǯ� ���ϸ�, �ᱹ ���������δ� �츮�� mem1dc(������۸��� hdc)�� �׷�����Ѵ�. 
			//pdc���� sprite�̹����� �ְ�(857line���� ���ش�), sprite���� �ʿ��� �κи� ©�� gdidc�� �׷��� ����
			//gdialphablend�� gdidc���� alpha���� �����Ѵ��� mem1dc�� ���������� �׷��ִ°��̴�.

			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
			if (stealth > 0)
			{
				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
			{
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
			}
		}

	}
	else if (state == 4) //�̵�����
	{
		SelectObject(pdc, hbitwalk);
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by, bw, bh, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 68, by, bw, bh, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by + 50, bw, bh, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 68, by + 50, bw, bh, RGB(255, 255, 255));
			if (stealth > 0)
			{
				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
			{
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
			}
		}


	}
	else if (state == 2 || state == 7) //�����ϰų� ��������
	{
		SelectObject(pdc,hbitex);
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 107, 62, 48, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 107, 62, 50, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
	}
	else if (state == 3) //���̱�
	{
		SelectObject(pdc, hbitex);
		//h�� �ٰ� y�� �ð� 

		BitBlt(gdidc, 0, 0, charw * 2, 26, mem1dc, x - charw, y - h, SRCCOPY);
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 161, 62, 26, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 26, pdc, 0, 161, 62, 26, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - 12 - h + 12, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - 12 - h + 12, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
		}
		else if (dir == 2)//������
		{
			//	TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 26, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
		}
	}
	else if (state == 5 || state == 8)	//�ٿ� �Ŵ޸�����
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

	//�г��� �׷��ֱ�
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮 bold"));
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
