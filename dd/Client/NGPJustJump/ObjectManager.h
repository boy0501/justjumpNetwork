#pragma once
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "player.h"
#include "object.h"
#include "Camera.h"
#include "Map.h"
//#include"UI.h"
//#include"GameHUD.h"
using namespace std;
//���ٴڰ� �÷��̾� �浹üũ 1�̸� �δ�ħ
bool collp2w(PLAYER, OBJECT);
//�÷��̾�� ������Ʈ�� ��ȣ�ۿ� �Ǵ��ϰ� �׿��°� �ٲ���
void adjustPlayer(PLAYER&, OBJECT*, MAP&, int&, HINSTANCE);

//int(�� ��ȣ) �� ���� ��ֹ� ��ġ�� �־��ְ� ��� ������Ʈ�� ������ �˷��ִ� �Լ�
int initObject(OBJECT*, int, HINSTANCE);
//ī�޶� ������ũ
void adjustCamera(CAMERA&,PLAYER&);

//int saveMapNum = 0;