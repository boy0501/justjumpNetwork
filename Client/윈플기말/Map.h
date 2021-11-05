#pragma once
#include <windows.h>
#include <string>
#include <memory>
#include "Camera.h"
#include "player.h"
#define MAPWIDTH 1024
#define MAPHEIGHT 4098
#define MAPSPEED 5

#define LOGINBG 1
#define STARTBG 9
class MAP {
	int mapnum = 1;	//1�� �α���ȭ��, 9�� ���ӽ�����ȭ��, 13�� ���� �� ȭ��
	int black_t = 0;
	int ms;
	int index;
	HBITMAP hbitbk;
public:
	std::shared_ptr<class UI> mStartui;
	std::shared_ptr<class UI> mGameUi;
	std::shared_ptr<class UI> mDieUi;
	~MAP();
	//�ʹ�ȣ�� ���� 10~
	int getmapnum();
	//Black Screen Time get
	int getblack_t();

	//�ʹ�ȣ�� �ٲ�
	void setmapnum(int);
	//Black Screen Time set
	void setblack_t(int);

	//For 2 sec Screen hide
	bool BlackTime();
	//�� ����� ������
	void movemap();
	void DrawBK(HDC&, HDC&, RECT&);
	//������ �Ÿ����� �� ����
	void DrawLoadBK(HDC&, HDC&, BLENDFUNCTION);
	//die ���º�ȭ
	void ChangeDieNotice(HINSTANCE,int);

	void UpdateFontSize(HWND& hwnd);
	/*void CreateBlack(HINSTANCE)*/;
	void CreateMap(HINSTANCE);


	SIZE mFontSize;		//ĳ���� ���� ���ڿ� �� size���庯��
	bool LoginInputFlag = false;		//false�� ���̵��Է¹޴� ���� , true�� ��й�ȣ �Է¹޴� ����
};
