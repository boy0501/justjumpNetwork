#pragma once
#include <vector>
#include "Client.h"

class GameClient : public Client
{
public:
	GameClient();
	virtual ~GameClient();

	virtual void update(float delta_time) override;

	float elapsedtime;
	int mObjectCount;		//Object����
	class Map* mMap;		//obj�� �����ϴ� Map�� ����Ű�� ������
	bool collp2o(class Object* Obj);
	void adjustPlayer();
	void move(float delta_time);
};
