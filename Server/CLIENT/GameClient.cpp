#include <iostream>

#include "../Map.h"
#include "GameClient.h"
#include "../Object.h"


GameClient::GameClient()
	:elapsedtime(0)
	,obj_t(0)
{

}


GameClient::~GameClient()
{

}

void GameClient::update(float delta_time)
{
	elapsedtime += delta_time;
	if (elapsedtime > 1)
	{
		elapsedtime = 0;
		std::cout << "����Ŭ��" << std::endl;
	}

	adjustPlayer();

	obj_t += 1;
	move(obj_t, delta_time);
	BitMove();

	//send packet
	sc_packet_move_process packet;
	packet.size = sizeof(sc_packet_move_process);
	packet.type = SC_PACKET_MOVE_PROCESS;
	packet.dir = dir;
	packet.h = h;
	packet.id = c_id;
	packet.state = state;
	packet.stealth = stealth;
	packet.x = x;
	packet.y = y;
	do_send(&packet, sizeof(packet));

	Client::update(delta_time);
}


void GameClient::initPos()
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
void GameClient::initBitPos()
{
	bx = 0;
	by = 0;
	bw = 0;
	bh = 0;
}

//��Ʈ���� �ٲ��ִ��Լ� (�ִϸ��̼�)
void GameClient::BitMove()
{
	//std::cout << "bitmove" << std::endl;
	bx += 1; //�ε��� �������� �ٲ�
	if (state == 4)
	{
		if (bx >= 5) bx = 1;
	}
	if (state == 5 || state == 8)
	{

		if (bx >= 2) bx = 0;
	}
}

void GameClient::move(int obj_t, float deltatime)
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
				falldy -= GroundAccel;
			if (falldy < 0)
			{
				//std::cout << "state7�� change" << std::endl;
				state = 7;
			}
			y -= falldy;
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
		y += COLSPEED;
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
//������Ʈ�� �÷��̾� �浹üũ 1�̸� �δ�ħ
//----------------------------------------
bool GameClient::collp2o(Object* Obj)
{
	
	int adjust = 10;
	//�� 101�� �������ĸ� ���Ǻ��ٴ� ��ֹ��� �켱�����⶧����
	if (101 <= Obj->type && Obj->type < 301) { //��ֹ��϶��� �÷��̾� �׸������ ��Ʈ�ڽ����ȴ�
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
	else if (301 <= Obj->type && Obj->type < 401)	//����,���ٰ��� ���� ������ϴ� ������ ���δ� �÷��̾� �߱��� ��ƾ��ϰ� �Ʒ��δ� �÷��̾� �������� ������ ������ ���������� �־���ϹǷ� ���� ���ϰ� �ش�
	{

		if (y + h < Obj->y || y - h > Obj->y + Obj->h) return 0;	//�ϴܸ��� ������� ����

		if (Obj->x < x && x < Obj->x + Obj->w)	//�������� �׷��� �� �β���� �̾ȿ������� cehck
		{
			if (UPkey == true)//����� Ư���ϰ� �ö󰡸� �ö󰡴��� üũ�� ���̳����Ѵ�.
			{

				if (y + h <= Obj->y)	//�ö����� �Ʒ�Ű�� �����ϸ� �浹üũ x �ȱ׷��� �����ؼ� ��� �ٿ��Ŵ޸��� ����
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
	else if (Obj->type == 1)	//���ٴ��϶�
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
void GameClient::adjustPlayer()
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
						COMMAND_move = 1 ;
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
			else if (obj->type >= 101 && obj->type <= 200)	//��ֹ��� �ε�����
			{

				if (obj->type == 101)	//��ö��
				{
					if (stealth == 0)	//������ �ƴ϶��
					{
						if (state == 5 || state == 8)
						{
							COMMAND_ropehurt = 1;
						}
						if (state == 3) //���̰��־��ٸ�
						{
							y = y - 12;
							h = h + 12;
							//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (state == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (COMMAND_move == 1)
							{
								spike_hurt = -8;	//8�� �������� ��
							}
							else if (COMMAND_move == 2)
							{
								spike_hurt = 8;	//8�� ���������ΰ�
							}

							stealth = 100;	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							state = 6;		//�ǰ����ΰ�
						}
						//player.hurt();
					}
				}
				else if (obj->type == 102) //Break Pipe Left
				{
					if (state == 7) //�������� ���϶� �ε����� ?
					{
						y = obj->y - h;//�������� y��ǥ �������ְ�

						if (LEFTkey == 0 && RIGHTkey == 0)	//�ٵ� �׿��߿��� �ƹ�Ű�� �ȴ������־��� ? 
							COMMAND_move = false;	//�׷� ����������� ���°� ���ߵ������ش�.
						else if (LEFTkey == 1 && RIGHTkey == 1)
							COMMAND_move = false;	//���ÿ� �������־�� �����ش�
						else if (LEFTkey == 1)	//������ ������ �������־���?
							COMMAND_move = 1;
						else if (RIGHTkey == 1)			//�׿�����ٲ��ش�
							COMMAND_move = 2;

						state = 1;				//�׸��� �����ε������� ������������
						adjustspd = 0;			//�����������ӵ��� ���ѰŴ� �̰͵� ��������
						COMMAND_hurt = 0;			//���� ������� �ǰݾƴ�
						COMMAND_ropehurt = 0;		//���� ������� �ǰݾƴ�

						if (ROWSPEED != 150)		//ROWSPEED�� ���Ƿ� �ٲ��־��ٸ� ���� ������ �ʱ�ȭ�� ������� �����ش�
							ROWSPEED = 150;
					}
					//X Collapse
					if (state == 1 || state == 4) //Walking Collpse
					{
						if (obj->y < y - h)
						{
							if (obj->x < x + w) //Left Collpse
							{
								x = obj->x - w;// x��ǥ �������ְ�
								COMMAND_move = 0;//Don't Move
							}
						}
					}
				}
				else if (obj->type == 103) //���� ����, ���ÿ� ����� ��� ���Ⱑ ������ �վ���  �������� �ǰ������� �ִ�.
				{
					if (obj->index == 2) //���Ⱑ ������ �վ��������� �ǰ��� �߻��Ѵ�
					{
						if (stealth == 0)
						{
							if (state == 5 || state == 8)
							{
								COMMAND_ropehurt = 1;
							}
							if (state == 3) //���̰��־��ٸ�
							{
								y = y - 12;
								h = h + 12;	//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
							}
							if (state == 7)
							{
								if (dir == 1 || dir == 2) //������ �������ΰ�
								{
									spike_hurt = -8;
								}
								stealth = 100;
							}
							else {
								COMMAND_move = 1;//������ ������
								state = 6;
							}
							//player.hurt();
						}
					}
				}
				else if (obj->type == 104) //Break Pipe Right
				{
					//Update Cooming Soon
				}
				else if (obj->type == 105) //Gas Right
				{
					//Update Cooming Soon
				}
				else if (obj->type == 106)
				{
					if (stealth == 0)	//������ �ƴ϶��
					{
						if (state == 5 || state == 8)
						{
							COMMAND_ropehurt = 1;
						}
						if (state == 3) //���̰��־��ٸ�
						{
							y = y - 12;
							h = h + 12;
							//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (state == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (dir == 1)
							{
								spike_hurt = -8;	//8�� �������� ��
							}
							else if (dir == 2)
							{
								spike_hurt = 8;	//8�� ���������ΰ�
							}

							stealth = 100;	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							COMMAND_move = dir;
							state = 6;		//�ǰ����ΰ�
						}
						//player.hurt();
					}
				}
				else if (obj->type == 107)
				{
					if (stealth == 0)	//������ �ƴ϶��
					{
						if (state == 5 || state == 8)
						{
							COMMAND_ropehurt = 1;
						}
						if (state == 3) //���̰��־��ٸ�
						{
							y = y - 12;
							h = h + 12;
							//������� �������� �����Ѵ�. �̰� ���� ���������� �ٽõ����ش�
						}
						if (state == 7)//�Ϲ��϶��� ��¦ ���� �ٵ��� ���µ� �����������̸� ��¦�� �̵��Ѵ�
						{
							if (dir == 1)
							{
								spike_hurt = -8;	//8�� �������� ��
							}
							else if (dir == 2)
							{
								spike_hurt = -8;	//8�� ���������ΰ�
							}

							stealth = 100;	//�����ð� �־��� (�̵��ϴ·����� state==7 �϶� �˾Ƽ� �ٷ���
						}
						else {
							COMMAND_move = dir;
							state = 6;		//�ǰ����ΰ�
						}
						//player.hurt();
					}
				}
			}
			else if (obj->type >= 201 && obj->type <= 300) //�÷��̾�� ��ȣ�ۿ��ϴ� ������Ʈ ex)��Ż
			{
				if (obj->type == 201) //Portal
				{
					if (UPkey == true)
					{
						//���� �ٲ�� ���� 

						//m.setblack_t(50);
						///*m.CreateBlack(g_hinst);*/
						//m.setmapnum(m.getmapnum() + 1);
						////saveMapNum = m.getmapnum();
						//player.initPos();
						//if (m.getmapnum() == 13) {
						//
						//	m.CreateMap(g_hinst);
						//
						//}
						//
						//for (int j = 0; j < mObjectCount; j++)
						//	obj[j].ResetObject();
						//mObjectCount = initObject(obj, m.getmapnum(), g_hinst);
						//m.CreateMap(g_hinst);
						//
						//
						//Sound::GetSelf()->setindex(m.getmapnum() - 9);
						//Sound::GetSelf()->Sound_Play(BGMSOUND, Sound::GetSelf()->getindex(), BGMVOL);
						//Sound::GetSelf()->Sound_Play(EFFECTSOUND, PORTALEF, EFVOL);
						//
						//return;
					}
				}
			}
			else if (obj->type == 301) //Rope
			{
				if (jumpignore <= 0)
				{
					if (COMMAND_ropehurt == 0)	//�������� ������ �ٽ� ���� ��Ž
					{
						if (UPkey == true || DOWNkey == true)
						{
							if (DOWNkey == true && (state == 2 || state == 7))	//���߿��ְų� �������϶� �Ʒ�Ű�δ� �ٿ� ����������
								return;

							if (state != 5 && state != 8)	//�ٿ� �Ŵ޷����� �ʾҴٸ� �ٿ� �Ŵ޸��� ���¸� ������ش�. �̹̺پ��ִٸ� �����ʿ����
							{
								state = 5;
								if (UPkey == true)
									COMMAND_move = 3;
								if (DOWNkey == true)
									COMMAND_move = 4;
								x = obj->x + (obj->w / 2);
								if (DOWNkey == true)	//�̶��� ���׸���ƴ϶� ���� �Ʒ��� �������°��̹Ƿ� ���׸���� ���ΰ� �����ش�
								{
									y = y - 12;
									h = 25;
								}
							}
							//player.BitMove();
						}
					}
				}
			}
			else if (obj->type == 0)
			{

			}
			if (ROWSPEED != 150)		//ROWSPEED�� ���Ƿ� �ٲ��־��ٸ� ���� ������ �ʱ�ȭ�� ������� �����ش�
				ROWSPEED = 150;			//��� ���� �÷����� �ּ�ó���ϰ� ���� �ٵ� �̰� �����Ű�����

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