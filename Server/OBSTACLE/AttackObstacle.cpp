#include "AttackObstacle.h"
#include <iostream>


AttackObstacle::AttackObstacle(float IndexChangeDelay,int imagefps)
	:mIndexChangeDelay(IndexChangeDelay)
	,mIndexFps(imagefps)
	,mCurDelay(0.f)
	,index(0)
{

}

AttackObstacle::~AttackObstacle()
{

}

void AttackObstacle::update(float deltatime)
{
	mCurDelay = std::max(mCurDelay - deltatime, 0.0f);
	if (mCurDelay <= 0)
	{
		IndexChange();
	}
	
}

void AttackObstacle::IndexChange()
{
	index = (index + 1) % mIndexFps;
	mCurDelay = mIndexChangeDelay;
}