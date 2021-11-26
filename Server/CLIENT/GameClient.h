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
	void adjustPlayer(float deltatime);
	void spike_hurttime(float deltatime);
	
	//int obj_t; // for 애니메이션
	void move(float deltatime);
	//void BitMove();
	void initPos();
	void initBitPos();
	void hurt();	//로그인은 hurt필요없고, lobby도 사실상 다칠 일이 없기때문에 이곳에만 넣음
	void stealthtime(float deltatime);
	

private:
	int mRank[3] = { 1,1,1 };//랭킹을 저장할 변수
	
};
