#pragma once
#include <windows.h>
#include <string>
#include"Sound.h"

class PLAYER {
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
	HBITMAP hbitcur;    //���� ������� ��Ʈ��
	HBITMAP hbitwalk;   //������ ��Ʈ��
	HBITMAP hbitex;		//����,���̱� ��Ÿ�� ��Ʈ��
	int bx, by, bw, bh; //��Ʈ���� ��������Ʈ��ġ�� �ٲٱ����� ��ǥ ����
	bool is_active;		//ĳ���Ͱ� �׷������ϴ��� �ƴ��� �Ǵܺ���(other player�� �׷��ֱ� ����.)
public:
	//HDC hdc, pdc, mem1dc;
	int ready_players = 0;
	bool ready_to_go = false;
	int player_cid = 0;
	bool WhenPlayerDied = false;
	char stage;			//���° ���������� �ִ���
	int rank;			//�� ������
	float falldy = 0;	//�ӵ� ����
	float GroundAccel = 0.5;	//�߷°��ӵ�
	std::string mPlayername;	//�÷��̾��̸�
	std::wstring mPlayerwname;	//�÷��̾��̸� wchar���
	PLAYER();
	~PLAYER();

	// ĳ������ġ �ʱ�ȭ �Լ�
	void initPos();
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	void setx(int);
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	void sety(int);
	//�������� ������ ������ y��ǥ�̴�
	void setsavey(int);
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	void setw(int);			//����
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	void seth(int);
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	void setcharw(int);			//����
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	void setcharh(int);
	//�÷��̾��� hp���̴�
	void sethp(int);
	//�������� x�� õõ�� �̵������ֱ� ���� ����
	void setadjspd(int);	
	//1�⺻����,2��������,3���̱����,4�̵�����,5�ٿ� �Ŵ޸�����,6�ǰݻ���,7���߿��ִ»���
	void setstate(int);
	//1���� 2������ 3�� 4�Ʒ�
	void setdir(int);
	//�̰� �����̰��ִ� ��Ȳ���� �ƴ��� ���� 1�̸�������ο�����  2�� ���������� ������0�̸� �ȿ����� 3�̸� ���ο����� 4��Ʒ��ο�����
	void setCMD_move(int);
	//�̰� �ǰݻ������� �ƴ��� ���� 1�̸� �ǰ� 0�̸� �ǰݾƴ�
	void setCMD_hurt(bool);
	//�̰� �ǰݻ������� �ƴ��� ���� 1�̸� �ǰ� 0�̸� �ǰݾƴ�
	void setCMD_ropehurt(bool);
	//������ �ƴ��� 1�̸� ���� 0�̸� ��
	void setCMD_die(bool);
	//���Ӹ�弳�� 0�̸� �Ϲ� 1�̸� �ö���
	void setGamemode(bool);
	//�ǰݽ� ���� �ð� 2�ʱ��� 0�̸� �����ð� ��
	void setstealth(int);
	//�ϰ��� ���÷����� ������ ��ǥ�̵��� 0�̸� �̵��� -�̸� �������ΰ����� + �̸� ���������� �����̵�
	void setspike_hurt(int);
	// ����
	void setRanking(int);
	//��Ʈ�ʼ���
	void setBit(HINSTANCE);
	//�⺻ ��Ʈ�� Pos ����
	void initBitPos();
	


	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	int getx();
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	int gety();
	//�������� ������ y���� ��ǥ�̴�
	int getsavey();
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	int getw();			//����
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	int geth();
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	int getcharw();			//����
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	int getcharh();
	//�÷��̾��� hp���̴�
	int gethp();
	//�������� x�� õõ�� �̵������ֱ� ���� ����
	int getadjspd();
	//1�⺻����,2��������,3���̱����,4�̵�����,5�ٿ� ���� ����,6�ǰݻ���,7���߿��ִ»��� 8�� �̵�
	int getstate();
	//1���� 2������ 3�� 4�Ʒ�
	int getdir();
	//�̰� �����̰��ִ� ��Ȳ���� �ƴ��� ���� 1�̸�������ο�����  2�� ���������� ������0�̸� �ȿ����� 3�̸� ���ο����� 4��Ʒ��ο�����
	int getCMD_move();
	//�̰� �ǰݻ������� �ƴ��� ���� 1�̸� �ǰ� 0�̸� �ǰݾƴ�
	bool getCMD_hurt();
	//�̰� �ǰݻ������� �ƴ��� ���� 1�̸� �ǰ� 0�̸� �ǰݾƴ�
	bool getCMD_ropehurt();
	//������ �ƴ��� 1�̸� ���� 0�̸� ��
	bool getCMD_die();
	//���Ӹ�弳�� 0�̸� �Ϲ� 1�̸� �ö���
	bool getGamemode();
	//�ǰݽ� ���� �ð� 2�ʱ��� 0�̸� �����ð� ��
	int getstealth();
	//�ٿ��� ������ �ٽ� ����½ð� 0.3������?
	int getjumpignore();
	//�ϰ��� ���÷����� ������ ��ǥ�̵��� 0�̸� �̵��� -�̸� �������ΰ����� + �̸� ���������� �����̵�
	int getspike_hurt();

	int getRanking();






	//�ִϸ��̼�
	void BitMove();		 
	//�÷��̾� �׸�
	void draw(HDC&, HDC&); 
};


extern int ROWSPEED; 	//���� �̵��ӵ�
extern int COLSPEED;	//���� �̵��ӵ�
extern bool LEFTkey;//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
extern bool RIGHTkey;//������Ű �������� 1�̸� ���� 0�̸� �ȴ���
extern bool UPkey;	//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
extern bool DOWNkey;//�Ʒ���Ű �������� 1�̸� ���� 0�̸� �ȴ���
extern int jumpcount;
extern int diecount;
		//���ʿ�����Ű ���ÿ� �����ִ���Ȯ��