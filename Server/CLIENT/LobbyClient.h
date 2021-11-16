#pragma once
#include "Client.h"
#include <array>
static std::array<class Client*, 3> CLIENTS;

class LobbyClient : public Client
{
public:
	LobbyClient();
	virtual ~LobbyClient();

	virtual void update(float delta_time) override;
	
	float elapsedtime;

	int obj_t;

	void move(int obj_t, float deltatime);
	void BitMove();
	void initBitPos();
	void initPos();

	int robby_timer;
	int robby_cnt = 0;

	bool is_robby = false;

	static class LobbyClient* mLobbyClient;
	static class LobbyClient* GetLobbyClient();
};
