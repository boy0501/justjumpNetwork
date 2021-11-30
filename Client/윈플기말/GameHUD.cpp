#include "GameHUD.h"
#include "player.h"
#include "Camera.h"
#include"Button.h"
#include<atlstr.h>
#include"Network.h"
#include<string>
#include"Text.h"

GameHUD::GameHUD(const int& cnt,PLAYER& player, PLAYER* others) 
	:UI(cnt)
	,mPlayer(&player)
	,mOthers(others)
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

	//drawMyRanking(mem1dc);
	drawOtherPlayerRanking(mem1dc);

		
	UI::draw(mem1dc);
	
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
				exit(1);
				}
			, NULL, "img/Exit", 800,500, 138, 82, RGB(255, 0, 0));
			try_once = true;
		}
}



void GameHUD::update(float deltatime)
{

}


void GameHUD::drawMyRanking(HDC& mem1dc)
{
	EnterCriticalSection(&mPlayer->cs);
	auto localranking = mPlayer->rank;
	auto localplayerName = mPlayer->mPlayerwname;
	LeaveCriticalSection(&mPlayer->cs);

	int ranking = mPlayer->getRanking();

	TCHAR playerRanking[100];

	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 bold"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
	SetTextColor(mem1dc, RGB(255, 255, 0));

	_itow_s(mPlayer->getRanking(), playerRanking, 10);
	TextOut(mem1dc, mCamera->getx() + 80, mCamera->gety() + 30, mPlayer->mPlayerwname.c_str(), lstrlenW(mPlayer->mPlayerwname.c_str()));
	TextOut(mem1dc, mCamera->getx() + 40, mCamera->gety() + 30, playerRanking, lstrlenW(playerRanking));
	TextOut(mem1dc, mCamera->getx() + 50, mCamera->gety() + 30, L"등", lstrlenW(L"등"));

	SelectObject(mem1dc, oldfont); 
	DeleteObject(hfont);
}

void GameHUD::drawOtherPlayerRanking(HDC& mem1dc)
{
	//EnterCriticalSection(&mPlayer->cs);
	//auto localMyC_id = mPlayer->player_cid;
	//LeaveCriticalSection(&mPlayer->cs);

	//EnterCriticalSection(&mOthers[0].cs);
	//auto localother1Name = mOthers[0].mPlayerwname;
	//auto localother1Rank = mOthers[0].rank;
	//auto localother1c_id = mOthers[0].player_cid;
	//LeaveCriticalSection(&mOthers[0].cs);

	//EnterCriticalSection(&mOthers[1].cs);
	////로컬로 캐싱
	//LeaveCriticalSection(&mOthers[1].cs);

	//EnterCriticalSection(&mOthers[2].cs);
	////로컬로 캐싱
	//LeaveCriticalSection(&mOthers[2].cs);
	
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 bold"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
	SetTextColor(mem1dc, RGB(255, 105, 180));

	TextOut(mem1dc, mCamera->getx() + 40, mCamera->gety() + 30, L"1등", lstrlenW(L"1등"));
	TextOut(mem1dc, mCamera->getx() + 40, mCamera->gety() + 60, L"2등", lstrlenW(L"2등"));
	TextOut(mem1dc, mCamera->getx() + 40, mCamera->gety() + 90, L"3등", lstrlenW(L"3등"));
	

	for (int i = 0; i < 3; ++i)
	{
		if (mPlayer->player_cid == i)
		{
			SetTextColor(mem1dc, RGB(200, 200, 0));
			if (mPlayer->getRanking() == 1)
			{
				TextOut(mem1dc, mCamera->getx() + 60 , mCamera->gety() + 30, mPlayer->mPlayerwname.c_str(), lstrlenW(mPlayer->mPlayerwname.c_str()));
			}
			if (mPlayer->getRanking() == 2)
			{
				TextOut(mem1dc, mCamera->getx() + 60, mCamera->gety() + 60, mPlayer->mPlayerwname.c_str(), lstrlenW(mPlayer->mPlayerwname.c_str()));
			}
			if (mPlayer->getRanking() == 3)
			{
				TextOut(mem1dc, mCamera->getx() + 60 , mCamera->gety() + 90, mPlayer->mPlayerwname.c_str(), lstrlenW(mPlayer->mPlayerwname.c_str()));
			}
		}

		else
		{
			SetTextColor(mem1dc, RGB(210, 210, 210));
			
			if (mOthers[i].getRanking() == 1)
			{
				TextOut(mem1dc, mCamera->getx() + 60 + mPlayer->mPlayerwname.size() * 10 + mOthers[i].mPlayername.size()*i*10, mCamera->gety() + 30, mOthers[i].mPlayerwname.c_str(), lstrlenW(mOthers[i].mPlayerwname.c_str()));
				
			}
			if (mOthers[i].getRanking() == 2)
			{
				TextOut(mem1dc, mCamera->getx() + 60 + mPlayer->mPlayerwname.size() * 10 + mOthers[i].mPlayername.size()*i * 10, mCamera->gety() + 60, mOthers[i].mPlayerwname.c_str(), lstrlenW(mOthers[i].mPlayerwname.c_str()));
				
			}
			if (mOthers[i].getRanking() == 3)
			{
				TextOut(mem1dc, mCamera->getx() + 60 + mPlayer->mPlayerwname.size() * 10 + mOthers[i].mPlayername.size()*i * 10, mCamera->gety() + 90, mOthers[i].mPlayerwname.c_str(), lstrlenW(mOthers[i].mPlayerwname.c_str()));
				
			}
			
				
		}
	}



	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
}




void GameHUD::drawHP(HDC& mem1dc)
{
	EnterCriticalSection(&mPlayer->cs);
	int localhp = mPlayer->hp;
	LeaveCriticalSection(&mPlayer->cs);

	int hp = localhp * 171 / 100;
	TCHAR hpname[100];
	_itow_s(localhp, hpname, 10);
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
