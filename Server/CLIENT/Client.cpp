﻿#include "Client.h"
#include "../CLIENT/LoginClient.h"
#include "../CLIENT/LobbyClient.h"

#include <iostream>
#include "../Network.h"

Client::Client()
	:prev_size(0)
	,mCss(CSS_LIVE)
	,mSn(SN_LOGIN)
	,is_ingame(false)
	,is_active(false)
{
	ZeroMemory(buf, sizeof(buf));
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
	case CS_PACKET_DIEOK: {
		cs_packet_die_ok* packet = reinterpret_cast<cs_packet_die_ok*>(p);

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

		hp = 100;

		break;
	}
	case CS_PACKET_MOVE: {
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);
		if (c_id != packet->id) break;
		switch ((int)packet->dir) {
		case VK_LEFT: //VK_LEFT
			//std::cout << "left" << std::endl;
			LEFTkey = true;
			if (RIGHTkey == true)
			{
				LRkey = true;
				if (state == 4)
					state = 1;

				break;
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
					ROWSPEED = 150;
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
		case VK_RIGHT: //VK_RIGHT
			RIGHTkey = true;
			if (LEFTkey == true)
			{
				LRkey = true;
				if (state == 4)
					state = 1;
				break;
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
					ROWSPEED = 150;
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
		case VK_UP: //VK_UP
			UPkey = true;

			if (DOWNkey == true)
			{
				UDkey = true;
				if (state == 8)
					state = 5;
				break;
			}
			if (state == 5 || state == 8)
			{
				state = 8;
				COMMAND_move = 3;

			}
			break;
		case VK_DOWN: //VK_DOWN
			if (COMMAND_hurt == 1)
			{
				break;
			}
			DOWNkey = true;
			if (UPkey == true)
			{
				UDkey = true;
				if (state == 8)
					state = 5;
				break;
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
		case VK_SPACE: //VK_SPACE
			if (DOWNkey == true)
			{
				break;
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
					else {
						break; 
					}
				}
				else {
					break;
				}
			}
			if (state != 2 && state != 7)
			{
				falldy = 10;
				jumpcount++;
				state = 2;
				savey = y;
			}
			break;
		case VK_END:	//치트키
			EndKey = true;
			break;
		default:
			break;
		}
		break;
	}
	case CS_PACKET_KEYUP: {
		cs_packet_keyup* packet = reinterpret_cast<cs_packet_keyup*>(p);
		switch (packet->vk_key)
		{
		case VK_LEFT: //left keyUp
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
		case VK_RIGHT: //right keyUp
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
		case VK_UP: //up keyUp
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
		case VK_DOWN: //down keyDown
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
		break;
	}
	case CS_PACKET_ROBBY: {
		cs_packet_robby* packet = reinterpret_cast<cs_packet_robby*>(p);
		break;
	}

	//case CS_PACEKT_LOGOUT: {
	//	cs_packet_logout* packet = reinterpret_cast<cs_packet_logout*>(p);
	//	//std::cout << "값은: " << (int)packet->out << std::endl;
	//	if ((int)packet->out == 2) {
	//		is_logout = true;
	//	}

	//	break;
	//}
		
	}

}
int Client::do_recv()
{
	int received;
	char* ptr = reinterpret_cast<char*>(buf + prev_size);
	received = recv(c_socket, ptr, MAX_BUF_SIZE - prev_size, 0);
	if (received == SOCKET_ERROR)
	{
		//jpark logout 작업 수정
		is_logout = true;
		is_active = false;

		switch (c_id)
		{
		case 0: {
			if (CLIENTS[1]->is_active == true)
				SetEvent(Client1Event);
			else
				SetEvent(Client2Event);
			break;
		}
		case 1: {
			if (CLIENTS[2]->is_active == true)
				SetEvent(Client2Event);
			else
				SetEvent(Client0Event);

			break;
		}
		case 2: {
			if (CLIENTS[0]->is_active == true)
				SetEvent(Client0Event);
			else
				SetEvent(Client1Event);
			break;
		}
		}

		for (int i = 0; i < Cnt_Player; ++i) {

			auto& c = CLIENTS[i];
			if (c->is_logout == true) {

				for (int i = 0; i < Cnt_Player; ++i)
				{
					sc_packet_logout_object packet;
					packet.size = sizeof(sc_packet_logout_object);
					packet.type = SC_PACKET_LOGOUT_OBJECT;
					packet.id = c->c_id;
					CLIENTS[i]->do_send(&packet, sizeof(packet));
				}
			}
		}
		
		return SOCKET_ERROR;
	}
	int remain_data = received + prev_size;
	unsigned char* packet_start = reinterpret_cast<unsigned char*>(buf);
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