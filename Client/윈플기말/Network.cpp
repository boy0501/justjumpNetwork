
#include "Network.h"

#include "../../Protocol/protocol.h"
#include "player.h"
Network* Network::mNetwork = nullptr;

void error_display(int err_no)
{
	wchar_t* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf << std::endl;
	//while (true);
	LocalFree(lpMsgBuf);
}
Network::Network()
	: mPlayer(nullptr)
	, prev_size(0)
{
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

}


Network::~Network()
{
	closesocket(s_socket);
	WSACleanup();
	delete mNetwork;
}


Network* Network::GetNetwork()
{
	if (mNetwork == nullptr)
		mNetwork = new Network();
	return mNetwork;
}


void Network::ConnectServer()
{
	int ret = connect(s_socket, (SOCKADDR*)(&server_addr), sizeof(server_addr));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "�������" << std::endl;
		exit(0);
		
	}
}

int Network::C_Recv()
{
	//std::cout << "recv��!" << std::endl;
	int received;
	char tmpbuf[MAX_BUF_SIZE];
	char* ptr = tmpbuf;
	received = recv(s_socket, ptr, MAX_BUF_SIZE, 0);
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

void draw()
{

}

HWND hWnd2;

void Network::ProcessPacket(unsigned char* p)
{
	unsigned char packet_type = p[1];
	switch (packet_type) {
	case SC_PACKET_LOGIN_OK: {
		sc_packet_login_ok* packet = reinterpret_cast<sc_packet_login_ok*>(p);
		mPlayer->player_cid = packet->id;
		//send_login_ok_packet(c_id);
		break;
	}
	case SC_PACKET_ROBBY: {
		sc_packet_robby* packet = reinterpret_cast<sc_packet_robby*>(p);
		/*if ((int)(packet->count_start) == 1) {
			mPlayer->ready_to_go = true;
		}*/
		if (packet->player_cnt == 1) {
			mPlayer->ready_to_go = true;

		}
	}
	case SC_PACKET_MOVE_PROCESS: {
		sc_packet_move_process* packet = reinterpret_cast<sc_packet_move_process*>(p);
		
		//player draw�Լ�===========================================================================
		BLENDFUNCTION bf;
		bf.AlphaFormat = 0;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = 255;

		mPlayer->hdc = GetDC(hWnd2);
		mPlayer->mem1dc = CreateCompatibleDC(mPlayer->hdc);


		mPlayer->pdc = CreateCompatibleDC(mPlayer->mem1dc);
		//�ǰݴ������ÿ� ����ó�� ���� dc�� mem1dc�� ����
		HDC gdidc = CreateCompatibleDC(mPlayer->mem1dc);
		//mem1dc�� ĳ���ͱ׸�������ŭ�� ���´�(���� mem1dc���� ����������Ƿ� 0,0 ���� 62,50 ������ ��Ʈ���� ��)
		HBITMAP tmpdc = CreateCompatibleBitmap(mPlayer->mem1dc, 62, 50);
		HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
		//���⼭ 0,0 ~62,50 ������ ��Ʈ���� ĳ���ͱ������� �ٲ��ش� (�÷��̾ �ִ� ��ġ�� ��Ʈ���� ������)
		BitBlt(gdidc, 0, 0, mPlayer->charw * 2, packet->h * 2, mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, SRCCOPY);
		//�⺻ ������
		SelectObject(mPlayer->pdc, mPlayer->hbitcur);
		//pdc�� hbitcur �� sprite�� �������
		if (packet->state == 1) // �������� 
		{

			if (packet->dir == 1)//����
			{
				//TransparentBlt(gdidc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 0, 62, 50, RGB(255, 255, 255));
				//gdidc�� 0,0~ 62,50 �̴ϱ� �� ��ġ�� ������ ĳ���͸� ��������ְ� GdialphaBlend �� ���� ����ȭó�� ���ش�.
				TransparentBlt(gdidc, 0, 0, 62, 50, mPlayer->pdc, 0, 0, 62, 50, RGB(255, 255, 255));

				//2021-10-04 �ּ� �߰�
				//���� Ǯ� ���ϸ�, �ᱹ ���������δ� �츮�� mem1dc(������۸��� hdc)�� �׷�����Ѵ�. 
				//pdc���� sprite�̹����� �ְ�(857line���� ���ش�), sprite���� �ʿ��� �κи� ©�� gdidc�� �׷��� ����
				//gdialphablend�� gdidc���� alpha���� �����Ѵ��� mem1dc�� ���������� �׷��ִ°��̴�.

				if (packet->stealth > 0)
				{

					bf.SourceConstantAlpha = 155;
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
					bf.SourceConstantAlpha = 255;

				}
				else
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
			}
			else if (packet->dir == 2)//������
			{
				//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
				TransparentBlt(gdidc, 0, 0, 62, 50, mPlayer->pdc, 0, 50, 62, 50, RGB(255, 255, 255));
				if (packet->stealth > 0)
				{

					bf.SourceConstantAlpha = 155;//����
					//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
					bf.SourceConstantAlpha = 255;

				}
				else
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
			}

		}
		else if (packet->state == 4) //�̵�����
		{
			if (packet->dir == 1)//����
			{
				//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by, bw, bh, RGB(255, 255, 255)); //68 0 130 50
				TransparentBlt(gdidc, 0, 0, 62, 50, mPlayer->pdc, mPlayer->bx * 68, mPlayer->by, mPlayer->bw, mPlayer->bh, RGB(255, 255, 255));
				if (packet->stealth > 0)
				{

					bf.SourceConstantAlpha = 155;//����
					//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
					bf.SourceConstantAlpha = 255;

				}
				else
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
			}
			else if (packet->dir == 2)//������
			{
				//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by + 50, bw, bh, RGB(255, 255, 255));
				TransparentBlt(gdidc, 0, 0, 62, 50, mPlayer->pdc, mPlayer->bx * 68, mPlayer->by + 50, mPlayer->bw, mPlayer->bh, RGB(255, 255, 255));
				if (packet->stealth > 0)
				{

					bf.SourceConstantAlpha = 155;//����
					//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
					bf.SourceConstantAlpha = 255;

				}
				else
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
			}


		}
		else if (packet->state == 2 ||packet->state == 7) //�����ϰų� ��������
		{
			if (packet->dir == 1)//����
			{
				//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 107, 62, 48, RGB(255, 255, 255)); //68 0 130 50
				TransparentBlt(gdidc, 0, 0, 62, 50, mPlayer->pdc, 0, 107, 62, 50, RGB(255, 255, 255));
				if (packet->stealth > 0)
				{

					bf.SourceConstantAlpha = 155;//����
					//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
					bf.SourceConstantAlpha = 255;

				}
				else
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
			}
			else if (packet->dir == 2)//������
			{
				//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
				TransparentBlt(gdidc, 0, 0, 62, 50, mPlayer->pdc, 77, 107, 62, 48, RGB(255, 255, 255));
				if (packet->stealth > 0)
				{

					bf.SourceConstantAlpha = 155;//����
					//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
					bf.SourceConstantAlpha = 255;

				}
				else
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
			}
		}
		else if (packet->state == 3) //���̱�
		{
			//h�� �ٰ� y�� �ð� 

			BitBlt(gdidc, 0, 0, mPlayer->charw * 2, 26, mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, SRCCOPY);
			if (packet->dir == 1)//����
			{
				//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 161, 62, 26, RGB(255, 255, 255)); //68 0 130 50
				TransparentBlt(gdidc, 0, 0, 62, 26, mPlayer->pdc, 0, 161, 62, 26, RGB(255, 255, 255));
				if (packet->stealth > 0)
				{

					bf.SourceConstantAlpha = 155;//����
					//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - 12 - packet->h + 12, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 26, bf);
					bf.SourceConstantAlpha = 255;

				}
				else
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - 12 - packet->h + 12, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 26, bf);
			}
			else if (packet->dir == 2)//������
			{
				//	TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
				TransparentBlt(gdidc, 0, 0, 62, 26, mPlayer->pdc, 77, 161, 62, 26, RGB(255, 255, 255));
				if (packet->stealth > 0)
				{

					bf.SourceConstantAlpha = 155;//����
					//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 26, bf);
					bf.SourceConstantAlpha = 255;

				}
				else
					GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 26, bf);
			}
		}
		else if (packet->state == 5 || packet->state == 8)	//�ٿ� �Ŵ޸�����
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, mPlayer->pdc, mPlayer->bx * 77, 54, 62, 50, RGB(255, 255, 255));

			if (packet->stealth > 0)
			{

				bf.SourceConstantAlpha = 155;
				GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else GdiAlphaBlend(mPlayer->mem1dc, packet->x - mPlayer->charw, packet->y - packet->h, mPlayer->charw * 2, packet->h * 2, gdidc, 0, 0, 62, 50, bf);

		}

		//�г��� �׷��ֱ�
		HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮 bold"));
		HFONT oldfont = (HFONT)SelectObject(mPlayer->mem1dc, hfont);
		SetBkMode(mPlayer->mem1dc, TRANSPARENT);
		SetTextColor(mPlayer->mem1dc, RGB(255, 108, 168));
		RECT rt{ packet->x - 60,packet->y + 25,packet->x + 60,packet->y + 65 };
		DrawText(mPlayer->mem1dc, mPlayer->mPlayerwname.c_str(), lstrlenW(mPlayer->mPlayerwname.c_str()), &rt, DT_CENTER);

		SelectObject(mPlayer->mem1dc, oldfont);
		DeleteObject(hfont);


		SelectObject(gdidc, oldtmpdc);
		DeleteObject(tmpdc);
		DeleteObject(gdidc);
		DeleteObject(mPlayer->pdc);
		break;
	}
	}
	
}

void Network::C_Send(void* packet, int bytes)
{
	unsigned char buf[256];
	memcpy(buf, packet, bytes);
	send(s_socket, reinterpret_cast<char*>(buf), bytes, 0);
}