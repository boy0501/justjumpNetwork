#pragma once
#include <string>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../Protocol/protocol.h"

class Player
{
public:
	int x, y, w, h;		// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�.
	int charw, charh;	//ĳ���� ��ü �������̴�. 
	int hp;				//�÷��̾��� hp�̴�.
	int savex, savey;	//savey �� �����۶� �� ������ y��ǥ�� ����ϱ� �����̰� x�� Ȥ�ø��� �־��
	int state;			//1�⺻����,2��������,3���̱����,4�̵�����,5�� ����,6�ǰݻ���,7���߿��ִ»��� 8 ���̵�
	int dir;			//1���� 2������ 3�� 4�Ʒ� �÷��̾ �����ִ�'����'
	int adjustspd;		//�������� x�� õõ�� �̵������ֱ� ���� ����
	int COMMAND_move;	//�̰� �����̰��ִ� ��Ȳ���� �ƴ��� ���� 1�̸�������ο�����  2�� ���������� ������0�̸� �ȿ����� 3�̸� ���ο����� 4��Ʒ��ο�����
	bool COMMAND_hurt;	//�ĸ����� 1 �ƴϸ� 0 �����Ҷ� ������ ��
	bool COMMAND_ropehurt;	//�������� �ĸ����� 1 �ƴϸ� 0
	bool COMMAND_die;	//������ 1 �ƴϸ� 0
	bool Gamemode;		//0�̸� �Ϲ� 1�̸� �ö��׸��
	int stealth;		//�ǰݽ� ���� 2�ʸ� ���������Ѵ�.
	int jumpignore;		//�ٿ��� ������ �� �ٷ� ������Ѵ�. stealth �� ���� ���ư�����
	int spike_hurt;		//�ϰ��� ���÷����� ������ ��ǥ�̵��� -�̸� �������ΰ����� + �̸� ���������� �����̵�
	int bx, by, bw, bh; //��Ʈ���� ��������Ʈ��ġ�� �ٲٱ����� ��ǥ ����
	bool WhenPlayerDied = false;
	float falldy = 0;	//�ӵ� ����
	float GroundAccel = 0.5;	//�߷°��ӵ�
	std::wstring mPlayerwname;	//�÷��̾��̸� wchar���
	char playername[20];

	SOCKET c_socket;
	int c_id;
	int prev_size;

	unsigned char* buf[MAX_BUF_SIZE];
	SOCKADDR_IN c_addr;
	Player();
	~Player();

	int do_recv();
	void do_send(void* packet,int bytes);
	void ProcessPacket(unsigned char* p);
};

