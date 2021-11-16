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
	int mObjectCount;		//Object개수
	class Map* mMap;		//obj를 관리하는 Map을 가리키는 포인터

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
