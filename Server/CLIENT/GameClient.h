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
	
	int obj_t; // for 애니메이션
	void move(int obj_t, float deltatime);
	void BitMove();
	void initPos();
	void initBitPos();
};
