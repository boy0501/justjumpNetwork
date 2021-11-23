#pragma once
#include "../Object.h"
class Interact : public Object
{
public:
	Interact();
	virtual ~Interact();
	virtual void update(float deltatime);
};

