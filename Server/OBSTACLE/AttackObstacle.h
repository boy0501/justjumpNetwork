#pragma once
#include "Obstacle.h"
class AttackObstacle : public Obstacle
{
public:
	AttackObstacle(float IndexChangeDelay,int imagefps);
	virtual ~AttackObstacle();
	virtual void update(float deltatime);

	void IndexChange();
	int index;					//�ִϸ��̼� ������ �迭�� �ε����� �ٲ��־� �̹����� �ٲ۴�
	int mIndexFps;				//�ε����� fps
	float mIndexChangeDelay;
	float mCurDelay;
};

