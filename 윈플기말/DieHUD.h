#pragma once
#include "UI.h"
class DieHUD : public UI
{
public:
	DieHUD(const int& cnt, class PLAYER& player,class CAMERA& camera);
	~DieHUD();

	virtual void draw(HDC& mem1dc);
	virtual void update(float deltatime);

	void LoadHpUiBitmap(HINSTANCE& g_hinst, std::string name, const int& x, const int& y, const int& w, const int& h, const COLORREF& cr, class CAMERA& camera);
	void drawHP(HDC& mem1dc);
private:
	class PLAYER* mPlayer;
	HBITMAP mHpBitmap;
};

