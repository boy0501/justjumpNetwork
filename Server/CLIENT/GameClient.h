#pragma once
#include <vector>
#include "Client.h"

class GameClient : public Client
{
public:
	GameClient();
	GameClient(const GameClient& rhs);
	virtual ~GameClient();

	virtual void update(float delta_time) override;

	float elapsedtime;
	int mObjectCount;		//Object개수
	class Map* mMap;		//obj를 관리하는 Map을 가리키는 포인터
	std::vector<class Object*> mObj;
	bool collp2o(class Object* Obj);
	void adjustPlayer();
};
