#pragma once
#include <Windows.h>

//HBITMAP LoadBlack(HBITMAP, HINSTANCE);
HBITMAP LoadBK(HBITMAP& hbit1, HINSTANCE& g_hinst, int mapnum);
//���� Uiǥ��
HBITMAP LoadUi(HBITMAP, HINSTANCE);
//HP Ui ǥ��
HBITMAP LoadHP(HBITMAP, HINSTANCE);
//���۹�ư
HBITMAP LoadStart(HBITMAP , HINSTANCE,int);
//�α��ι�ư
HBITMAP LoadLogin(HINSTANCE& g_hinst, int num);
//���� ��ư
HBITMAP LoadExit(HINSTANCE& g_hinst, int num);   
//���۹�
HBITMAP LoadHelp(HBITMAP, HINSTANCE, int);

HBITMAP LoadObj(HBITMAP, HINSTANCE, int);

HBITMAP LoadWalk(HINSTANCE);

HBITMAP LoadEx(HINSTANCE);
//������ �ߴ� ����â
HBITMAP LoadDieNotice(HINSTANCE);
//Ŭ��,Ȯ�ο� �� �ö󰥶� ��ȭ
HBITMAP LoadDieNoticeChange(HINSTANCE, int);


//Ŭ��,Ȯ�ο� �� �ö󰥶� ��ȭ
HBITMAP LoadHelpChange(HINSTANCE, int);