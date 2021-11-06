#include "Player.h"
#include "Network.h"

int ROWSPEED = 3; 	//���� �̵��ӵ�
int COLSPEED = 10;	//���� �̵��ӵ�
int ROPESPEED = 2;
bool LEFTkey = 0;//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool RIGHTkey = 0;//������Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool UPkey = 0;	//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool DOWNkey = 0;//�Ʒ���Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool LRkey = 0;//���ʿ�����Ű ���ÿ� �������� 1�̸鴭�� 0�̸� �ȴ���
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
		adjustspd = 0;		//�������� �� �Ĵ� 1���°� �ǹǷ� ���⼭ �ʱ�ȭ������
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
	else if (state == 2)	//���������϶��� ����������� �̵����ؾ��� 
	{
		if (COMMAND_hurt == true)	//�ǰݴ��Ѱ��
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
			if (abs((y - savey)) >= 40)	//40�ȼ���ŭ �ǰݴ��ؼ� ���� ��¦��
			{
				state = 7;			//�ٽ� ������ ����������
				savex = x;			//�̼����� x��ǥ�� �����(���ӵ��� �޴ٰ� �����ó�� ���ٿ���)
			}
		}
		else if (COMMAND_hurt == false)	//�Ϲݻ���
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
			���� �� ��¥ �ڵ带 ����������
			���ӵ� g�� �����ϴ� �ڵ�� ���� - 2021-07-05
			GroundAccel�� �߷°��ӵ��ε� �׳� 0.98������
			falldy�� �� �״�� �׳� dy�� �÷��̾��� y��ǥ y�� dy�� ��� �����ְ�
			��~���� ���� �ȵ����� ObjectManager���� ��� �浹�˻縦 ���ÿ� ���ָ鼭
			�ű⼭ �ǰ������� ������ ���⼭�� ��·�� y���� �������ָ� �ȴٴ� ��
			�ϵ� ������ § �ڵ�� ������ ���ݾ� �غ�������.
			*/
			if (falldy > -5)	//��ü�� �ִ볫�ϼӷ��� ������ ���� -10���� ���� -> defineȭ ���ѵ� �ɵ�
				falldy -= GroundAccel;	//�ӷ��� ���ӵ��� ���� ��� ��ȭ�ȴ�.
			if (falldy < 0)	//�ӷ��� -�� �Ǹ� ���������ִٴ� ��! �ڵ带 ������ ¥�� state�� �÷��̾� ���¸� ��ȭ������� �ϴ� �������� ������				
			{
				state = 7;	//���� �������� ������? ������ 1ȸ���ڵ���
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

		savey = y;	//�ٿ��Ŵ޷������� ���ڸ��� ���������̴�
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
		stealth = 100;		//�����ð� 2��
		savey = y;			//�ǰݰ� ���ÿ� y��ǥ����(������ �������� ����)
		COMMAND_hurt = true;	//�ǰ��Լ� on
		state = 2;				//�ǰ��ϸ� �������� �ѹ� �� ���

	}
	else if (state == 7)
	{
		y += COLSPEED;					//�Ʒ��� ������
		if (adjustspd < 1000)			//1000������ �������� ������
			adjustspd++;
		if (LEFTkey == true)			//�������� �� ���� �ڴ�����������
			if (adjustspd % 30 == 0)	//Ÿ�̸Ӱ� 30�� ���ư������� �ѹ��� �Ȱ���
				x -= ROWSPEED;
		if (RIGHTkey == true)
			if (adjustspd % 30 == 0)
				x += ROWSPEED;
		if (COMMAND_move == 1)		//�������� �����̰��ִٸ�
		{
			if (adjustspd <= 10)	//�������� �� ���ٰ�
			{
				x -= ROWSPEED;
			}
			if (adjustspd > 10)		//10�� ���� ������ 2���� �ѹ��� ����
			{
				if (adjustspd % 2 == 0)
					x -= ROWSPEED;
			}
			else if (adjustspd > 30)	//2���� 10�� �� ������ ���� 5���� 1���� ��� ���� �̰� �����ʵ� �Ȱ��� ����
			{
				if (adjustspd % 5 == 0)
					x -= ROWSPEED;
			}

			if (LEFTkey == 0)				//50ĭ������ ��ó�������� ���� , �����Ŀ� ����Ű�� ���������� ������ϰ� �ƴϸ� �������� �� ����
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
		savey = y;	//�ٿ��Ŵ޷������� ���ڸ��� ���������̴�
		//if (obj_t % 10 == 0)	//10�� Ÿ�̸� ���ư��� �ѹ� �����̰����ش�
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

				if (state == 1)//�����ִٰ� �����϶� �ѹ� �ٷ� �����������
				{

					state = 4;
					dir = 1;

				}
				else {
					state = 4;
					dir = 1;
				}
				COMMAND_move = 1;	//1�̵� 4�� ������ �ϴ��� ���������¸� �ٲ���
				//std::cout << "LEFT����" << std::endl;
			}
			else if (state == 2)
			{
				if (COMMAND_hurt != 1)	//�ĸ°��������� �� ���� �����ؿ�~
					ROWSPEED = 1;	//���������� ������ �ٲٷ��ϸ� ���ƽ�ϰ� �ٽ� ���°��� ������ �׷��� ���ߴ��ͺ��� ���� ����
				dir = 1;	//������ �ٲ������� ���������´� ����
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
			RIGHTkey = true;	//Ű ��������
			if (LEFTkey == true)
			{
				LRkey = true;
				if (state == 4)		//�����̰� �������� 1�� �ؼ� ���߰��ϴ°��� �ٸ��� 1�ιٲٸ� ������
					state = 1;
				return;
			}
			if (state == 7)
			{
				dir = 2;	//������ �ٲ������� ���������´� ���� (�Ʒ� move���� �����̻� ������ ���ݾ��� �ٲ������)
			}
			else if (state == 1 || state == 4)
			{
				if (state == 1)//�����ִٰ� �����϶� �ѹ� �ٷ� �����������
				{

					state = 4;
					dir = 2;
				}
				else {
					state = 4;
					dir = 2;

				}
				COMMAND_move = 2;	//1�̵� 4�� ������ �ϴ��� ���������¸� �ٲ���
				//std::cout << "RIGHT ����" << std::endl;
			}
			else if (state == 2)
			{
				if (COMMAND_hurt != 1)	//�ĸ°��������� �� ���� �����ؿ�~
					ROWSPEED = 1;	//���������� ������ �ٲٷ��ϸ� ���ƽ�ϰ� �ٽ� ���°��� ������ �׷��� ���ߴ��ͺ��� ���� ����
				dir = 2;	//������ �ٲ������� ���������´� ����
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

				if (state == 5)//�����ִٰ� �����϶� �ѹ� �ٷ� �����������
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

				state = 3;	//���̴°Ŵ� ������ �������� �����ϴ�
				h -= 12;		//���̸� Ű�� �پ�����Ѵ�.
				y += 12;
			}
			break;
		case 32: //VK_SPACE
			if (DOWNkey == true)//���׸��������� ��������
			{
				return;	//�ƹ��͵��������ʴ´� ����������
			}
			if (state == 5 || state == 8)//�ٿ� �Ŵ޷�����
			{
				if (LRkey == 0)		//���ÿ� �¿�Ű�� ������ �ʾ����鼭
				{
					if (LEFTkey == 1 || RIGHTkey == 1)	//���߿� �ϳ��� Ű�� �������־��ٸ� ������ ������ �ƴϸ� ����
					{
						COMMAND_move = dir;	//�׸��� �̶� ���ζ۰��� ������ ����
						jumpignore = 2;	//������ �ٽø���Ե� �ٲ���
					}
					else return;//�ƴϸ����
				}
				else return;//�ƴϸ� ����
			}
			if (state != 2 && state != 7)	//������ �����̾ƴ϶�� �����ۼ��ִ�. ������ �ٿ��Ŵ޷������� �ȵǱ� ��������
			{
				//Sound::GetSelf()->Sound_Play(EFFECTSOUND, JUMPEF, EFVOL);
				falldy = 10;	//�ӽ�
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