#pragma once
#include "UI.h"

class LoginHUD : public UI
{
public:
	LoginHUD(const int& cnt,class MAP* map);
	~LoginHUD();

	virtual void draw(HDC& mem1dc);
	virtual void update(float deltatime);
	virtual void processInput(const int& mx, const int& my, const int& command);
	class MAP* mMap;
};

