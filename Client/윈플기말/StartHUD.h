#pragma once
#include "UI.h"

class StartHUD : public UI
{
public:
	StartHUD(const int& cnt);
	~StartHUD();

	virtual void draw(HDC& mem1dc);
	virtual void update(float deltatime);
};

