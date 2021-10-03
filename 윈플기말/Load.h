#pragma once
#include <Windows.h>

//HBITMAP LoadBlack(HBITMAP, HINSTANCE);
HBITMAP LoadBK(HBITMAP, HINSTANCE, int);
//상태 Ui표시
HBITMAP LoadUi(HBITMAP, HINSTANCE);
//HP Ui 표시
HBITMAP LoadHP(HBITMAP, HINSTANCE);
//시작버튼
HBITMAP LoadStart(HBITMAP , HINSTANCE,int);
//조작법
HBITMAP LoadHelp(HBITMAP, HINSTANCE, int);

HBITMAP LoadObj(HBITMAP, HINSTANCE, int);

HBITMAP LoadWalk(HINSTANCE);

HBITMAP LoadEx(HINSTANCE);
//죽으면 뜨는 상태창
HBITMAP LoadDieNotice(HINSTANCE);
//클릭,확인에 손 올라갈때 변화
HBITMAP LoadDieNoticeChange(HINSTANCE, int);


//클릭,확인에 손 올라갈때 변화
HBITMAP LoadHelpChange(HINSTANCE, int);