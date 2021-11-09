#pragma once
#include "UI.h"
class ExitGameHUD : public UI
{
public:
	ExitGameHUD(const int& cnt);
	~ExitGameHUD();

	virtual void draw(HDC& mem1dc);
	virtual void update(float deltatime);
};

