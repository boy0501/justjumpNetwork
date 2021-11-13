#pragma once
#include "UI.h"

class ExitHUD : public UI
{
public:
	ExitHUD(const int& cnt);
	~ExitHUD();

	virtual void draw(HDC& mem1dc);
	virtual void update(float deltatime);

	int getMapNum();

private:
	bool try_once = false;
	int mapNum = 11;
};

