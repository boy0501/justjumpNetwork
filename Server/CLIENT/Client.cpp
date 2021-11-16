#include "Client.h"
#include "../CLIENT/LoginClient.h"
#include <iostream>


Client::Client()
	:prev_size(0)
	,mCss(CSS_LIVE)
	,mSn(SN_LOGIN)
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
	SceneChangeTrigger = CreateEvent(NULL, FALSE, FALSE, NULL);
	SceneChangeIsDone = CreateEvent(NULL, FALSE, FALSE, NULL);

	//
	//bx = 0;
}


Client::~Client()
{
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

		//한 번 로그인 하면 더이상 로그인 패킷은 받지 않음.
		if (mSn == SN_LOBBY) break;

		strcpy_s(playername, packet->username);
		x = 80;
		y = 665;
		mStageNum = 0;

		mCss = CSS_DEAD;
		mSn = SN_LOBBY;
		SetEvent(SceneChangeTrigger);
		WaitForSingleObject(SceneChangeIsDone, INFINITE);

		break;
	}
	//jpark
	case CS_PACKET_MOVE: {
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);


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
					ROWSPEED = ROWSPEED/3;
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
					ROWSPEED = ROWSPEED/3;
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
		break;
	}
	case CS_PACKET_GAMEJOIN: {
		cs_packet_gamejoin* packet = reinterpret_cast<cs_packet_gamejoin*>(p);

		//한 번 게임접속하면 더이상 게임접속 패킷은 받지않음.
		if (mSn == SN_INGAME) break;

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
		WaitForSingleObject(SceneChangeIsDone, INFINITE);
		break;
	}
	}
}
int Client::do_recv()
{
	int received;
	char* ptr = reinterpret_cast<char*>(buf + prev_size);
	received = recv(c_socket, ptr, MAX_BUF_SIZE - prev_size, 0);
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