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
		std::cout << "�κ�Ŭ��" << std::endl;
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

//��Ʈ���� �ٲ��ִ��Լ� (�ִϸ��̼�)
//void LobbyClient::BitMove()
//{
//	//std::cout << "bitmove" << std::endl;
//	bx += 1; //�ε��� �������� �ٲ�
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

	//std::cout << "moveȣ��" << std::endl;
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
				//std::cout << "state7�� change" << std::endl;
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
		//std::cout << "state7����" << std::endl;
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



//������Ʈ�� �÷��̾� �浹üũ 1�̸� �δ�ħ
//----------------------------------------
bool LobbyClient::collp2o(Object* Obj)
{

	int adjust = 10;	//���� �β�
	if (Obj->type == 1)	//���ٴ��϶�
	{
		if (Obj->x <= x && x <= Obj->x + Obj->w)
		{
			if (Obj->y <= y + h)
			{
				return 1;
			}
		}
	}
	else if (Obj->type <= 100) {	//�÷����϶��� �÷��̾� ������ ��Ʈ�ڽ����ȴ�
		if (Obj->x <= x && x <= Obj->x + Obj->w)
		{
			if (Obj->y <= y + h && y + h <= Obj->y + adjust)
			{
				return 1;
				//����
			}
		}
	}

	return 0;
}



//�÷��̾�� ������Ʈ�� ��ȣ�ۿ� �Ǵ��ϰ� �׿��°� �ٲ���
void LobbyClient::adjustPlayer()
{
	int check_coll = 0;	//�ϳ��� �ε������� �Ǻ��ϱ�����
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
			//std::cout << "�ε���" << std::endl;
			check_coll++;	//�ϳ��� �ε������� coll�� �ö�
			if (obj->type < 101 && obj->type > 0)			//�ٵ� �װ� ���ٴ��̿���?
			{

				if (state == 7) //�������� ���϶� �ε����� ?
				{
					if (abs(savey - y) > 200)	//�������޾ƾ��Ѵٸ�
					{
						if (stealth == 0)	//������ �ƴ϶��
						{
							COMMAND_move = dir;//�����ִ¹������� ������ ������, �������µ� ���������� ������
							state = 6;//�ǰ����ΰ�
							//player.hurt();
							return;
						}
					}
					y = (obj->y - h);//�������� y��ǥ �������ְ�

					if (LEFTkey == 0 && RIGHTkey == 0)	//�ٵ� �׿��߿��� �ƹ�Ű�� �ȴ������־��� ? 
						COMMAND_move = false;	//�׷� ����������� ���°� ���ߵ������ش�.
					else if (LEFTkey == 1 && RIGHTkey == 1)
						COMMAND_move = false;	//���ÿ� �������־�� �����ش�
					else if (LEFTkey == 1)	//������ ������ �������־���?
						COMMAND_move = 1;
					else if (RIGHTkey == 1)			//�׿�����ٲ��ش�
						COMMAND_move = 2;

					if (DOWNkey == true) {
						state = 3;	//���̰��ִ� ���¿��ٸ� ��� ���̰��־���
						y = y + 12;
						h = h - 12;	//����� �����ٸ� �ٽ� ���̱���·� ������
					}
					else state = 1;				//���̴��� �ƴϿ����� �����ε������� ������������
					adjustspd = 0;			//�����������ӵ��� ���ѰŴ� �̰͵� ��������
					COMMAND_hurt = 0;			//���� ������� �ǰݾƴ�
					COMMAND_ropehurt = 0;		//���� ������� �ǰݾƴ�

					if (ROWSPEED != 150)		//ROWSPEED�� ���Ƿ� �ٲ��־��ٸ� ���� ������ �ʱ�ȭ�� ������� �����ش�
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
			//return;			//�ϳ��� �ε����ٸ� �״�� Ż���Ѵ�
		}
	}

	if (check_coll != 0)
		return;	//�ϳ��� �ε����ٸ� �״�� Ż��
	if ((state == 4 || state == 1) || (state == 5 || state == 8))	//�ϳ��� ���ε������� ���߿��ִ°Ŵϱ� ���������Ѵ�
	{
		state = 7;
		//player.fall2save();		//�������� ������ x��ǥ�� ���
	}



}