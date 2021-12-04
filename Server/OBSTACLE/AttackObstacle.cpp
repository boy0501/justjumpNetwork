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
	//indexchangedelay(초) 만큼 기다렸다가 indexchange
	//ex) indexchangedelay가 1이면 1초마다 들어감.
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