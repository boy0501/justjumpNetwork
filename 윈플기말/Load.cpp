#include "Load.h"
#include "resource.h"
#include <windows.h>
#include <tchar.h>

//HBITMAP LoadBlack(HBITMAP hbit1, HINSTANCE g_hinst)
//{
//	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk_black.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
//	return hbit1;
//
//}

//배경화면
HBITMAP LoadBK(HBITMAP& hbit1, HINSTANCE& g_hinst, int mapnum)
{
	//게임시작 인터페이스
	if (mapnum == 9)
	{
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/start_rayer1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
		return hbit1;
	}
	//게임 클리어
	else if (mapnum == 13)
	{
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/clear.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
		return hbit1;
	}
	else if (mapnum == 1)
	{
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/login.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		return hbit1;
	}
	//나머지 게임맵
	else {
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
		return hbit1;

	}
	
}
//상태 Ui표시
HBITMAP LoadUi(HBITMAP hbit, HINSTANCE g_hinst)
{
	hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;

}
//HP Ui 표시
HBITMAP LoadHP(HBITMAP hbit, HINSTANCE g_hinst)
{
	hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui_HP.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;

}
//사망 Ui
HBITMAP LoadDieNotice(HINSTANCE g_hinst)
{
	HBITMAP hbit;
	hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/notice3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;
}
//사망 Ui 상태변화
HBITMAP LoadDieNoticeChange(HINSTANCE g_hinst,int i)
{
	HBITMAP hbit=NULL;
	if(i==0)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/notice3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if (i == 1)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/notice4.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if (i == 2)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/notice5.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;
}

//시작버튼
HBITMAP LoadStart(HBITMAP hbit, HINSTANCE g_hinst, int i)
{

	if (i == 0)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/start1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if (i == 1)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/start2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if (i == 2)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/start3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;
}

//로그인버튼
HBITMAP LoadLogin(HINSTANCE& g_hinst, int num)
{
	if (num == 0)
		return (HBITMAP)LoadImage(g_hinst, TEXT("img/LoginButton0.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (num == 1)
		return (HBITMAP)LoadImage(g_hinst, TEXT("img/LoginButton1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (num == 2)
		return (HBITMAP)LoadImage(g_hinst, TEXT("img/LoginButton2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (num == 3)
		return (HBITMAP)LoadImage(g_hinst, TEXT("img/LoginButton3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (num == 4)
		return (HBITMAP)LoadImage(g_hinst, TEXT("img/idpassword.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	return NULL;
}


//조작법 Ui 상태변화
HBITMAP LoadHelpChange(HINSTANCE g_hinst, int i)
{
	HBITMAP hbit = NULL;
	if (i == 0)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/help1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if (i == 1)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/help2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;
}
//조작법
HBITMAP LoadHelp(HBITMAP hbit, HINSTANCE g_hinst,int i)
{
	if (i == 0)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/help1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if (i == 1)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/help2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;
}
HBITMAP LoadObj(HBITMAP hbit1, HINSTANCE g_hinst, int type) //오브젝트의 이미지파일을 불러옴
{
	if (type == 1) // 바닥
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if(type == 0)
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/test1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if(type == 2 || type == 7) // 발판
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if(type == 3)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 4) //컨베이어 벨트 오른쪽
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/belt.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 6) //컨베이어 벨트 왼쪽
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/belt1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 5) //투명
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if(type==101) //나사못
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/nasa.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 102)//깨진 파이프
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/breakpipe1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if(type == 103)//증기
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/gas.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 106 || type == 107) //톱니바퀴
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/gear.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 201) //포탈
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/Portal_sprite.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 301) //로프
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/rope.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	return hbit1;
}

HBITMAP LoadWalk(HINSTANCE g_hinst) // 캐릭터 이미지를 불러옴 좌우 걷기
{
	HBITMAP hbit1;
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/char_sprite.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	return hbit1;
}
HBITMAP LoadEx(HINSTANCE g_hinst) // 캐릭터 이미지를 불러옴 숙이기 점프 줄타기
{
	HBITMAP hbit1;
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/char_sprite2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	return hbit1;
}