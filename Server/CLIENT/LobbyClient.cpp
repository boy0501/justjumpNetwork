#include <iostream>

#include "LobbyClient.h"
#include "../Map.h"
#include "../Object.h"


LobbyClient::LobbyClient()
	:elapsedtime(0)
	, obj_t(0)
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
		//std::cout << "로비클라" << robby_timer << "초" << std::endl;
		
		if (robby_cnt == 1)
		{
			robby_timer--;
			if (robby_timer < 0) {
				//mCss = CSS_DEAD;
				//mSn = SN_INGAME;
				//SetEvent(SceneChangeTrigger);
				robby_timer = 0;

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
				mStageNum = 1;

				mCss = CSS_DEAD;
				mSn = SN_INGAME;
				SetEvent(SceneChangeTrigger);
			}

			std::cout << robby_timer << std::endl;
			sc_packet_robby packet;
			packet.size = sizeof(sc_packet_robby);
			packet.type = SC_PACKET_ROBBY;
			packet.countdown = robby_timer;
			do_send(&packet, sizeof(packet));
		}
	}

	obj_t += 1;
	move(obj_t, delta_time);
	BitMove();
	//timer();
	//send packet
	sc_packet_empty packet;
	packet.size = sizeof(sc_packet_empty);
	packet.type = SC_PACKET_EMPTY;
	do_send(&packet, sizeof(packet));

	Client::update(delta_time);
}



//void timer()
//{
//
//	if (time == 10)
//	{
//		mCss = CSS_DEAD;
//		mSn = SN_INGAME;
//	}
//}
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
void LobbyClient::BitMove()
{
	//std::cout << "bitmove" << std::endl;
	bx += 1; //인덱스 형식으로 바꿈
	if (state == 4)
	{
		if (bx >= 5) bx = 1;
	}
	if (state == 5 || state == 8)
	{

		if (bx >= 2) bx = 0;
	}
}

void LobbyClient::move(int obj_t, float deltatime)
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

				x -= ROWSPEED * deltatime;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED * deltatime;
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

				x -= ROWSPEED * deltatime;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED * deltatime;
			}


			if (falldy > -5)
				falldy -= GroundAccel;
			if (falldy < 0)
			{
				std::cout << "state7로 change" << std::endl;
				state = 7;
			}
			y -= falldy;
			std::cout << "y = " << y << std::endl;
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
			if (obj_t % 5 == 0)
			{
				BitMove();
			}

			if (COMMAND_move == 1)
			{
				x -= ROWSPEED * deltatime;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED * deltatime;
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
		y += COLSPEED;
		//std::cout << y << std::endl;
		if (adjustspd < 1000)
			adjustspd++;
		if (LEFTkey == true)
			if (adjustspd % 30 == 0)
				x -= ROWSPEED * deltatime;
		if (RIGHTkey == true)
			if (adjustspd % 30 == 0)
				x += ROWSPEED * deltatime;
		if (COMMAND_move == 1)
		{
			if (adjustspd <= 10)
			{
				x -= ROWSPEED * deltatime;
			}
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x -= ROWSPEED * deltatime;
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x -= ROWSPEED * deltatime;
			}

			if (LEFTkey == 0)
				if (abs(x - savex) > 50)
					COMMAND_move = 0;

		}
		else if (COMMAND_move == 2)
		{
			if (adjustspd <= 10)
			{
				x += ROWSPEED * deltatime;
			}
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x += ROWSPEED * deltatime;
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x += ROWSPEED * deltatime;
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
		if (obj_t % 10 == 0)
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