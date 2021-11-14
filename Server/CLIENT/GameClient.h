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
	int mObjectCount;		//Object����
	class Map* mMap;		//obj�� �����ϴ� Map�� ����Ű�� ������
	std::vector<class Object*> mObj;
	bool collp2o(class Object* Obj);
	void adjustPlayer();
};
