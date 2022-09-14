#include "Client.h"
#include "../OBSTACLE/AttackObstacle.h"
#include "../Map.h"

#include <iostream>
#include <array>
#include "../Network.h"

Client::Client()
	:prev_size(0)
	,is_ingame(false)
	,is_active(false)
	, lobby_timer(11)
	,x(80),y(655),w(14),h(25),hp(100)
	,oldx(80),oldy(80)
	,savex(80),savey(655),state(1),dir(2),adjustspd(0)
	,COMMAND_move(false),COMMAND_hurt(false),COMMAND_ropehurt(false)
	,COMMAND_die(false),Gamemode(false),stealth(0),jumpignore(0)
	,spike_hurt(0),playername(""),rank(0),c_id(0),elapsedtime(0.f),mObjectCount(0)
{
	ZeroMemory(buf, sizeof(buf));
	

	//
	//bx = 0;
}


Client::~Client()
{
	closesocket(c_socket);
}


void Client::update(float delta_time)
{
	elapsedtime += delta_time;
	if (elapsedtime > 1)
	{
		if (SceneName == Scene_Name::SN_LOBBY)
		{
			LobbyCountDown();
		}
		elapsedtime = 0;
	}

	if (SceneName == Scene_Name::SN_INGAME)
	{
		oldx = x;
		oldy = y;
		move(delta_time);
		adjustPlayer(delta_time);
		spike_hurttime(delta_time);
		stealthtime(delta_time);
		vx = (x - oldx) / delta_time;
		vy = (y - oldy) / delta_time;
	}else if (SceneName == Scene_Name::SN_LOBBY)
	{
		oldx = x;
		oldy = y;
		move(delta_time);
		adjustPlayer(delta_time);
		vx = (x - oldx) / delta_time;
		vy = (y - oldy) / delta_time;
	}
}

void Client::ProcessPacket(unsigned char* p)
{
	unsigned char packet_type = p[1];
	switch (packet_type) {
	case CS_PACKET_LOGIN: {
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(p);

		
		//한 번 로그인 하면 더이상 로그인 패킷은 받지 않음.

		strcpy_s(playername, packet->username);
		x = 80;
		y = 665;

		mStageNum = 0;
		elapsedtime = 0;
		LobbyinitPos();

		//login Button 누른 플레이어는 여기 와서 비로소 active가 된다.
		is_ingame = true;
		//send_ok_packet me and other
		for (auto& c : CLIENTS)
		{
			if (c->is_ingame == false) continue;
			if (c->c_id == c_id)
			{
				sc_packet_login_ok packet;
				packet.size = sizeof(sc_packet_login_ok);
				packet.type = SC_PACKET_LOGIN_OK;
				packet.id = c_id;
				packet.x = x;
				packet.y = y;
				packet.stage = 1;
				c->do_send(&packet, sizeof(packet));
			}
			else {
				sc_packet_put_object packet;
				packet.size = sizeof(sc_packet_put_object);
				packet.type = SC_PACKET_PUT_OBJECT;
				packet.dir = dir;
				packet.h = h;
				packet.hp = hp;
				packet.id = c_id;
				packet.state = state;
				packet.stealth = stealth;
				strcpy_s(packet.username, 20, playername);
				packet.x = x;
				packet.y = y;
				packet.w = w;

				c->do_send(&packet, sizeof(packet));
			}
		}
		//send_ok_packet 상대방껄 나에게
		for (auto& c : CLIENTS)
		{
			if (c->is_ingame == false) continue;
			if (c->c_id == c_id)continue;

			sc_packet_put_object packet;
			packet.size = sizeof(sc_packet_put_object);
			packet.type = SC_PACKET_PUT_OBJECT;
			packet.dir = c->dir;
			packet.h = c->h;
			packet.hp = c->hp;
			packet.id = c->c_id;
			packet.state = c->state;
			packet.stealth = c->stealth;
			strcpy_s(packet.username, 20, c->playername);
			packet.x = c->x;
			packet.y = c->y;
			packet.w = c->w;
			do_send(&packet, sizeof(packet));
		}

		//여기서 드디어 로그인이 승인되고 하나의 Client로 인정받아, GameLogic의 Loop문을 실행하게 됨 .
		SceneName = Scene_Name::SN_LOBBY;
		Cnt_Player++;
		lobby_cnt++;
		
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
		if (COMMAND_die == true) break;	//서버에서 죽었으면 서버에서 더이상 move패킷 안받음. 
		switch ((int)packet->vk_key) {
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
	/*case CS_PACKET_ROBBY: {
		cs_packet_robby* packet = reinterpret_cast<cs_packet_robby*>(p);
		break;
	}*/
		
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


void Client::LobbyinitPos()
{
	x = 80;
	y = 655;
	savey = 655;
	w = 14;
	h = 25;
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
void Client::move(float deltatime)
{
	if (state == 1)
	{
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

		if (EndKey == true)
		{

			if (mStageNum == 1)
			{
				x = 40;
				y = 225;
			}
			if (mStageNum == 2)
			{
				x = 865;
				y = 205;
			}
			if (mStageNum == 3)
			{
				x = 14;
				y = 0;
			}
			else
			{
				EndKey = false;
			}
			EndKey = false;
		}
	}
	else if (state == 2)
	{
		if (COMMAND_hurt == true)
		{
			if (COMMAND_move == 1)
			{
				x -= (ROWSPEED * deltatime);
			}
			else if (COMMAND_move == 2)
			{
				x += (ROWSPEED * deltatime);
			}
			if (abs(y - savey) > 40) {
				y -= (3 * 50 * deltatime);
			}
			else {
				y -= (COLSPEED / 2 * deltatime);
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

				x -= (ROWSPEED * deltatime);
			}
			else if (COMMAND_move == 2)
			{
				x += (ROWSPEED * deltatime);
			}


			if (falldy > -5)
				falldy -= GroundAccel * deltatime * 50;
			if (falldy < 0)
			{
				state = 7;
			}
			y -= (falldy * deltatime * 60);
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

			if (COMMAND_move == 1)
			{
				x -= (ROWSPEED * deltatime);

			}
			else if (COMMAND_move == 2)
			{
				x += (ROWSPEED * deltatime);

			}
		}
	}
	else if (state == 5)
	{
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

	}
	else if (state == 7)
	{
		y += (COLSPEED * deltatime);
		if (adjustspd < 1000)
			adjustspd++;
		if (LEFTkey == true)
			if (adjustspd % 30 == 0)
				x -= (ROWSPEED * deltatime);
		if (RIGHTkey == true)
			if (adjustspd % 30 == 0)
				x += (ROWSPEED * deltatime);
		if (COMMAND_move == 1)
		{
			if (adjustspd <= 10)
				x -= (ROWSPEED * deltatime);
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x -= (ROWSPEED * deltatime);
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x -= (ROWSPEED * deltatime);
			}

			if (LEFTkey == 0)
				if (abs(x - savex) > 50)
					COMMAND_move = 0;

		}
		else if (COMMAND_move == 2)
		{
			if (adjustspd <= 10)
			{
				x += (ROWSPEED * deltatime);
			}
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x += (ROWSPEED * deltatime);
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x += (ROWSPEED * deltatime);
			}
			if (RIGHTkey == 0)
				if (abs(x - savex) > 50)
					COMMAND_move = 0;
		}
	}
	else if (state == 8)
	{
		savey = y;
		if (UDkey == true)
		{

		}
		else {


			if (COMMAND_move == 3)
			{
				y -= (ROPESPEED * deltatime);
			}
			else if (COMMAND_move == 4)
			{
				y += (ROPESPEED * deltatime);
			}
		}
	}


	//순위 판정-------------------------------------------------


	for (int i = 0; i < Cnt_Player; ++i)
	{



		if (CLIENTS[i]->mStageNum < 4)
		{
			mRank[i] = 1;
			for (int j = 0; j < Cnt_Player; ++j)
			{
				if (i == j)continue;

				if (CLIENTS[i]->mStageNum < CLIENTS[j]->mStageNum)
				{
					mRank[i] += 1;

				}
				if (CLIENTS[i]->mStageNum == CLIENTS[j]->mStageNum)
				{
					if (CLIENTS[i]->y > CLIENTS[j]->y)
						mRank[i] += 1;
				}
				if (CLIENTS[i]->mStageNum > CLIENTS[j]->mStageNum)
					mRank[j] += 1;


			}
		}

		CLIENTS[i]->rank = mRank[i];
		if (CLIENTS[i]->mStageNum == 4)
		{
			CLIENTS[i]->rank = mRank[i];
		}

	}
}
//오브젝트와 플레이어 충돌체크 1이면 부닥침
//----------------------------------------
bool Client::collp2o(Object* Obj)
{

	int adjust = 10;
	//왜 101이 먼저오냐면 발판보다는 장애물이 우선순위기때문임
	if (101 <= Obj->type && Obj->type < 301) { //장애물일때는 플레이어 네모빡스가 히트박스가된다
		if (Obj->type == 106 || Obj->type == 107)
		{
			if (x + w < Obj->x + Obj->mx || x - w > Obj->x + Obj->mx + Obj->w) return 0;
			if (y + h < Obj->y + Obj->my || y - h > Obj->y + Obj->my + Obj->h) return 0;
		}
		else
		{
			if (x + w < Obj->x || x - w > Obj->x + Obj->w) return 0;
			if (y + h < Obj->y || y - h > Obj->y + Obj->h) return 0;
		}


		return 1;
	}
	else if (301 <= Obj->type && Obj->type < 401)	//로프,밧줄같은 딱코 맞춰야하는 오브젝 위로는 플레이어 발까지 닿아야하고 아래로는 플레이어 중점에서 끝난다 하지만 내려갈수도 있어야하므로 조금 후하게 준다
	{

		if (y + h < Obj->y || y - h > Obj->y + Obj->h) return 0;	//일단먼저 닿았으면 들어와

		if (Obj->x < x && x < Obj->x + Obj->w)	//파이프가 그래도 좀 두꺼우니 이안에들어오면 cehck
		{
			if (UPkey == true)//여기는 특이하게 올라가면 올라가는쪽 체크는 끝이나야한다.
			{

				if (y + h <= Obj->y)	//올라갔을때 아랫키를 만족하면 충돌체크 x 안그러면 반응해서 계속 줄에매달리는 오류
					return 0;
				if (y < Obj->y + Obj->h)
					return 1;
			}
			else if (DOWNkey == true)
			{

				if (y + h <= Obj->y)
					return 1;
			}

			if (y + h <= Obj->y || y < Obj->y + Obj->h)
				return 1;
		}

		return 0;
	}
	else if (Obj->type == 1)	//땅바닥일때
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
			}
		}
	}

	return 0;
}



//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
void Client::adjustPlayer(float deltatime)
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
							stealth = 100;
							COMMAND_hurt = true;
							savey = y;
							state = 2;
							hurt();
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
					x += (beltspeed * deltatime);
				}
				if (obj->type == 6)
				{
					x -= (beltspeed * deltatime);
				}
			}
			else if (obj->type >= 101 && obj->type <= 200)	//장애물에 부딪히면
			{

				if (obj->type == 101)	//까시라면
				{
					if (stealth == 0)	//무적이 아니라면
					{
						if (state == 5 || state == 8)
						{
							COMMAND_ropehurt = 1;
						}
						if (state == 3) //숙이고있었다면
						{
							y = y - 12;
							h = h + 12;
							//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (state == 7)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (COMMAND_move == 1)
							{
								spike_hurt = -8;	//8번 왼쪽으로 감
							}
							else if (COMMAND_move == 2)
							{
								spike_hurt = 8;	//8번 오른쪽으로감
							}

							stealth = 100;	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							stealth = 100;
							COMMAND_hurt = true;
							savey = y;
							state = 2;
						}
						hurt();
					}
				}
				else if (obj->type == 102) //Break Pipe Left
				{
					if (state == 7) //떨어지는 중일때 부딪혔다 ?
					{
						y = obj->y - h;//발판위로 y좌표 세팅해주고

						if (LEFTkey == 0 && RIGHTkey == 0)	//근데 그와중에도 아무키도 안누르고있었다 ? 
							COMMAND_move = false;	//그럼 진행방향으로 가는걸 멈추도록해준다.
						else if (LEFTkey == 1 && RIGHTkey == 1)
							COMMAND_move = false;	//동시에 누르고있었어도 멈춰준다
						else if (LEFTkey == 1)	//하지만 뭔가를 누르고있었다?
							COMMAND_move = 1;
						else if (RIGHTkey == 1)			//그에맞춰바꿔준다
							COMMAND_move = 2;

						state = 1;				//그리고 땅에부딪혔으니 정지상태해줌
						adjustspd = 0;			//떨어질때가속도를 위한거니 이것도 정지해줌
						COMMAND_hurt = 0;			//땅에 닿았으면 피격아님
						COMMAND_ropehurt = 0;		//땅에 닿았으면 피격아님

						if (ROWSPEED != 150)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
							ROWSPEED = 150;
					}
					//X Collapse
					if (state == 1 || state == 4) //Walking Collpse
					{
						if (obj->y < y - h)
						{
							if (obj->x < x + w) //Left Collpse
							{
								x = obj->x - w;// x좌표 세팅해주고
								COMMAND_move = 0;//Don't Move
							}
						}
					}
				}
				else if (obj->type == 103) //왼쪽 증기, 가시와 비슷함 대신 증기가 완전히 뿜어져  나왔을때 피격판정이 있다.
				{
					auto steamobj = reinterpret_cast<AttackObstacle*>(obj);
					if (steamobj->index == 2) //증기가 완전히 뿜어졌을때만 피격이 발생한다
					{

						if (stealth == 0)
						{
							if (state == 5 || state == 8)
							{
								COMMAND_ropehurt = 1;
							}
							if (state == 3) //숙이고있었다면
							{
								y = y - 12;
								h = h + 12;	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
							}
							if (state == 7)
							{
								if (dir == 1 || dir == 2) //무조건 왼쪽으로감
								{
									spike_hurt = -8;
								}
								stealth = 100;
							}
							else {
								COMMAND_move = 1;//무조건 왼쪽임
								stealth = 100;
								COMMAND_hurt = true;
								savey = y;
								state = 2;
							}
							hurt();
						}
					}
				}
				else if (obj->type == 104) //Break Pipe Right
				{
				}
				else if (obj->type == 105) //Gas Right
				{
				}
				else if (obj->type == 106)
				{
					if (stealth == 0)	//무적이 아니라면
					{
						if (state == 5 || state == 8)
						{
							COMMAND_ropehurt = 1;
						}
						if (state == 3) //숙이고있었다면
						{
							y = y - 12;
							h = h + 12;
							//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (state == 7)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (dir == 1)
							{
								spike_hurt = -8;	//8번 왼쪽으로 감
							}
							else if (dir == 2)
							{
								spike_hurt = 8;	//8번 오른쪽으로감
							}

							stealth = 100;	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							COMMAND_move = dir;
							//
							//ROWSPEED *= 3;
							stealth = 100;
							COMMAND_hurt = true;
							savey = y;
							state = 2;
							//state = 6;		//피격으로감
						}
						hurt();
					}
				}
				else if (obj->type == 107)
				{
					if (stealth == 0)	//무적이 아니라면
					{
						if (state == 5 || state == 8)
						{
							COMMAND_ropehurt = 1;
						}
						if (state == 3) //숙이고있었다면
						{
							y = y - 12;
							h = h + 12;
							//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (state == 7)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (dir == 1)
							{
								spike_hurt = -8;	//8번 왼쪽으로 감
							}
							else if (dir == 2)
							{
								spike_hurt = -8;	//8번 오른쪽으로감
							}

							stealth = 100;	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							COMMAND_move = dir;
							stealth = 100;
							COMMAND_hurt = true;
							savey = y;
							state = 2;
						}
						hurt();
					}
				}
			}
			else if (obj->type >= 201 && obj->type <= 300) //플레이어와 상호작용하는 오브젝트 ex)포탈
			{
				if (obj->type == 201) //Portal
				{
					if (UPkey == true)
					{

						//내 상태를 다른 사람들에게 전달 (나랑 같은 stage에 있던사람한테 보냄)
						//이후 다른사람껄 지움. 
						for (int i = 0; i < Cnt_Player; ++i)
						{
							if (c_id == CLIENTS[i]->c_id)continue;
							if (mStageNum != CLIENTS[i]->mStageNum) continue;
							sc_packet_logout_object packet;
							packet.size = sizeof(sc_packet_logout_object);
							packet.type = SC_PACKET_LOGOUT_OBJECT;
							packet.id = c_id;
							CLIENTS[i]->do_send(&packet, sizeof(packet));

							sc_packet_logout_object mypacket;
							mypacket.size = sizeof(sc_packet_logout_object);
							mypacket.type = SC_PACKET_LOGOUT_OBJECT;
							mypacket.id = i;
							do_send(&mypacket, sizeof(mypacket));


						}

						//Stage의 변경 
						mStageNum += 1;
						initPos();
						sc_packet_portal packet;
						packet.size = sizeof(sc_packet_portal);
						packet.type = SC_PACKET_PORTAL;
						packet.stagenum = mStageNum;
						do_send(&packet, sizeof(packet));


						//다른사람들의 상태를 나한테 전달 (바뀐 stage에 있는 사람들걸 가져옴
						//이후 나도 다른사람들한테 보냄.
						for (int i = 0; i < Cnt_Player; ++i)
						{
							if (c_id == CLIENTS[i]->c_id)continue;
							if (mStageNum != CLIENTS[i]->mStageNum) continue;
							auto other = CLIENTS[i];

							sc_packet_put_object packet;
							packet.size = sizeof(sc_packet_put_object);
							packet.type = SC_PACKET_PUT_OBJECT;
							packet.id = other->c_id;
							packet.dir = other->dir;
							packet.h = other->h;
							packet.hp = other->hp;
							packet.state = other->state;
							packet.stealth = other->stealth;
							strcpy_s(packet.username, 20, other->playername);
							packet.w = other->w;
							packet.x = other->x;
							packet.y = other->y;
							do_send(&packet, sizeof(packet));

							sc_packet_put_object otherpacket;
							otherpacket.size = sizeof(sc_packet_put_object);
							otherpacket.type = SC_PACKET_PUT_OBJECT;
							otherpacket.id = c_id;
							otherpacket.dir = dir;
							otherpacket.h = h;
							otherpacket.hp = hp;
							otherpacket.state = state;
							otherpacket.stealth = stealth;
							strcpy_s(otherpacket.username, 20, playername);
							otherpacket.w = w;
							otherpacket.x = x;
							otherpacket.y = y;
							CLIENTS[i]->do_send(&otherpacket, sizeof(otherpacket));
						}
					}
				}
			}
			else if (obj->type == 301) //Rope
			{
				if (jumpignore <= 0)
				{
					if (COMMAND_ropehurt == 0)	//로프에서 맞으면 다시 로프 못탐
					{
						if (UPkey == true || DOWNkey == true)
						{
							if (DOWNkey == true && (state == 2 || state == 7))	//공중에있거나 점프중일때 아랫키로는 줄에 붙을수없다
								return;

							if (state != 5 && state != 8)	//줄에 매달려있지 않았다면 줄에 매달리는 상태를 만들어준다. 이미붙어있다면 해줄필요없음
							{
								state = 5;
								if (UPkey == true)
									COMMAND_move = 3;
								if (DOWNkey == true)
									COMMAND_move = 4;
								x = obj->x + (obj->w / 2);
								if (DOWNkey == true)	//이때는 수그리기아니라 밧줄 아래로 내려가는것이므로 수그리기로 깍인거 돌려준다
								{
									y = y - 12;
									h = 25;
								}
							}
						}
					}
				}
			}
			else if (obj->type == 0)
			{

			}
			if (ROWSPEED != 150)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
				ROWSPEED = 150;			//잠깐 위로 올려줬음 주석처리하고 ㅇㅇ 근데 이게 맞을거같긴해

		}
	}

	if (check_coll != 0)
		return;	//하나라도 부딪혔다면 그대로 탈출
	if ((state == 4 || state == 1) || (state == 5 || state == 8))	//하나도 못부딪혔으면 공중에있는거니까 떨어져야한다
	{
		state = 7;
	}



}

void Client::hurt()
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
		diecount++;
	}
}

void Client::spike_hurttime(float deltatime)
{
	if (spike_hurt < 0)
	{
		spike_hurt++;
		x -= (200 * deltatime);			//왼쪽으로감
	}
	else if (spike_hurt > 0)
	{
		spike_hurt--;
		x += (200 * deltatime);
	}
}

void Client::stealthtime(float deltatime)
{
	if (COMMAND_die == 0)	//죽으면 무적안풀림
		if (stealth > 0)
		{
			stealth = max(stealth - (100 * deltatime), 0);
			if (stealth == 0)
				COMMAND_hurt = 0;
		}
	if (jumpignore > 0)
	{
		jumpignore = max(jumpignore - (100 * deltatime), 0);
	}
}

void Client::LobbyCountDown()
{
	if (lobby_cnt == 3)
	{

		lobby_timer--;
		if (lobby_timer < 0) {
			lobby_timer = 0;

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

			for (auto& c : CLIENTS)
			{
				c->SceneChangeTrigger = true;
			}
		}

		for (int i = 0; i < lobby_cnt; ++i) {
			sc_packet_lobby packet;
			packet.size = sizeof(sc_packet_lobby);
			packet.type = SC_PACKET_LOBBY;
			packet.countdown = lobby_timer;

			CLIENTS[i]->do_send(&packet, sizeof(packet));

		}

	}
}