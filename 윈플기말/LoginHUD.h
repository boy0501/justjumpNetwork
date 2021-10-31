#pragma once
#include "UI.h"

class LoginHUD : public UI
{
public:
	LoginHUD(const int& cnt);
	~LoginHUD();

	virtual void draw(HDC& mem1dc);
	virtual void update(float deltatime);
};

