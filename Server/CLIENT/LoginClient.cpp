#include <iostream>
#include "LoginClient.h"



LoginClient::LoginClient()
	:elapsedtime(0)
{
}

LoginClient::~LoginClient()
{

}

void LoginClient::update(float delta_time)
{
	elapsedtime += delta_time;
	if (elapsedtime > 1)
	{
		elapsedtime = 0;
		std::cout << "로그인클라" << std::endl;
	}

	//sc_packet_empty packet;
	//packet.size = sizeof(sc_packet_empty);
	//packet.type = SC_PACKET_EMPTY;
	//do_send(&packet, sizeof(packet));

	Client::update(delta_time);
}