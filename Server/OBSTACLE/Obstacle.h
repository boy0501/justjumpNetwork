#pragma once
#include "../Object.h"

class Obstacle : public Object
{
public:
	Obstacle();
	virtual ~Obstacle();
	virtual void update(float deltatime);


	Type mType;

};

