#pragma once
#include <windows.h>
#include <string>
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
	HBITMAP hbitui;
	HBITMAP hbithp;
	HBITMAP hbitdie;
	HBITMAP hbitstart[3];
	HBITMAP hbithelp[2];
	HBITMAP hbitlogin[5];
public:
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
	//상태창
	void DrawUi(HDC&, HDC&, CAMERA);
	//HP바
	void DrawHP(HDC&, HDC&, CAMERA,PLAYER);
	//die ui
	void DrawDie(HDC&, HDC&, CAMERA, Sound&);
	//시작 버튼
	void DrawStart(HDC&, HDC&, int);
	//조작법
	void DrawHelp(HDC&, HDC&,int);
	//die 상태변화
	void ChangeDieNotice(HINSTANCE,int);
	//로그인버튼
	void DrawLogin(HDC& mem1dc, HDC& mem2dc,int num);
	/*void CreateBlack(HINSTANCE)*/;
	void CreateMap(HINSTANCE);
	void CreateUi(HINSTANCE);
	void CreateHP(HINSTANCE);
	void CreateDie(HINSTANCE);
	void CreateStart(HINSTANCE);
	void CreateHelp(HINSTANCE);
	void CreateLogin(HINSTANCE&);


	int IDinput = 1;	//아이디입력 가능
	int PWinput = 0;	//패스워드입력
	std::string id{""};		//아이디
};
