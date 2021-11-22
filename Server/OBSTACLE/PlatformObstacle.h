#pragma once
#include "Obstacle.h"
class PlatformObstacle : public Obstacle
{
public:
	PlatformObstacle();
	virtual ~PlatformObstacle();
	virtual void update(float deltatime);




};

