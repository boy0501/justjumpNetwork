#include <iostream>

#include "../Network.h"
#include "LobbyClient.h"
#include "../Map.h"
#include "../Object.h"

LobbyClient::LobbyClient()
	:elapsedtime(0),
	robby_timer(11)
{
	
}

LobbyClient::~LobbyClient()
{
}



void LobbyClient::update(float delta_time)
{
	
	elapsedtime += delta_time;
	if (elapsedtime > 1)
	{
		
	
		elapsedtime = 0;
		std::cout << "로비클라" << std::endl;
		std::cout << robby_cnt << std::endl;

		if (robby_cnt == 2)
		{

			robby_timer--;
			if (robby_timer < 0) {
				//mCss = CSS_DEAD;
				//mSn = SN_INGAME;
				//SetEvent(SceneChangeTrigger);
				robby_timer = 0;

				//gamestart initpos
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
				//mStageNum = 1;

				mCss = CSS_DEAD;
				mSn = SN_INGAME;
				SetEvent(SceneChangeTrigger);

			}

			for (int i = 0; i < robby_cnt; ++i) {
				std::cout << robby_timer << std::endl;
				sc_packet_robby packet;
				packet.size = sizeof(sc_packet_robby);
				packet.type = SC_PACKET_ROBBY;
				packet.countdown = robby_timer;
				
				CLIENTS[i]->do_send(&packet, sizeof(packet));

			}
			
		}
	}

	//obj_t += 1;
	move(delta_time);
	//BitMove();
	adjustPlayer();
	//timer();
	//send packet

	/*sc_packet_move_process packet;
	packet.size = sizeof(sc_packet_move_process);
	packet.type = SC_PACKET_MOVE_PROCESS;
	packet.dir = dir;
	packet.h = h;
	packet.id = c_id;
	packet.state = state;
	packet.stealth = stealth;
	packet.x = x;
	packet.y = y;
	do_send(&packet, sizeof(packet));*/


	Client::update(delta_time);
}



void LobbyClient::initBitPos()
{
	bx = 0;
	by = 0;
	bw = 0;
	bh = 0;
}

void LobbyClient::initPos()
{
	x = 80;
	y = 655;
	savey = 3700;
	w = 14;
	h = 25;
	//charw = 31;	
	//charh = 25;	
	hp = 100;
	state = 1;
	dir = 2;
	adjustspd = 0;
	stealth = 0;
	spike_hurt = 0;
	COMMAND_move = false;
	COMMAND_hurt = false;
	COMMAND_die = false;
	COMMAND_ropehurt = false;
}

//비트맵을 바꿔주는함수 (애니메이션)
//void LobbyClient::BitMove()
//{
//	//std::cout << "bitmove" << std::endl;
//	bx += 1; //인덱스 형식으로 바꿈
//	if (state == 4)
//	{
//		if (bx >= 5) bx = 1;
//	}
//	if (state == 5 || state == 8)
//	{
//
//		if (bx >= 2) bx = 0;
//	}
//}

void LobbyClient::move(float deltatime)
{

	//std::cout << "move호출" << std::endl;
	if (state == 1)
	{
		//std::cout << "state1" << std::endl;
		adjustspd = 0;
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
	else if (state == 2)
	{
		//std::cout << "state2" << std::endl;
		if (COMMAND_hurt == true)
		{
			if (COMMAND_move == 1)
			{
				x -= (int)(ROWSPEED * deltatime);
			}
			else if (COMMAND_move == 2)
			{
				x += (int)(ROWSPEED * deltatime);
			}
			//y -= 1;
			if (abs(y - savey) > 40) {
				y -= 3;
			}
			else {
				y -= COLSPEED / 2;
			}
			if (abs((y - savey)) >= 40)
			{
				state = 7;
				savex = x;
			}
		}
		else if (COMMAND_hurt == false)
		{
			if (COMMAND_move == 1)
			{

				x -= (int)(ROWSPEED * deltatime);
			}
			else if (COMMAND_move == 2)
			{
				x += (int)(ROWSPEED * deltatime);
			}


			if (falldy > -5)
				falldy -= GroundAccel * deltatime * 50;
			if (falldy < 0)
			{
				//std::cout << "state7로 change" << std::endl;
				state = 7;
			}
			y -= (int)(falldy * deltatime * 60);
		}


	}
	else if (state == 3)
	{
		//std::cout << "state3" << std::endl;
	}
	else if (state == 4)
	{
		//std::cout << "state4" << std::endl;
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
				x -= (int)(ROWSPEED * deltatime);

			}
			else if (COMMAND_move == 2)
			{
				x += (int)(ROWSPEED * deltatime);

			}
		}
	}
	else if (state == 5)
	{
		//std::cout << "state5" << std::endl;
		savey = y;
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
		//std::cout << "state6" << std::endl;
		ROWSPEED *= 3;
		stealth = 100;
		savey = y;
		COMMAND_hurt = true;
		state = 2;

	}
	else if (state == 7)
	{
		//std::cout << "state7들어옴" << std::endl;
		y += (int)(COLSPEED * deltatime);
		//std::cout << y << std::endl;
		if (adjustspd < 1000)
			adjustspd++;
		if (LEFTkey == true)
			if (adjustspd % 30 == 0)
				x -= (int)(ROWSPEED * deltatime);
		if (RIGHTkey == true)
			if (adjustspd % 30 == 0)
				x += (int)(ROWSPEED * deltatime);
		if (COMMAND_move == 1)
		{
			if (adjustspd <= 10)
				x -= (int)(ROWSPEED * deltatime);
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x -= (int)(ROWSPEED * deltatime);
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x -= (int)(ROWSPEED * deltatime);
			}

			if (LEFTkey == 0)
				if (abs(x - savex) > 50)
					COMMAND_move = 0;

		}
		else if (COMMAND_move == 2)
		{
			if (adjustspd <= 10)
			{
				x += (int)(ROWSPEED * deltatime);
			}
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x += (int)(ROWSPEED * deltatime);
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x += (int)(ROWSPEED * deltatime);
			}
			if (RIGHTkey == 0)
				if (abs(x - savex) > 50)
					COMMAND_move = 0;
		}
	}
	else if (state == 8)
	{
		//std::cout << "state8" << std::endl;
		savey = y;
		/*if (obj_t % 10 == 0)
			BitMove();*/
		if (UDkey == true)
		{

		}
		else {


			if (COMMAND_move == 3)
			{
				y -= (int)(ROPESPEED * deltatime);
			}
			else if (COMMAND_move == 4)
			{
				y += (int)(ROPESPEED * deltatime);
			}
		}
	}
}



//오브젝트와 플레이어 충돌체크 1이면 부닥침
//----------------------------------------
bool LobbyClient::collp2o(Object* Obj)
{

	int adjust = 10;	//발판 두께
	if (Obj->type == 1)	//땅바닥일때
	{
		if (Obj->x <= x && x <= Obj->x + Obj->w)
		{
			if (Obj->y <= y + h)
			{
				return 1;
			}
		}
	}
	else if (Obj->type <= 100) {	//플랫폼일때는 플레이어 중점이 히트박스가된다
		if (Obj->x <= x && x <= Obj->x + Obj->w)
		{
			if (Obj->y <= y + h && y + h <= Obj->y + adjust)
			{
				return 1;
				//ㅇㅇ
			}
		}
	}

	return 0;
}



//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
void LobbyClient::adjustPlayer()
{
	int check_coll = 0;	//하나라도 부딪혔는지 판별하기위함
	if (x - w < 0)
	{
		x = w;
		COMMAND_move = 0;
	}
	else if (x + w > 1023)
	{
		x = 1023 - w;
		COMMAND_move = 0;
	}

	for (auto& obj : mMap->mObjects[mStageNum])
	{
		if (collp2o(obj))
		{
			//std::cout << "부딪힘" << std::endl;
			check_coll++;	//하나라도 부딪혔으면 coll이 올라감
			if (obj->type < 101 && obj->type > 0)			//근데 그게 땅바닥이였다?
			{

				if (state == 7) //떨어지는 중일때 부딪혔다 ?
				{
					if (abs(savey - y) > 200)	//낙뎀을받아야한다면
					{
						if (stealth == 0)	//무적이 아니라면
						{
							COMMAND_move = dir;//보고있는방향으로 앞으로 나가게, 떨어졌는데 가만히있진 않지요
							state = 6;//피격으로감
							//player.hurt();
							return;
						}
					}
					y = (obj->y - h);//발판위로 y좌표 세팅해주고

					if (LEFTkey == 0 && RIGHTkey == 0)	//근데 그와중에도 아무키도 안누르고있었다 ? 
						COMMAND_move = false;	//그럼 진행방향으로 가는걸 멈추도록해준다.
					else if (LEFTkey == 1 && RIGHTkey == 1)
						COMMAND_move = false;	//동시에 누르고있었어도 멈춰준다
					else if (LEFTkey == 1)	//하지만 뭔가를 누르고있었다?
						COMMAND_move = 1;
					else if (RIGHTkey == 1)			//그에맞춰바꿔준다
						COMMAND_move = 2;

					if (DOWNkey == true) {
						state = 3;	//숙이고있던 상태였다면 계속 숙이고있어줌
						y = y + 12;
						h = h - 12;	//계산이 끝났다면 다시 숙이기상태로 돌려줌
					}
					else state = 1;				//숙이던게 아니였으면 땅에부딪혔으니 정지상태해줌
					adjustspd = 0;			//떨어질때가속도를 위한거니 이것도 정지해줌
					COMMAND_hurt = 0;			//땅에 닿았으면 피격아님
					COMMAND_ropehurt = 0;		//땅에 닿았으면 피격아님

					if (ROWSPEED != 150)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
						ROWSPEED = 150;
				}

				if (obj->type == 4)
				{
					//x = x + beltspeed;
				}
				if (obj->type == 6)
				{
					//x = x - beltspeed;
				}
			}
			//return;			//하나라도 부딪혔다면 그대로 탈출한다
		}
	}

	if (check_coll != 0)
		return;	//하나라도 부딪혔다면 그대로 탈출
	if ((state == 4 || state == 1) || (state == 5 || state == 8))	//하나도 못부딪혔으면 공중에있는거니까 떨어져야한다
	{
		state = 7;
		//player.fall2save();		//떨어지는 순간의 x좌표점 기억
	}



}