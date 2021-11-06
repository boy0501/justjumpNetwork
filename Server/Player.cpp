#include "Player.h"
#include "Network.h"

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

Player::Player()
{

}

Player::~Player()
{

}

void Player::initPos()
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

void Player::move()
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
	}
	else if (state == 2)	//점프상태일때도 진행방향으로 이동은해야함 
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
			/*if (obj_t % 5 == 0)
			{
				BitMove();
			}*/

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
		//if (obj_t % 10 == 0)	//10번 타이머 돌아갈때 한번 움직이게해준다
		//	BitMove();
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



void Player::ProcessPacket(unsigned char* p)
{
	unsigned char packet_type = p[1];
	//Player* pl = CLIENTS[client_id];
	switch (packet_type) {
	case CS_PACKET_LOGIN: {
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(p);
		strcpy_s(playername, packet->name);
		//send_login_ok_packet(c_id);
		break;
	}
	//jpark
	case CS_PACKET_MOVE: {
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);
		

		switch (packet->key) {
		case 37: //VK_LEFT
			LEFTkey = true;
			if (RIGHTkey == true)
			{
				LRkey = true;
				if (state == 4)
					state = 1;
				return;
			}
			if (state == 7) {
				dir = 1;
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
				//std::cout << "LEFT눌림" << std::endl;
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
			}
			break;
		case 39: //VK_RIGHT
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
				//std::cout << "RIGHT 눌림" << std::endl;
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
			break;
		case 38: //VK_UP
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
			break;
		case 40: //VK_DOWN
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
			break;
		case 32: //VK_SPACE
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
				}
				else return;//아니면 못뜀
			}
			if (state != 2 && state != 7)	//점프나 공중이아니라면 점프뛸수있다. 하지만 줄에매달렸을때도 안되긴 마찬가지
			{
				//Sound::GetSelf()->Sound_Play(EFFECTSOUND, JUMPEF, EFVOL);
				falldy = 10;	//임시
				jumpcount++;
				state = 2;
				savey = y;
			}
			break;
		}


		
	}
	}
}
int Player::do_recv()
{
	//Player* pl = CLIENTS[client_id];

	int received;
	char tmpbuf[MAX_BUF_SIZE];
	char* ptr = tmpbuf;
	received = recv(c_socket, ptr, MAX_BUF_SIZE, 0);
	if (received == SOCKET_ERROR)
	{
		return SOCKET_ERROR;
	}
	int remain_data = received + prev_size;
	unsigned char* packet_start = reinterpret_cast<unsigned char*>(&ptr);
	int packet_size = packet_start[0];

	while (packet_size <= remain_data)
	{
		ProcessPacket(packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}
	prev_size = remain_data;
	if (0 < remain_data)
	{
		memcpy(&buf, packet_start, remain_data);
	}
	return 0;
}