#include "GameHUD.h"
#include "player.h"
#include "Camera.h"
#include"Button.h"
GameHUD::GameHUD(const int& cnt,PLAYER& player) 
	:UI(cnt)
	,mPlayer(&player)
{
	//mapNum = 0;
}

GameHUD::~GameHUD()
{
	if (mHpBitmap) DeleteObject(mHpBitmap);
}



void GameHUD::draw(HDC& mem1dc)
{
	drawByUserUi(mem1dc );
	drawHP(mem1dc);
	for (const auto& button : mButtons)
	{
		button->drawByScreenButton(mem1dc);
	}
	UI::draw(mem1dc);
	//drawExit(mem1dc);
	//getMapNum();
	//std::cout <<mapNum  << std::endl;
	//setMapNum(mapNum);

	//if (mapNum == 11)
	//{
	//	drawExit(mem1dc);
	//}

	//if (getMapNum() == 11) {
 //   if (try_once == false) {
	//		this->addButton([this]() {
	//			}
	//		, NULL, "img/Exit", 315, 300, 138, 82, RGB(255, 0, 0));
	//		try_once = true;
	//	}
	//}
	//	
	
	//getMapNum();
}

void GameHUD::drawExit(HDC& mem1dc)
{
	drawByUserUi(mem1dc);
	drawByScreenUi(mem1dc);
	for (const auto& button : mButtons)
	{
		button->drawByUserButton(mem1dc,mCamera);
	}
	UI::drawExit(mem1dc);
	
	
		if (try_once == false) {
			this->addButton([this]() {
				}
			, NULL, "img/Exit", 315, 500, 138, 82, RGB(255, 0, 0));
			try_once = true;
		}

}


void GameHUD::update(float deltatime)
{
	
}

void GameHUD::drawHP(HDC& mem1dc)
{
	int hp = mPlayer->gethp() * 171 / 100;
	TCHAR hpname[100];
	_itow_s(mPlayer->gethp(), hpname, 10);
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
	HDC mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, mHpBitmap);
	BitBlt(mem1dc, mCamera->getx() + 421, mCamera->gety() + 728, hp, 65, mem2dc, 0, 0, SRCCOPY);
	SetBkMode(mem1dc, 1);
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, mCamera->getx() + 481, mCamera->gety() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, mCamera->getx() + 483, mCamera->gety() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, mCamera->getx() + 482, mCamera->gety() + 727, hpname, lstrlenW(hpname));
	TextOut(mem1dc, mCamera->getx() + 482, mCamera->gety() + 729, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, mCamera->getx() + 482, mCamera->gety() + 728, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, mCamera->getx() + 504, mCamera->gety() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, mCamera->getx() + 506, mCamera->gety() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, mCamera->getx() + 505, mCamera->gety() + 727, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, mCamera->getx() + 505, mCamera->gety() + 729, L"/100", lstrlenW(L"/100"));
	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, mCamera->getx() + 505, mCamera->gety() + 728, L"/100", lstrlenW(L"/100"));

	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
	DeleteObject(mem2dc);

	
}



void GameHUD::LoadHpUiBitmap(HINSTANCE& g_hinst, std::string name, const int& x, const int& y, const int& w, const int& h, const COLORREF& cr,CAMERA& camera)
{
	
	auto tmp = std::wstring(name.begin(), name.end());
	auto tmp_convert_wstr = tmp.c_str();
	mHpBitmap = (HBITMAP)LoadImage(g_hinst, tmp_convert_wstr, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	mUiInfo.push_back(UIINFO(x, y, w, h, cr, true));
}
