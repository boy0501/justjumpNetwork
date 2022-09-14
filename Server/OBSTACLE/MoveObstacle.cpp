#include "MoveObstacle.h"
#include <iostream>
#include "../../Protocol/protocol.h"

MoveObstacle::MoveObstacle(MoveObstacle::MoveType movetype)
	:mMoveType(movetype)
	,degree(0)
	, vd(0)
{
	mx = 0;
	my = 0;
}

MoveObstacle::~MoveObstacle()
{

}

void MoveObstacle::update(float deltatime)
{
	//360 / GAERCYCLE 초에 한번 완주 
	float olddegree = degree;
	degree += deltatime * GEARCYCLE;
	switch (mMoveType)
	{
	case MoveType::Horizontal:
		mx = sin(degree * PI / 180)* GEARSPEED;
		break;
	case MoveType::Vertical:
		my = sin(degree * PI / 180) * GEARSPEED;
		break;
	}
	if (degree > 360)
		degree = 0;
	vd = (degree - olddegree) / deltatime;
}
