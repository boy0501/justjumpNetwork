#pragma once
#include "UI.h"
#include "Map.h"


class GameHUD :public UI
{
public:
	GameHUD(const int& cnt,class PLAYER& player, PLAYER* others);
	~GameHUD();

	virtual void draw(HDC& mem1dc);
	virtual void update(float deltatime);
	void drawExit(HDC& mem1dc);
	//virtual void drawRanking(HDC& mem1dc);

	void LoadHpUiBitmap(HINSTANCE& g_hinst, std::string name, const int& x, const int& y, const int& w, const int& h, const COLORREF& cr, class CAMERA& camera);
	void drawHP(HDC& mem1dc);
	void drawMyRanking(HDC& mem1dc);
	void drawOtherPlayerRanking(HDC& mem1dc);
	
	
private:
	class PLAYER* mPlayer;
	class PLAYER* mOthers;
	HBITMAP mHpBitmap;
	bool try_once = false;
	

};

