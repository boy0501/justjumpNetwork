#pragma once
#include "Client.h"
#include <array>
extern std::array<class Client*, 3> CLIENTS;


class LobbyClient : public Client
{
public:
	LobbyClient();
	virtual ~LobbyClient();

	virtual void update(float delta_time) override;
	
	float elapsedtime;

	//int obj_t;
	int mObjectCount;		//Object개수
	class Map* mMap;		//obj를 관리하는 Map을 가리키는 포인터

	void move(float deltatime);
	//void BitMove();
	void initBitPos();
	void initPos();

	int robby_timer = 11;
	int robby_cnt = 0;

	bool is_robby = false;

	bool collp2o(class Object* Obj);
	void adjustPlayer();
};
