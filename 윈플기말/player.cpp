#pragma comment (lib, "Msimg32.lib")
#include "player.h"
#include "Load.h"
#include <iostream>
int ROWSPEED = 3; 	//가로 이동속도
int COLSPEED = 10;	//세로 이동속도
int ROPESPEED = 2;
bool LEFTkey = 0;//왼쪽키 눌렀는지 1이면 누름 0이면 안누름
bool RIGHTkey = 0;//오른쪽키 눌렀는지 1이면 누름 0이면 안누름
bool UPkey = 0;	//위쪽키 눌렀는지 1이면 누름 0이면 안누름
bool DOWNkey = 0;//아래쪽키 눌렀는지 1이면 누름 0이면 안누름
bool LRkey = 0;//왼쪽오른쪽키 동시에 눌렀는지 1이면눌림 0이면 안눌림
bool UDkey = 0;
int jumpcount = 0;
int diecount = 0;
PLAYER::PLAYER()
{
	// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다. 
	x = 80; //100 캐릭터의 중심x좌표
	y = 655; //3800 캐릭터의 중심y좌표
	savey = 3700;	//처음 캐릭터의 중심좌표
	w = 14; //캐릭터 width의 절반
	h = 25;	//캐릭터 hegiht의 절반
	charw = 31;	//캐릭터이펙트 width의 절반
	charh = 25;	//캐릭터이펙트 height의 절반
	hp = 100;	//캐릭터 hp는 100
	state = 1; //캐릭터의 state
	dir = 2;
	adjustspd = 0;
	stealth = 0;
	spike_hurt = 0;
	COMMAND_move = false;
	COMMAND_hurt = false;
	COMMAND_die = false;
	COMMAND_ropehurt = false;
	
}
void PLAYER::initPos()
{
	x = 80;
	y = 3700;
	savey = 3700;
	w = 14;
	h = 25;
	state = 7;
	dir = 2;
	adjustspd = 0;
	stealth = 0;
	spike_hurt = 0;
	COMMAND_move = false;
	COMMAND_hurt = false;
	COMMAND_die = false;
}
void PLAYER::setx(int i)
{
	x = i;
}

void PLAYER::sety(int i)
{
	y = i;
}

void PLAYER::setsavey(int i)
{
	savey = i;
}

void PLAYER::setw(int i)
{
	w = i;
}

void PLAYER::seth(int i)
{
	h = i;
}

void PLAYER::setcharw(int i)
{
	charw = i;
}

void PLAYER::setcharh(int i)
{
	charh = i;
}

void PLAYER::sethp(int i)
{
	hp = i;
}

void PLAYER::setadjspd(int i)
{
	adjustspd = i;
}

void PLAYER::setstate(int i)
{
	state = i;
}

void PLAYER::setdir(int i)
{
	dir = i;
}
void PLAYER::setCMD_move(int i)
{
	COMMAND_move = i;
}
void PLAYER::setCMD_hurt(bool i)
{
	COMMAND_hurt = i;
}
void PLAYER::setCMD_ropehurt(bool i)
{
	COMMAND_ropehurt = i;
}
void PLAYER::setCMD_die(bool i)
{
	COMMAND_die = i;
}
void PLAYER::setGamemode(bool i)
{
	Gamemode = i;
}
void PLAYER::setstealth(int i)
{
	stealth = i;
}
void PLAYER::setspike_hurt(int i)
{
	spike_hurt = i;
}
void PLAYER::setBit(HINSTANCE g_hinst)
{
	hbitwalk = LoadWalk(g_hinst);
	hbitex = LoadEx(g_hinst);
}

void PLAYER::initBitPos()
{
	bx = 0;
	by = 0;
	bw = 0;
	bh = 0;
}

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ세터
int PLAYER::getx()
{
	return x;
}

int PLAYER::gety()
{
	return y;
}

int PLAYER::getsavey()
{
	return savey;
}

int PLAYER::getw()
{
	return w;
}

int PLAYER::geth()
{
	return h;
}

int PLAYER::getcharw()
{
	return charw;
}

int PLAYER::getcharh()
{
	return charh;
}

int PLAYER::gethp()
{
	return hp;
}

int PLAYER::getadjspd()
{
	return adjustspd;
}

int PLAYER::getstate()
{
	return state;
}
int PLAYER::getdir()
{
	return dir;
}
int PLAYER::getCMD_move()
{
	return COMMAND_move;
}
bool PLAYER::getCMD_hurt()
{
	return COMMAND_hurt;
}
bool PLAYER::getCMD_ropehurt()
{
	return COMMAND_ropehurt;
}
bool PLAYER::getCMD_die()
{
	return COMMAND_die;
}
bool PLAYER::getGamemode()
{
	return Gamemode;
}
int PLAYER::getstealth()
{
	return stealth;
}
int PLAYER::getjumpignore()
{
	return jumpignore;
}
int PLAYER::getspike_hurt()
{
	return spike_hurt;
}
//플레이어 상태 변경
void PLAYER::PlayerSetting(WPARAM wParam, Sound& sound)
{
	if (wParam == VK_LEFT)
	{
		
		LEFTkey = true;				//키 누름상태
		if (RIGHTkey == true)		//좌우를 동시에 누르고있다면 움직이지않음
		{
			LRkey = true;
			if (state == 4)		//움직이고 있을때나 1로 해서 멈추게하는거지 다를때 1로바꾸면 난리남
				state = 1;
			return;
		}

		if (state == 7)
		{
			dir = 1;	//방향은 바꿔주지만 움직임형태는 냅둠 (아래 move에서 몇초이상 누르면 조금씩은 바뀌게해줌)
		}
		else if (state == 1 || state == 4)
		{

			if (state == 1)//멈춰있다가 움직일때 한번 바로 움직여줘야함
			{

				state = 4;
				dir = 1;
				
			}
			else {
				state = 4;
				dir = 1;
			}
			COMMAND_move = 1;	//1이든 4든 누르면 일단은 움직임형태를 바꿔줌
			std::cout << "LEFT눌림" << std::endl;
		}
		else if (state == 2)
		{
			if (COMMAND_hurt != 1)	//쳐맞고있을때는 이 로직 안통해요~
				ROWSPEED = 1;	//점프했을때 방향을 바꾸려하면 드라마틱하게 다시 오는경우는 없지만 그래도 원했던것보단 조금 나감
			dir = 1;	//방향은 바꿔주지만 움직임형태는 냅둠
		}
		else if (state == 3)
		{

			h += 12;
			y -= 12;
			state = 4;
			dir = 1;
			COMMAND_move = 1;
		}
		else if (state == 5 || state == 8)
		{
			dir = 1;
			//COMMAND_move = 1;
		}
		return;
	}
	if (wParam == VK_RIGHT)
	{
		RIGHTkey = true;	//키 누름상태
		if (LEFTkey == true)
		{
			LRkey = true;
			if (state == 4)		//움직이고 있을때나 1로 해서 멈추게하는거지 다를때 1로바꾸면 난리남
				state = 1;
			return;
		}
		if (state == 7)
		{
			dir = 2;	//방향은 바꿔주지만 움직임형태는 냅둠 (아래 move에서 몇초이상 누르면 조금씩은 바뀌게해줌)
		}
		else if (state == 1 || state == 4)
		{
			if (state == 1)//멈춰있다가 움직일때 한번 바로 움직여줘야함
			{

				state = 4;
				dir = 2;
			}
			else {
				state = 4;
				dir = 2;
				
			}
			COMMAND_move = 2;	//1이든 4든 누르면 일단은 움직임형태를 바꿔줌
			std::cout << "RIGHT 눌림" << std::endl;
		}
		else if (state == 2)
		{
			if (COMMAND_hurt != 1)	//쳐맞고있을때는 이 로직 안통해요~
				ROWSPEED = 1;	//점프했을때 방향을 바꾸려하면 드라마틱하게 다시 오는경우는 없지만 그래도 원했던것보단 조금 나감
			dir = 2;	//방향은 바꿔주지만 움직임형태는 냅둠
		}
		else if (state == 3)
		{
			h += 12;
			y -= 12;
			state = 4;
			dir = 2;
			COMMAND_move = 2;
		}
		else if (state == 5 || state == 8)
		{
			dir = 2;
			//COMMAND_move = 2;
		}

		return;
	}
	if (wParam == VK_UP)
	{
		UPkey = true;

		if (DOWNkey == true)
		{
			UDkey = true;
			if (state == 8)
				state = 5;
			return;
		}
		if (state == 5 || state == 8)
		{
			state = 8;
			COMMAND_move = 3;

		}

		return;
	}
	if (wParam == VK_DOWN)
	{
		if (COMMAND_hurt == 1)
			return;
		DOWNkey = true;
		if (UPkey == true)
		{
			UDkey = true;
			if (state == 8)
				state = 5;
			return;
		}
		if (state == 5 || state == 8)
		{
			
			if (state == 5)//멈춰있다가 움직일때 한번 바로 움직여줘야함
			{

				state = 8;
			}
			else {
				//BitMove();
				state = 8;
			}
			COMMAND_move = 4;

		}
		else if (state == 1) {

			state = 3;	//숙이는거는 가만히 있을때만 가능하다
			h -= 12;		//숙이면 키도 줄어들어야한다.
			y += 12;
		}

		return;
	}
	if (wParam == VK_SPACE)
	{
		if (DOWNkey == true)//수그리고있을땐 점프못함
		{
			return;	//아무것도해주지않는다 현상태유지
		}
		if (state == 5 || state == 8)//줄에 매달렸을때
		{
			if (LRkey == 0)		//동시에 좌우키가 눌리지 않았으면서
			{
				if (LEFTkey == 1 || RIGHTkey == 1)	//둘중에 하나의 키라도 누르고있었다면 점프뜀 하지만 아니면 못뜀
				{
					COMMAND_move = dir;	//그리고 이때 어디로뛸건지 강제로 정함
					jumpignore = 2;	//점프시 다시못잡게도 바꿔줌
				}
				else return;//아니면못뜀
			}else return;//아니면 못뜀
		}
		if (state != 2 && state != 7)	//점프나 공중이아니라면 점프뛸수있다. 하지만 줄에매달렸을때도 안되긴 마찬가지
		{
			FMOD_Channel_Stop(sound.Channel[1]);
			FMOD_System_PlaySound(sound.System, sound.effectSound[0], NULL, 0, &sound.Channel[1]);
			falldy = 10;	//임시
			jumpcount++;
			state = 2;
			savey = y;
		}
		return;
	}
}
//플레이어 대기상태
void PLAYER::PlayerWaiting(WPARAM wParam)
{
	if (wParam == VK_DOWN)
	{
		if (UPkey == true)
		{
			if (state == 5)			//둘다눌럿을때의 로직은 state==1일때에만 발동이 된다. 
				COMMAND_move = 3;
		}
		else if (UPkey == false)	//오른쪽키를 누르고있지 않았다면 움직이는상태였을땐 멈춰줘야한다.
		{
			if (state == 8)
			{
				state = 5;
				COMMAND_move = 0;	//움직이는 방향은 그대로지만 움직이지는 않는다.
			}
			else if (state == 5)	//종종버그성 플레이로인해서 (점프키와 동시에 키를 누른후 바닥에 닿음과 동시에 땔때) 이경우가있는데, 이때도 멈춰주도록한다.
			{
				COMMAND_move = 0;
			}
		}
		if (DOWNkey == true)
		{
			if (state == 3)
			{
				h += 12;
				y -= 12;	//다시 키 늘려줌
				state = 1;
			}
		}

		UDkey = false;
		DOWNkey = false;
		return;
	}
	if (wParam == VK_LEFT)
	{
		if (RIGHTkey == true)		//오른쪽키도 누르고있었다면 왼쪽키를 땟을때 오른쪽으로 몸을틀어야한다
		{
			dir = 2;
			if (state == 1)			//둘다눌럿을때의 로직은 state==1일때에만 발동이 된다. 
				COMMAND_move = 2;
		}
		else if (RIGHTkey == false)	//오른쪽키를 누르고있지 않았다면 움직이는상태였을땐 멈춰줘야한다.
		{
			if (state == 4)
			{
				state = 1;
				COMMAND_move = 0;	//움직이는 방향은 그대로지만 움직이지는 않는다.
			}
			else if (state == 1)	//종종버그성 플레이로인해서 (점프키와 동시에 키를 누른후 바닥에 닿음과 동시에 땔때) 이경우가있는데, 이때도 멈춰주도록한다.
			{
				COMMAND_move = 0;
			}
			if (DOWNkey == true)//만약 수그리고있었다면
			{
				if (state == 1)	//수그리기의 원래 알고리즘인 state==1 일때만 수그리도록 한다
				{
					state = 3;
					h -= 12;
					y += 12;//원래대로 돌려놔주자
				}
			}
		}


		LRkey = false;				//한개를 땠으니 false
		LEFTkey = false;			//LEFTkey 땠으니 false

		//std::cout << "LEFT 똄" << std::endl;

		return;
	}
	if (wParam == VK_RIGHT)
	{
		if (LEFTkey == true)		//왼쪽키도 누르고있었다면 오른쪽키를 땟을때 왼쪽으로 몸을틀어야한다
		{
			dir = 1;
			if (state == 1)			//둘다눌럿을때의 로직은 state==1일때에만 발동이 된다. 
				COMMAND_move = 1;
		}
		else if (LEFTkey == false)	//왼쪽키를 누르고있지 않았다면 움직이는상태였을땐 멈춰줘야한다.
		{
			if (state == 4)
			{
				state = 1;
				COMMAND_move = 0;	//움직이는 방향은 그대로지만 움직이지는 않는다.
			}
			else if (state == 1)	//종종버그성 플레이로인해서 (점프키와 동시에 키를 누른후 바닥에 닿음과 동시에 땔때) 이경우가있는데, 이때도 멈춰주도록한다.
			{
				COMMAND_move = 0;
			}
			if (DOWNkey == true)//만약 수그리고있었다면
			{
				if (state == 1)	//수그리기의 원래 알고리즘인 state==1 일때만 수그리도록 한다
				{
					state = 3;
					h -= 12;
					y += 12;//원래대로 돌려놔주자
				}
			}
		}



		LRkey = false;				//한개를 땠으니 false
		RIGHTkey = false;			//RIGHTkey 땠으니 false
		//std::cout << "RIGTH 뗌" << std::endl;

		return;
	}
	if (wParam == VK_UP)
	{
		if (DOWNkey == true)
		{
			if (state == 5)			//둘다눌럿을때의 로직은 state==5일때에만 발동이 된다. 
				COMMAND_move = 4;
		}
		else if (DOWNkey == false)	
		{
			if (state == 8)
			{
				state = 5;
				COMMAND_move = 0;	//움직이는 방향은 그대로지만 움직이지는 않는다.
			}
		}

		UPkey = false;
		UDkey = false;
		return;
	}
	return;
}


//플레이어 움직임
void PLAYER::move(int obj_t)
{

	if (state == 1)
	{
		adjustspd = 0;		//수직낙하 한 후는 1상태가 되므로 여기서 초기화시켜줌
		if (LRkey == true)
		{

		}
		else if (RIGHTkey == true)
		{
			dir = 2;
			state = 4;
		}
		else if (LEFTkey == true)
		{
			dir = 1;
			state = 4;
		}

		//else {
		//	if (COMMAND_move == 1)
		//	{
		//		x -= ROWSPEED;
		//	}
		//	else if (COMMAND_move == 2)
		//	{
		//		x += ROWSPEED;
		//	}
		//}
		//가만히있을때는 움직일수 없는데 왜 움직였냐 여태
	}
	else if (state == 2 )	//점프상태일때도 진행방향으로 이동은해야함 
	{
		if (COMMAND_hurt == true)	//피격당한경우
		{
			if (COMMAND_move == 1)
			{

				x -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED;
			}
			//y -= 1;
			if (abs(y - savey) > 40) {
				y -= 3;
			}
			else {
				y -= COLSPEED / 2;
			}
			if (abs((y - savey)) >= 40)	//40픽셀만큼 피격당해서 위로 살짝뜸
			{
				state = 7;			//다시 땅으로 떨어지게함
				savex = x;			//이순간의 x좌표를 기억함(가속도를 받다가 멈춘것처럼 해줄예정)
			}
		}
		else if (COMMAND_hurt == false)	//일반상태
		{
			if (COMMAND_move == 1)
			{

				x -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED;
			}
			//y -= 1;
			//if (abs(y - savey) > 80) {
			//	y -= 3;
			//}
			//else 
			//{
			//	y -= COLSPEED;
			//}
			//if (abs((y - savey)) >= 100)	//30픽셀만큼 점프했다면
			//{
			//	state = 7;			//다시 땅으로 떨어지게함
			//	savex = x;			//이순간의 x좌표를 기억함(가속도를 받다가 멈춘것처럼 해줄예정)
			//}
			
			/*
			위의 쌩 초짜 코드를 지워버리고
			가속도 g를 적용하는 코드로 수정 - 2021-07-05
			GroundAccel은 중력가속도인데 그냥 0.98로했음
			falldy는 말 그대로 그냥 dy임 플레이어의 y좌표 y에 dy를 계속 더해주고
			굉~장히 맘에 안들지만 ObjectManager에서 모든 충돌검사를 동시에 해주면서 
			거기서 피격판정을 내버림 여기서는 어쨌든 y값만 조절해주면 된다는 점
			하도 옛날에 짠 코드라 수정을 조금씩 해보려고함. 
			*/
			if (falldy > -5)	//물체는 최대낙하속력이 존재함 나는 -10으로 설정 -> define화 시켜도 될듯
				falldy -= GroundAccel;	//속력은 가속도에 의해 계속 변화된다.
			if (falldy < 0)	//속력이 -가 되면 떨어지고있다는 점! 코드를 더럽게 짜서 state로 플레이어 상태를 변화시켜줘야 하는 불편함이 존재함				
			{
				std::cout << "ㅇㅇ";
				state = 7;	//딱히 수정하진 않을듯? 어차피 1회용코드임
			}
			y -= falldy;
		}


	}
	else if (state == 3)
	{

	}
	else if (state == 4)
	{

		if (LRkey == true)
		{

		}
		else {
			if (obj_t % 5 == 0)
			{
				BitMove();
			}

			if (COMMAND_move == 1)
			{
				x -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED;
			}
		}
	}
	else if (state == 5)
	{

	savey = y;	//줄에매달렸을때는 그자리가 저장지점이다
		if (UDkey == true)
		{

		}
		else if (UPkey == true)
		{
			
			state = 8;

		}
		else if (DOWNkey == true)
		{
			
			state = 8;

		}
	}
	else if (state == 6)
	{
		ROWSPEED *= 3;
		stealth = 100;		//무적시간 2초
		savey = y;			//피격과 동시에 y좌표저장(적당히 내려오기 위해)
		COMMAND_hurt = true;	//피격함수 on
		state = 2;				//피격하면 공중으로 한번 붕 뜬다

	}
	else if (state == 7)
	{
		y += COLSPEED;					//아래로 떨어짐
		if (adjustspd < 1000)			//1000까지만 왼쪽으로 움직임
			adjustspd++;
		if (LEFTkey == true)			//떨어질ㄸ ㅐ 왼쪽 꾹누르고있으면
			if (adjustspd % 30 == 0)	//타이머가 30번 돌아갈때마다 한번씩 옴겨줌
				x -= ROWSPEED;
		if (RIGHTkey == true)
			if (adjustspd % 30 == 0)
				x += ROWSPEED;
		if (COMMAND_move == 1)		//왼쪽으로 움직이고있다면
		{
			if (adjustspd <= 10)	//왼쪽으로 슥 갔다가
			{
				x -= ROWSPEED;
			}
			if (adjustspd > 10)		//10번 왼쪽 갔으면 2번에 한번씩 가줌
			{
				if (adjustspd % 2 == 0)
					x -= ROWSPEED;
			}
			else if (adjustspd > 30)	//2번씩 10번 또 갔으면 이젠 5번에 1번씩 찔끔 가줌 이건 오른쪽도 똑같이 적용
			{
				if (adjustspd % 5 == 0)
					x -= ROWSPEED;
			}

			if (LEFTkey == 0)				//50칸까지는 맨처음방향대로 가고 , 그이후에 왼쪽키를 때고있으면 멈춤당하고 아니면 왼쪽으로 쭉 날라감
				if (abs(x - savex) > 50)
					COMMAND_move = 0;

		}
		else if (COMMAND_move == 2)
		{
			if (adjustspd <= 10)
			{
				x += ROWSPEED;
			}
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x += ROWSPEED;
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x += ROWSPEED;
			}
			if (RIGHTkey == 0)
				if (abs(x - savex) > 50)
					COMMAND_move = 0;
		}
	}
	else if (state == 8)
	{
		savey = y;	//줄에매달렸을때는 그자리가 저장지점이다
		if(obj_t%10==0)	//10번 타이머 돌아갈때 한번 움직이게해준다
			BitMove();
		if (UDkey == true)
		{

		}
		else {
			

			if (COMMAND_move == 3)
			{
				y -= ROPESPEED;
			}
			else if (COMMAND_move == 4)
			{
				y += ROPESPEED;
			}
		}
	}
}

//상태에따라 비트맵을 선택하는 함수
void PLAYER::selectBit()
{
	if (state == 1 || state == 4)
	{
		hbitcur = hbitwalk;
		bw = 62;
		bh = 50;
	}

	else
		hbitcur = hbitex;
}

//비트맵을 바꿔주는함수 (애니메이션)
void PLAYER::BitMove()
{
	bx += 1;//인덱스 형식으로 바꿈
	if (state == 4)
	{
		if (bx >= 5) bx = 1;
	}
	if (state == 5||state==8)
	{

		if (bx >= 2) bx = 0;
	}
}

//플레이어를 그려줌
void PLAYER::draw(HDC& mem1dc, HDC& pdc)
{
	BLENDFUNCTION bf;
	bf.AlphaFormat = 0;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;



	pdc = CreateCompatibleDC(mem1dc);
	//피격당했을시에 투명처리 해줄 dc를 mem1dc와 연결
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc의 캐릭터그릴공간만큼만 얻어온다(실제 mem1dc에는 배경이있으므로 0,0 부터 62,50 까지의 비트맵이 들어감)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, 62, 50);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//여기서 0,0 ~62,50 까지의 비트맵을 캐릭터기준으로 바꿔준다 (플레이어가 있는 위치의 비트맵을 복사함)
	BitBlt(gdidc, 0, 0, charw * 2, h * 2, mem1dc, x - charw, y - h, SRCCOPY);
	//기본 움직임
	SelectObject(pdc, hbitcur);
	if (state == 1) // 정지상태 
	{

		if (dir == 1)//왼쪽
		{
			//TransparentBlt(gdidc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 0, 62, 50, RGB(255, 255, 255));
			//gdidc는 0,0~ 62,50 이니까 이 위치에 투명한 캐릭터를 복사시켜주고 GdialphaBlend 를 통해 투명화처리 해준다.
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 0, 62, 50, RGB(255, 255, 255));

			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//오른쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}

	}
	else if (state == 4) //이동상태
	{
		if (dir == 1)//왼쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by, bw, bh, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 68, by, bw, bh, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//오른쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by + 50, bw, bh, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 68, by + 50, bw, bh, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}


	}
	else if (state == 2 || state == 7) //점프하거나 떨어질때
	{
		if (dir == 1)//왼쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 107, 62, 48, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 107, 62, 50, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//오른쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
	}
	else if (state == 3) //숙이기
	{
		//h는 줄고 y는 늘고 

		BitBlt(gdidc, 0, 0, charw * 2, 26, mem1dc, x - charw, y - h, SRCCOPY);
		if (dir == 1)//왼쪽
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 161, 62, 26, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 26, pdc, 0, 161, 62, 26, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - 12 - h + 12, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - 12 - h + 12, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
		}
		else if (dir == 2)//오른쪽
		{
			//	TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 26, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
		}
	}
	else if (state == 5 || state == 8)	//줄에 매달린상태
	{
		TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 77, 54, 62, 50, RGB(255, 255, 255));

		if (stealth > 0)
		{

			bf.SourceConstantAlpha = 155;
			GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
			bf.SourceConstantAlpha = 255;

		}
		else GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);

	}

	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
	DeleteObject(pdc);

}

void PLAYER::fall2save()
{
	savex = x;
}

void PLAYER::stealthtime()
{
	if(COMMAND_die==0)	//죽으면 무적안풀림
		if (stealth > 0)
		{
			stealth--;
			if (stealth == 0)
				COMMAND_hurt = 0;
		}
	if (jumpignore > 0)
		jumpignore--;
	
}
void PLAYER::spike_hurttime()
{
	if (spike_hurt < 0)
	{
		spike_hurt++;
		x -= 4;			//왼쪽으로감
	}
	else if (spike_hurt > 0)
	{
		spike_hurt--;
		x += 4;
	}
}

void PLAYER::hurt(Sound& sound)
{
	if (COMMAND_die == false)
		hp -= 5;
	if (hp <= 0)	//0 이하라면
	{
		hp = 0;
		COMMAND_die = true;
		COMMAND_move = false;
		state = 3;
		y += 12;
		h = 13;
		stealth = 1;
		LEFTkey = 0;
		RIGHTkey = 0;
		UPkey = 0;
		DOWNkey = 0;
		FMOD_Channel_Stop(sound.Channel[1]);
		FMOD_System_PlaySound(sound.System, sound.effectSound[2], NULL, 0, &sound.Channel[1]);
		diecount++;
	}
}