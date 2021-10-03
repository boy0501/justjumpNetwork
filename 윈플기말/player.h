#pragma once
#include <windows.h>
#include"Sound.h"

class PLAYER {
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
	HBITMAP hbitcur;    //현재 사용중인 비트맵
	HBITMAP hbitwalk;   //걸을때 비트맵
	HBITMAP hbitex;		//점프,숙이기 줄타기 비트맵
	int bx, by, bw, bh; //비트맵의 스프라이트위치를 바꾸기위한 좌표 변수
public:
	float falldy = 0;	//속도 제어
	float GroundAccel = 0.5;	//중력가속도

	PLAYER();
	// 캐릭터위치 초기화 함수
	void initPos();
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	void setx(int);
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	void sety(int);
	//떨어지기 시작한 시점의 y좌표이다
	void setsavey(int);
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	void setw(int);			//세터
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	void seth(int);
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	void setcharw(int);			//세터
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	void setcharh(int);
	//플레이어의 hp값이다
	void sethp(int);
	//떨어질때 x값 천천히 이동시켜주기 위한 변수
	void setadjspd(int);	
	//1기본상태,2점프상태,3숙이기상태,4이동상태,5줄에 매달린상태,6피격상태,7공중에있는상태
	void setstate(int);
	//1왼쪽 2오른쪽 3위 4아래
	void setdir(int);
	//이게 움직이고있는 상황인지 아닌지 구분 1이면왼쪽으로움직임  2면 오른쪽으로 움직임0이면 안움직임 3이면 위로움직임 4면아래로움직임
	void setCMD_move(int);
	//이게 피격상태인지 아닌지 구분 1이면 피격 0이면 피격아님
	void setCMD_hurt(bool);
	//이게 피격상태인지 아닌지 구분 1이면 피격 0이면 피격아님
	void setCMD_ropehurt(bool);
	//죽은지 아닌지 1이면 죽음 0이면 삼
	void setCMD_die(bool);
	//게임모드설정 0이면 일반 1이면 플라잉
	void setGamemode(bool);
	//피격시 무적 시간 2초기준 0이면 무적시간 끝
	void setstealth(int);
	//하강중 가시로인한 강제적 좌표이동값 0이면 이동끝 -이면 왼쪽으로가야함 + 이면 오른쪽으로 강제이동
	void setspike_hurt(int);
	//비트맵설정
	void setBit(HINSTANCE);
	//기본 비트맵 Pos 설정
	void initBitPos();
	


	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	int getx();
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	int gety();
	//떨어지기 시작한 y점의 좌표이다
	int getsavey();
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	int getw();			//게터
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	int geth();
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	int getcharw();			//게터
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	int getcharh();
	//플레이어의 hp값이다
	int gethp();
	//떨어질때 x값 천천히 이동시켜주기 위한 변수
	int getadjspd();
	//1기본상태,2점프상태,3숙이기상태,4이동상태,5줄에 정지 상태,6피격상태,7공중에있는상태 8줄 이동
	int getstate();
	//1왼쪽 2오른쪽 3위 4아래
	int getdir();
	//이게 움직이고있는 상황인지 아닌지 구분 1이면왼쪽으로움직임  2면 오른쪽으로 움직임0이면 안움직임 3이면 위로움직임 4면아래로움직임
	int getCMD_move();
	//이게 피격상태인지 아닌지 구분 1이면 피격 0이면 피격아님
	bool getCMD_hurt();
	//이게 피격상태인지 아닌지 구분 1이면 피격 0이면 피격아님
	bool getCMD_ropehurt();
	//죽은지 아닌지 1이면 죽음 0이면 삼
	bool getCMD_die();
	//게임모드설정 0이면 일반 1이면 플라잉
	bool getGamemode();
	//피격시 무적 시간 2초기준 0이면 무적시간 끝
	int getstealth();
	//줄에서 점프시 다시 못잡는시간 0.3초정도?
	int getjumpignore();
	//하강중 가시로인한 강제적 좌표이동값 0이면 이동끝 -이면 왼쪽으로가야함 + 이면 오른쪽으로 강제이동
	int getspike_hurt();











	//방향키 눌렀을때 플레이어 세팅
	void PlayerSetting(WPARAM,Sound&);
	//방향키 땠을때 플레이어 세팅풀기
	void PlayerWaiting(WPARAM);
	//떨어지는 시점의 y좌표 기억
	void fall2save();
	//플레이어 무브
	void move(int);
	//플레이어 스프라이트선택
	void selectBit();	   
	//애니메이션
	void BitMove();		 
	//플레이어 그림
	void draw(HDC&, HDC&); 
	//피격시 무적시간
	void stealthtime();
	//가시한테 찔렸을때 강제이동값
	void spike_hurttime();
	//다쳤을때 피 다는 계산식 + 뒤지면초기화
	void hurt(Sound&);
	//뒤지면 초기화
	void die();
};


extern int ROWSPEED; 	//가로 이동속도
extern int COLSPEED;	//세로 이동속도
extern bool LEFTkey;//왼쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool RIGHTkey;//오른쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool UPkey;	//위쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool DOWNkey;//아래쪽키 눌렀는지 1이면 누름 0이면 안누름
extern int jumpcount;
extern int diecount;
		//왼쪽오른쪽키 동시에 눌려있는지확인