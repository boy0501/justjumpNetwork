#pragma once
#include "Client.h"

class LobbyClient : public Client
{
public:
	LobbyClient();
	virtual ~LobbyClient();

	virtual void update(float delta_time) override;
	
	float elapsedtime;

	int obj_t;
	int mObjectCount;		//Object����
	class Map* mMap;		//obj�� �����ϴ� Map�� ����Ű�� ������

	void move(int obj_t, float deltatime);
	void BitMove();
	void initBitPos();
	void initPos();
};
