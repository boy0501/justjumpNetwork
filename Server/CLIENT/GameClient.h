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
	int mObjectCount;		//Object개수
	class Map* mMap;		//obj를 관리하는 Map을 가리키는 포인터
	bool collp2o(class Object* Obj);
	void adjustPlayer();
	void move(float delta_time);
};
