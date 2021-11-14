#include "Client.h"
#include "../CLIENT/LoginClient.h"
#include <iostream>


int ROWSPEED = 3; 	
int COLSPEED = 10;	
int ROPESPEED = 2;
bool LEFTkey = 0;
bool RIGHTkey = 0;
bool UPkey = 0;	
bool DOWNkey = 0;
bool LRkey = 0;
bool UDkey = 0;
int jumpcount = 0;
int diecount = 0;

Client::Client()
	:prev_size(0)
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

	//
	//bx = 0;
}
void Client::initBitPos()
{
	bx = 0;
	by = 0;
	bw = 0;
	bh = 0;
}

Client::~Client()
{

}

void Client::initPos()
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

//비트맵을 바꿔주는함수 (애니메이션)
void Client::BitMove()
{
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

void Client::move(int obj_t)
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

				x -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED;
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
				x -= ROWSPEED;
		if (RIGHTkey == true)
			if (adjustspd % 30 == 0)
				x += ROWSPEED;
		if (COMMAND_move == 1)		
		{
			if (adjustspd <= 10)	
			{
				x -= ROWSPEED;
			}
			if (adjustspd > 10)		
			{
				if (adjustspd % 2 == 0)
					x -= ROWSPEED;
			}
			else if (adjustspd > 30)	
			{
				if (adjustspd % 5 == 0)
					x -= ROWSPEED;
			}

			if (LEFTkey == 0)				
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
void Client::update(float delta_time)
{
	
}

void Client::ProcessPacket(unsigned char* p)
{
	unsigned char packet_type = p[1];
	switch (packet_type) {
	case CS_PACKET_LOGIN: {
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(p);
		strcpy_s(playername, packet->username);
		//send_login_ok_packet(c_id);
		break;
	}
	//jpark
	case CS_PACKET_MOVE: {
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);

		std::cout << (int)packet->dir << std::endl;

		switch ((int)packet->dir) {
		case 37: //VK_LEFT
			//std::cout << "left" << std::endl;
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
				if (state == 1)
				{
				state = 4;
					dir = 1;

				}
				else {
					state = 4;
					dir = 1;
				}
				COMMAND_move = 1;	
				
			}
			else if (state == 2)
			{
				if (COMMAND_hurt != 1)	
					ROWSPEED = 1;	
				dir = 1;	
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
			RIGHTkey = true;	
			if (LEFTkey == true)
			{
				LRkey = true;
				if (state == 4)		
					state = 1;
				return;
			}
			if (state == 7)
			{
				dir = 2;	
			}
			else if (state == 1 || state == 4)
			{
				if (state == 1)
				{

					state = 4;
					dir = 2;
				}
				else {
					state = 4;
					dir = 2;

				}
				COMMAND_move = 2;	
				
			}
			else if (state == 2)
			{
				if (COMMAND_hurt != 1)	
					ROWSPEED = 1;	
				dir = 2;	
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

				if (state == 5)
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

				state = 3;	
				h -= 12;		
				y += 12;
			}
			break;
		case 32: //VK_SPACE
			//std::cout << "space bar " << std::endl;
			if (DOWNkey == true)
			{
				return;
			}
			if (state == 5 || state == 8)
			{
				if (LRkey == 0)		
				{
					if (LEFTkey == 1 || RIGHTkey == 1)	
					{
						COMMAND_move = dir;
						jumpignore = 2;	
					}
					else return;
				}
				else return;
			}
			if (state != 2 && state != 7)	
			{
				//Sound::GetSelf()->Sound_Play(EFFECTSOUND, JUMPEF, EFVOL);
				falldy = 10;	
				jumpcount++;
				state = 2;
				savey = y;
			}
			break;

		case 47: //left keyUp
			if (RIGHTkey == true)		
			{
				dir = 2;
				if (state == 1)			
					COMMAND_move = 2;
			}
			else if (RIGHTkey == false)
			{
				if (state == 4)
				{
					state = 1;
					COMMAND_move = 0;	
				}
				else if (state == 1)	
				{
					COMMAND_move = 0;
				}
				if (DOWNkey == true)
				{
					if (state == 1)	
					{
						state = 3;
						h -= 12;
						y += 12;
					}
				}
			}


			LRkey = false;				
			LEFTkey = false;			
			break;
		case 49: //right keyUp
			if (LEFTkey == true)		
			{
				dir = 1;
				if (state == 1)			
					COMMAND_move = 1;
			}
			else if (LEFTkey == false)	
			{
				if (state == 4)
				{
					state = 1;
					COMMAND_move = 0;	
				}
				else if (state == 1)	
				{
					COMMAND_move = 0;
				}
				if (DOWNkey == true)
				{
					if (state == 1)	
					{
						state = 3;
						h -= 12;
						y += 12;
					}
				}
			}

			LRkey = false;				
			RIGHTkey = false;			
			break;
		case 48: //up keyUp
			if (DOWNkey == true)
			{
				if (state == 5)			
					COMMAND_move = 4;
			}
			else if (DOWNkey == false)
			{
				if (state == 8)
				{
					state = 5;
					COMMAND_move = 0;	
				}
			}

			UPkey = false;
			UDkey = false;
			break;
		case 50: //down keyDown
			if (UPkey == true)
			{
				if (state == 5)			
					COMMAND_move = 3;
			}
			else if (UPkey == false)	
			{
				if (state == 8)
				{
					state = 5;
					COMMAND_move = 0;	
				}
				else if (state == 5)	
				{
					COMMAND_move = 0;
				}
			}
			if (DOWNkey == true)
			{
				if (state == 3)
				{
					h += 12;
					y -= 12;	
					state = 1;
				}
			}

			UDkey = false;
			DOWNkey = false;
			break;
		}
		//case 42: //spacebar keyUp
		//	
		//	break;

	}
	}
}
int Client::do_recv()
{
	//버그가 있음. player buf에 저장된걸
	//새로받은 데이터랑 합쳐주지 않는 버그 존재.
	int received;
	char tmpbuf[MAX_BUF_SIZE];
	char* ptr = tmpbuf;
	received = recv(c_socket, ptr, MAX_BUF_SIZE, 0);
	if (received == SOCKET_ERROR)
	{
		return SOCKET_ERROR;
	}
	int remain_data = received + prev_size;
	unsigned char* packet_start = reinterpret_cast<unsigned char*>(ptr);
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

void Client::do_send(void* packet, int bytes)
{
	unsigned char buf[256];
	memcpy(buf, packet, bytes);
	send(c_socket, reinterpret_cast<char*>(buf), bytes, 0);
}