#pragma once
#include "UI.h"

class OkHUD : public UI
{
public:
	OkHUD(const int& cnt);
	~OkHUD();

	virtual void draw(HDC& mem1dc);
	virtual void update(float deltatime);

};

