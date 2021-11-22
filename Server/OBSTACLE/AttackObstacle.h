#pragma once
#include "Obstacle.h"
class AttackObstacle : public Obstacle
{
public:
	AttackObstacle(float IndexChangeDelay,int imagefps);
	virtual ~AttackObstacle();
	virtual void update(float deltatime);

	void IndexChange();
	int index;					//애니메이션 돌릴때 배열의 인덱스를 바꿔주어 이미지를 바꾼다
	int mIndexFps;				//인덱스의 fps
	float mIndexChangeDelay;
	float mCurDelay;
};

