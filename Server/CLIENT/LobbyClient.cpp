#include <iostream>

#include "LobbyClient.h"



LobbyClient::LobbyClient()
	:elapsedtime(0)
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
	}

	//send packet
	sc_packet_empty packet;
	packet.size = sizeof(sc_packet_empty);
	packet.type = SC_PACKET_EMPTY;
	do_send(&packet, sizeof(packet));

	Client::update(delta_time);
}