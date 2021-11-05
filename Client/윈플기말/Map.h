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
	int mapnum = 1;	//1은 로그인화면, 9는 게임시작전화면, 13은 게임 끝 화면
	int black_t = 0;
	int ms;
	int index;
	HBITMAP hbitbk;
public:
	std::shared_ptr<class UI> mStartui;
	std::shared_ptr<class UI> mGameUi;
	std::shared_ptr<class UI> mDieUi;
	~MAP();
	//맵번호를 읽음 10~
	int getmapnum();
	//Black Screen Time get
	int getblack_t();

	//맵번호를 바꿈
	void setmapnum(int);
	//Black Screen Time set
	void setblack_t(int);

	//For 2 sec Screen hide
	bool BlackTime();
	//맵 배경이 움직임
	void movemap();
	void DrawBK(HDC&, HDC&, RECT&);
	//서서히 거매지는 것 구현
	void DrawLoadBK(HDC&, HDC&, BLENDFUNCTION);
	//die 상태변화
	void ChangeDieNotice(HINSTANCE,int);

	void UpdateFontSize(HWND& hwnd);
	/*void CreateBlack(HINSTANCE)*/;
	void CreateMap(HINSTANCE);


	SIZE mFontSize;		//캐럿을 위한 문자열 총 size저장변수
	bool LoginInputFlag = false;		//false면 아이디입력받는 상태 , true면 비밀번호 입력받는 상태
};
