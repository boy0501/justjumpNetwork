#pragma once
#include <string>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "../Protocol/protocol.h"

class Player
{
public:
	int x, y, w, h;		// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다.
	int charw, charh;	//캐릭터 전체 사이즈이다. 
	int hp;				//플레이어의 hp이다.
	int savex, savey;	//savey 는 점프뛸때 그 순간의 y좌표를 기억하기 위함이고 x는 혹시몰라서 넣어둠
	int state;			//1기본상태,2점프상태,3숙이기상태,4이동상태,5줄 정지,6피격상태,7공중에있는상태 8 줄이동
	int dir;			//1왼쪽 2오른쪽 3위 4아래 플레이어가 보고있는'방향'
	int adjustspd;		//떨어질때 x값 천천히 이동시켜주기 위한 변수
	int COMMAND_move;	//이게 움직이고있는 상황인지 아닌지 구분 1이면왼쪽으로움직임  2면 오른쪽으로 움직임0이면 안움직임 3이면 위로움직임 4면아래로움직임
	bool COMMAND_hurt;	//쳐맞으면 1 아니면 0 점프할때 로직에 들어감
	bool COMMAND_ropehurt;	//로프에서 쳐맞으면 1 아니면 0
	bool COMMAND_die;	//죽으면 1 아니면 0
	bool Gamemode;		//0이면 일반 1이면 플라잉모드
	int stealth;		//피격시 무적 2초를 기준으로한다.
	int jumpignore;		//줄에서 점프시 줄 바로 못잡게한다. stealth 와 같이 돌아갈예정
	int spike_hurt;		//하강중 가시로인한 강제적 좌표이동값 -이면 왼쪽으로가야함 + 이면 오른쪽으로 강제이동
	int bx, by, bw, bh; //비트맵의 스프라이트위치를 바꾸기위한 좌표 변수
	bool WhenPlayerDied = false;
	float falldy = 0;	//속도 제어
	float GroundAccel = 0.5;	//중력가속도
	std::wstring mPlayerwname;	//플레이어이름 wchar방식
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

